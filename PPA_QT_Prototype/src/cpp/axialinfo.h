#ifndef AXIALINFO_H
#define AXIALINFO_H

typedef enum {
    Linear,
    Exponential
} GraphMode;

struct AxialInfo {
    GraphMode mode;
    float min, max;
    QList<float> subminor;
    QList<float> minor;
    QList<float> major;
    QList<float> tick;
    QList<float> label;
};

#endif // AXIALINFO_H
