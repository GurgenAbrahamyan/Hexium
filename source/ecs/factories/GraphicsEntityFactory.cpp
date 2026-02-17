#include "GraphicsEntityFactory.h"
#include "../components/core/TransformComponent.h"
#include "../components/core/TagComponent.h"
#include "../components/graphics/LightComponent.h"
#include "../components/graphics/CubeMapComponent.h"
#include "../components/graphics/MeshComponent.h"
#include "../../resources/managers/MeshManager.h"
#include "../components/graphics/CameraComponent.h"
#include "../components/graphics/ActiveCameraTag.h"


namespace GraphicsEntityFactory {

    entt::entity createRectangle(
        entt::registry& registry,
        MeshManager& meshManager,
        const std::string& tag,
        Vector3 position,
        Vector3 scale)
    {
        auto entity = registry.create();
        registry.emplace<TagComponent>(entity, tag);
      //  registry.emplace<TransformComponent>(entity, position, Quat(), scale);
        registry.emplace<MeshComponent>(entity, meshManager.getRectangleMesh());
        return entity;
    }

    entt::entity createLight(
        entt::registry& registry,
        LightType type,
        Vector3 color,
        float intensity,
        Vector3 position,
        Vector3 direction,
        float innerConeAngle,
        float outerConeAngle,
        const std::string& tag)
    {
        auto entity = registry.create();
        registry.emplace<TagComponent>(entity, tag);
        registry.emplace<TransformComponent>(entity, position, Quat(), Vector3(1, 1, 1));
        registry.emplace<LightComponent>(entity, type, color, direction, intensity, innerConeAngle, outerConeAngle);
        return entity;
    }

    entt::entity createSkybox(
        entt::registry& registry,
        CubeMap* cubeMap)
    {
        auto view = registry.view<CubeMapComponent>();
        assert(view.size() == 0 && "Only one skybox allowed!");
        auto entity = registry.create();
        registry.emplace<TagComponent>(entity, "skybox");
        registry.emplace<CubeMapComponent>(entity, cubeMap);
        return entity;
    }

    entt::entity createCamera(
        entt::registry& registry,
        Vector3 position,
        float fov,
        float aspectRatio,
        float nearPlane,
        float farPlane,
        bool makeActive,
        const std::string& tag)
    {
        auto entity = registry.create();
        registry.emplace<TagComponent>(entity, tag);
        registry.emplace<TransformComponent>(entity, position, Quat(),Vector3(1, 1, 1));
        registry.emplace<CameraComponent>(entity, fov, aspectRatio, nearPlane, farPlane);

        if (makeActive) {
            registry.clear<ActiveCameraTag>();
            registry.emplace<ActiveCameraTag>(entity);
        }

        return entity;
    }
}