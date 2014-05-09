import QtQuick 2.1
import QtQuick.Controls 1.1

Rectangle {
    property var delegate
    property bool fixedWidthRows: true
    property var componentTemplate;
    property int viewSpacing: 3
    property int fitToScreenCount: -1

    color: "transparent"

    /* Delegate Methods
     numberOfItems
     configForIndex
     widthOfItemAtIndex
    */

    QtObject {
        id: internal
        property var _itemCount: 0
        property var _cachedViews: []
        property var _visibleItems: []

        property var _firstCreatedIndex: 0
        property var _lastCreatedIndex: 0

    }

    Component {
        id: itemContainer
        Item {
            property var containedView
        }
    }

    Flickable {
        id: hScrollView
        width: parent.width
        height: parent.height

        maximumFlickVelocity: 1200

        flickableDirection: Flickable.HorizontalFlick
        Row {
            id: rowRoot
            spacing: viewSpacing


            width: parent.width
            height: parent.height
        }

        Timer {
            repeat: true
            running: Math.abs(hScrollView.horizontalVelocity) > 0
            triggeredOnStart: true
            interval: 50
            onTriggered: onScroll()
        }

        Timer {
            repeat: true
            running: Math.abs(hScrollView.horizontalVelocity) > 0
            triggeredOnStart: false
            interval: 1000
            onTriggered: cull()
        }

        onWidthChanged: {
            var last = rowRoot.children[rowRoot.children.length - 1],
                lastPos;
            if (last) {
                hScrollView.contentWidth = last.x + last.width + viewSpacing;
                //onScroll();
            }
        }
    }

    Component.onCompleted: {
        if (delegate) {
            internal._itemCount = delegate.numberOfViews();

            //TODO: Fix for non-fixed widths
            hScrollView.contentHeight = hScrollView.height;

            var accumulatedWidth = 0;

            //Populate the horizontal container with components
            for (var i = internal._firstCreatedIndex; i < internal._itemCount; i++) {
                var config = delegate.configForIndex(i),
                    width = delegate.widthOfViewAtIndex();

                var view = componentTemplate.createObject(rowRoot, config);
                view.index = i;

                internal._visibleItems.push(view);
                accumulatedWidth += width + viewSpacing;

                if (accumulatedWidth > parent.width) {
                    internal._lastCreatedIndex = i;
                    break;
                }
            }

            hScrollView.contentWidth = accumulatedWidth;
        }
    }

    function dequeueView() {
        return internal._cachedViews.shift();
    }

    function prepareView(view, isVisible) {
        for (var i = 0; i < view.children.length;i++) {
            view.children[i].visible = isVisible;
        }
    }

    /**
    * Cull is called every second and hides items are are no longer within the bounds of the viewable region
    *
    */
    function cull() {
        var keepList = [],
            visibleItems = internal._visibleItems;

        //Look from left to right
        for (var i = 0; i < visibleItems.length;i++) {
            var view = visibleItems[i],
                minPos = view.mapToItem(hScrollView, 0, 0),
                maxPos = view.mapToItem(hScrollView, view.width, 0),
                bufferSize = 100; //TODO: More magic numbers

            if ((minPos.x >= -bufferSize && maxPos.x <= hScrollView.width + bufferSize)) {
                keepList.push(view);
            }
            else {
                prepareView(view, false);
            }
        }

        internal._visibleItems = keepList;
    }

    /**
    * onScroll happens every 50 milliseconds and calculates when the scroll offset has exceeded the number of rendered items
    * and, if so, creates new views to populate the scrollable view
    */
    function onScroll() {
        var visibleItems = internal._visibleItems,
            lastView = visibleItems[visibleItems.length - 1],
            lastViewPos = lastView.mapToItem(hScrollView, lastView ? lastView.width : 0, 0),
            processItems = false,
            sView = hScrollView;

        if (!lastView) return;

        //TODO: Get rid of magic margin
        if (lastViewPos.x + viewSpacing <= hScrollView.width) { //Only handle incoming

            //Add a new view
            var incomingIndex = lastView.index + 1;

            if (incomingIndex > delegate.numberOfViews()) {
                hScrollView.returnToBounds();
                return;
            }

            var newView = rowRoot.children[incomingIndex];

            if (!newView) {
                var newConfig = delegate.configForIndex(lastView.index + 1);
                newView = componentTemplate.createObject(rowRoot, newConfig);
                newView.index = incomingIndex;

                hScrollView.contentWidth += newView.width + viewSpacing;

                prepareView(newView, true);
            }
            else {
                prepareView(newView, true);
            }

            visibleItems.push(newView);
        }

        var firstView = visibleItems[0],
            firstViewPos = firstView.mapToItem(hScrollView, 0, 0);

        if (firstViewPos.x - viewSpacing > 0) {
            incomingIndex = firstView.index - 1;
            newView = rowRoot.children[incomingIndex];

            if (incomingIndex < 0) {
                hScrollView.returnToBounds();
                return;
            }

            visibleItems.unshift(newView);
            prepareView(newView, true);
        }
    }
}
