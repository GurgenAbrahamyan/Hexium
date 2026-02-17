#pragma once
#include "../../include/EnTT/entt.hpp"
#include "../../math_custom/Vector3.h"
#include "../../math_custom/Quat.h"

class CubeMap;
class MeshManager;
enum class LightType;

namespace GraphicsEntityFactory {

    // Primitives only — real models go through ModelManager::loadModel()

    entt::entity createRectangle(
        entt::registry& registry,
        MeshManager& meshManager,
        const std::string& tag,
        Vector3 position = Vector3(0, 0, 0),
        Vector3 scale = Vector3(1, 1, 1)
    );

    entt::entity createLight(
        entt::registry& registry,
        LightType type,
        Vector3 color,
        float intensity,
        Vector3 position,
        Vector3 direction = Vector3(0, 0, -1),
        float innerConeAngle = 12.5f,
        float outerConeAngle = 17.5f,
        const std::string& tag = "light"
    );

    entt::entity createSkybox(
        entt::registry& registry,
        CubeMap* cubeMap
    );

    entt::entity createCamera(
        entt::registry& registry,
        Vector3 position = Vector3(0, 0, 0),
        float fov = 45.0f,
        float aspectRatio = 16.0f / 9.0f,
        float nearPlane = 0.1f,
        float farPlane = 1000.0f,
        bool  makeActive = true,
        const std::string& tag = "camera"
    );
}
