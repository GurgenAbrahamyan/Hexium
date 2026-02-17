#pragma once



#include "../../../math_custom/Vector3.h"
#include "../../../math_custom/Mat4.h"

struct CameraComponent {
   
    float fov = 45.0f;
    float aspectRatio = 16.0f / 9.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;

    float pitch = 0.0f;
    float yaw = 90.0f;  

    Vector3 front = Vector3(0, 0, -1);
    Vector3 right = Vector3(1, 0, 0);
    Vector3 up = Vector3(0, 1, 0);

    Mat4 viewMatrix;
    Mat4 projectionMatrix;
};