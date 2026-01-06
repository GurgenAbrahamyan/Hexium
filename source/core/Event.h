#pragma once

#include "../shapes/Object3D.h"

class Event {
};

class CreateObject : Event {
public:
    Vector3* position;

    CreateObject(Vector3 position) : position(&position) {}
};

class StopEngine : Event {

public:
    StopEngine() {}

};

class PressedKey : Event {

public:
    char key;
    PressedKey(char e) {
        key = e;
    }
};

class MouseDragged : Event {
public:
    double x, y;
    MouseDragged(double x, double y) : x(x), y(y){}


};

class CameraMode : Event {
public:
    bool key;
    CameraMode(bool e) {
        key = e;
    }
};

class UiMode : Event {
public:
    bool key;
    UiMode(bool e) {
        key = e;
    }


};





