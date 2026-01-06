#pragma once

#include "../math_custom/Vector3.h"
#include "../math_custom/Mat4.h"
#include "../core/EventBus.h"
#include "../core/Event.h"
#include "../core/EngineContext.h"
#include <iostream>

class Camera {
public:
    Camera(Vector3 pos, Vector3 rot, EventBus* bus);
    Camera(EventBus* bus);

    void processKeyboard(char key, float deltaTime);
    void processMouse(float xoffset, float yoffset);

    Mat4 getViewMatrix();
    Mat4 getProjectionMatrix();
    Vector3 getPosition();
    Vector3 getFront();
    Vector3 getUp();

    void setPerspective(float fovDeg, float aspect, float nearPlane, float farPlane);
    void setOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane);

private:
    void updateVectors();

    Vector3 Position;
    Vector3 Rotation;
    Vector3 Front;
    Vector3 Up;
    Vector3 Right;

    EventBus* bus;
    Mat4 projection;

    float MovementSpeed = 20.0f;
    float MouseSensitivity = 0.1f;

    bool CamMode;
};
