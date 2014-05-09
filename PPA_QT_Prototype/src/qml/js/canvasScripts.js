var globalPts = [];

function getPoints() {
    return globalPts;
}

function updatePoint(index, x, y) {
    //console.log("Updating index: " + index + " With(" + x + "," + y + ")")
    globalPts[index].x = x;
    globalPts[index].y = y;
}
