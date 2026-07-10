#ifndef INCLUDE_H
#define INCLUDE_H

#include <touch.h>
#include <math.h>

struct Vector2A {
    float X, Y;
    Vector2A() : X(0), Y(0) {}
    Vector2A(float x, float y) : X(x), Y(y) {}
};

struct Vector3A {
    float X, Y, Z;
    Vector3A() : X(0), Y(0), Z(0) {}
    Vector3A(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

static float get_3D_Distance(float x1, float y1, float z1, float x2, float y2, float z2) {
    return sqrtf(powf(x1 - x2, 2) + powf(y1 - y2, 2) + powf(z1 - z2, 2));
}

extern int AimCount;
extern int MaxPlayerCount;
extern int Gmin;

extern float matrix[16];
extern float px, py;

#endif
