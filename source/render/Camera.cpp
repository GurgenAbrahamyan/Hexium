#include "Camera.h"
#include <cmath>

Camera::Camera(Vector3 pos, Vector3 rot, EventBus* bus)
    : Position(pos),
    Rotation(rot),
    Front(0, 0, -1),
    Up(0, 1, 0),
    Right(1, 0, 0),
    bus(bus)
{
    projection = Mat4::perspective(45.0f,
        (float)EngineContext::get().windowWidth / (float)EngineContext::get().windowHeight,
        EngineContext::get().nearPlane, EngineContext::get().farPlane);

    bus->subscribe<PressedKey>([this](PressedKey& event) {
        this->processKeyboard(event.key, EngineContext::get().deltaTime);
        });

    bus->subscribe<CameraMode>([this](CameraMode& event) {
        this->CamMode = event.key;
        });

    bus->subscribe<MouseDragged>([this](MouseDragged& event) {
        processMouse(event.x, event.y);
        });

    updateVectors();
}

Camera::Camera(EventBus* bus)
    : Camera(Vector3(0, 0, 10), Vector3(0, 0, 0), bus) {
}

void Camera::processKeyboard(char key, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (key == 'W') Position = Position.add(Front.multiply(velocity));
    if (key == 'S') Position = Position.add(Front.multiply(-velocity));
    if (key == 'A') Position = Position.add(Right.multiply(-velocity));
    if (key == 'D') Position = Position.add(Right.multiply(velocity));
    if (key == 'Q') Position = Position.add(Up.multiply(-velocity));
    if (key == 'E') Position = Position.add(Up.multiply(velocity));

    std::cout << "Camera Position: ("
        << Position.getX() << ", "
        << Position.getY() << ", "
        << Position.getZ() << ")\n";
}

void Camera::processMouse(float xoffset, float yoffset) {
    if (!CamMode) return;

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Rotation.x += yoffset;
    Rotation.y += xoffset;

    if (Rotation.x > 89.0f) Rotation.x = 89.0f;
    if (Rotation.x < -89.0f) Rotation.x = -89.0f;

    updateVectors();

    if (Rotation.y > 360) Rotation.y -= 360;
    if (Rotation.y < -360) Rotation.y += 360;

    std::cout << "Mouse offset (pixels): x=" << xoffset / MouseSensitivity
        << ", y=" << yoffset / MouseSensitivity
        << " | Rotation: pitch=" << Rotation.x
        << ", yaw=" << Rotation.y << std::endl;
}

Mat4 Camera::getViewMatrix() {
    return Mat4::lookAt(Position, Position.add(Front), Up);
}

Mat4 Camera::getProjectionMatrix() {
    return projection;
}

Vector3 Camera::getPosition() { return Position; }
Vector3 Camera::getFront() { return Front; }
Vector3 Camera::getUp() { return Up; }

void Camera::setPerspective(float fovDeg, float aspect, float nearPlane, float farPlane) {
    projection = Mat4::perspective(fovDeg, aspect, nearPlane, farPlane);
}

void Camera::setOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    projection = Mat4::ortho(left, right, bottom, top, nearPlane, farPlane);
}

void Camera::updateVectors() {
    float yaw = Mat4::radians(Rotation.y);
    float pitch = Mat4::radians(Rotation.x);

    Front.setX(cos(yaw) * cos(pitch));
    Front.setY(sin(pitch));
    Front.setZ(sin(yaw) * cos(pitch));
    Front = Front.normalize();

    Right = Front.cross(Vector3(0, 1, 0)).normalize();
    Up = Right.cross(Front).normalize();
}