
#include "CameraSystem.h"
#include <cmath>
#include <iostream>
#include "../../ecs/components/core/TransformComponent.h"
#include "../../ecs/components/graphics/CameraComponent.h"
#include "../../ecs/components/graphics/ActiveCameraTag.h"
#include "../../core/Event.h"
#include "../EngineContext.h"
#include "../../math_custom/Mat4.h"
#include "../../math_custom/GLAdapter.h"

CameraSystem::CameraSystem(EventBus* bus, entt::registry& registry) : registry(registry) {

    bus->subscribe<PressedKey>([this](PressedKey& event) {
        this->processKeyboard(this->registry, event.key, EngineContext::get().deltaTime);
        });

    bus->subscribe<CameraMode>([this](CameraMode& event) {
        camMode = event.key;
        });
    bus->subscribe<MouseDragged>([this](MouseDragged& event) {
        processMouse(this->registry, event.x, event.y);
        });
}

void CameraSystem::update(entt::registry& registry, float dt) {
    updateMatrices(registry);
}


void CameraSystem::processKeyboard(entt::registry& registry, char key, float dt) {
    entt::entity camEntity = entt::null;
    for (auto entity : registry.view<ActiveCameraTag>())
    {
        camEntity = entity; break;
    }
    if (camEntity == entt::null) return;

    auto* cam = registry.try_get<CameraComponent>(camEntity);
    auto* transform = registry.try_get<TransformComponent>(camEntity);
    if (!cam || !transform) return;

    float velocity = movementSpeed * dt;

    if (key == 'W') transform->position = transform->position + cam->front * velocity;
    if (key == 'S') transform->position = transform->position - cam->front * velocity;
    if (key == 'A') transform->position = transform->position - cam->right * velocity;
    if (key == 'D') transform->position = transform->position + cam->right * velocity;
    if (key == 'Q') transform->position = transform->position - cam->up * velocity;
    if (key == 'E') transform->position = transform->position + cam->up * velocity;

    std::cout << "Camera Position: ("
        << transform->position.x << ", "
        << transform->position.y << ", "
        << transform->position.z << ")\n";
}

void CameraSystem::processMouse(entt::registry& registry, float xoffset, float yoffset) {
    if (!camMode) return;

    entt::entity camEntity = entt::null;
    for (auto entity : registry.view<ActiveCameraTag>())
    {
        camEntity = entity; break;
    }
    if (camEntity == entt::null) return;

    auto* cam = registry.try_get<CameraComponent>(camEntity);
    if (!cam) return;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    cam->yaw += xoffset;
    cam->pitch += yoffset;

    if (cam->pitch > 89.0f) cam->pitch = 89.0f;
    if (cam->pitch < -89.0f) cam->pitch = -89.0f;
    if (cam->yaw > 360.f) cam->yaw -= 360.0f;
    if (cam->yaw < -360.f) cam->yaw += 360.0f;

    std::cout << "Mouse offset (pixels): x=" << xoffset / mouseSensitivity
        << ", y=" << yoffset / mouseSensitivity;

    updateVectors(registry);
}

void CameraSystem::updateVectors(entt::registry& registry) {
    entt::entity camEntity = entt::null;
    for (auto entity : registry.view<ActiveCameraTag>())
    {
        camEntity = entity; break;
    }
    if (camEntity == entt::null) return;

    auto* cam = registry.try_get<CameraComponent>(camEntity);
    if (!cam) return;

    float yaw = Mat4::radians(cam->yaw);
    float pitch = Mat4::radians(cam->pitch);

    cam->front.x = cos(yaw) * cos(pitch);
    cam->front.y = sin(pitch);
    cam->front.z = sin(yaw) * cos(pitch);
    cam->front = cam->front.normalized();
    cam->right = cam->front.cross(Vector3(0, 1, 0)).normalized();
    cam->up = cam->right.cross(cam->front).normalized();
}

void CameraSystem::updateMatrices(entt::registry& registry) {
    entt::entity camEntity = entt::null;
    for (auto entity : registry.view<ActiveCameraTag>())
    {
        camEntity = entity; break;
    }
    if (camEntity == entt::null) return;

    auto* cam = registry.try_get<CameraComponent>(camEntity);
    auto* transform = registry.try_get<TransformComponent>(camEntity);
    if (!cam || !transform) return;

    cam->viewMatrix = Mat4::lookAt(
        transform->position,
        transform->position + cam->front,
        cam->up
    );
    cam->projectionMatrix = Mat4::perspective(
        cam->fov,
        cam->aspectRatio,
        cam->nearPlane,
        cam->farPlane
    );
}