#pragma once
#include "../data/RenderContext.h"
#include "../data/BatchMap.h"
#include "../backend/containers/GlobalUniformBuffer.h"
#include "../data/GPULight.h"
#include "../../math_custom/Mat4.h"
#include "../../ecs/components/core/TransformComponent.h"
#include "../../ecs/components/core/ParentComponent.h"
#include "../../ecs/components/graphics/MeshComponent.h"
#include "../../ecs/components/graphics/MaterialComponent.h"
#include "../../ecs/components/graphics/LightComponent.h"
#include "../../ecs/components/graphics/CubeMapComponent.h"
#include "../../ecs/components/graphics/CameraComponent.h"
#include "../../ecs/components/graphics/ActiveCameraTag.h"
#include "../../ecs/components/core/WorldMatrixComponent.h"
#include "../../ecs/components/graphics/ModelComponent.h"
#include "../backend/Shader.h"
#include "../../math_custom/GLAdapter.h"
#include <glad/glad.h>

class RenderHandler {
protected:
    Shader* shader;
public:
    RenderHandler(Shader* s) : shader(s) {}
    virtual void renderBatch(const RenderContext& context) {}
};

// ?????????????????????????????????????????????????????????????????????????????

class ObjectRenderer : public RenderHandler {
public:
    ObjectRenderer(Shader* s) : RenderHandler(s) {}

    void renderBatch(const RenderContext& context) override {
        auto& registry = *context.registry;

      
        entt::entity camEntity = entt::null;
        for (auto entity : registry.view<ActiveCameraTag>()) {
            camEntity = entity;
            break;
        }
        if (camEntity == entt::null) return;

        auto* cam = registry.try_get<CameraComponent>(camEntity);
        auto* camTrans = registry.try_get<TransformComponent>(camEntity);
        if (!cam || !camTrans) return;

  

        uploadLights(registry);
        rebuildBatches(registry);

        shader->Activate();
        shader->setMat4("view", cam->viewMatrix);
        shader->setMat4("projection", cam->projectionMatrix);
        shader->setVec3("cameraPos", camTrans->position);

        for (auto& [mat, meshMap] : renderBatches) {
            mat->Bind(shader);
            for (auto& [mesh, batch] : meshMap) {
                if (batch.instances.empty()) continue;
                mesh->bind();
                mesh->setupInstanceVBO(batch.instances.size());
                glBindBuffer(GL_ARRAY_BUFFER, mesh->getInstanceVBO());
                glBufferSubData(GL_ARRAY_BUFFER, 0,
                    batch.instances.size() * sizeof(Mat4),
                    batch.instances.data());
                glDrawElementsInstanced(GL_TRIANGLES, mesh->indexCount(),
                    GL_UNSIGNED_INT, 0, batch.instances.size());
            }
        }
    }

private:
    std::unordered_map<Material*, std::unordered_map<RenderMesh*, MeshBatch>> renderBatches;

    UniformBuffer* lightsUBO = new UniformBuffer(
        sizeof(GPULight) * 32 + sizeof(int) + 12, 1
    );

    Mat4 getWorldTransform(entt::entity entity, entt::registry& registry) {
      
        Mat4 local = Mat4(); 

        if (auto* tc = registry.try_get<TransformComponent>(entity)) {
            local = 
                Mat4::scale(tc->scale)*
                Mat4::fromQuat(tc->rotation) *
                Mat4::translate(tc->position)
                ;
        }

  
        if (auto* parentComp = registry.try_get<ParentComponent>(entity)) {
            if (registry.valid(parentComp->parent)) {
                return getWorldTransform(parentComp->parent, registry) * local;
            }
        }

        return  local;
    }


    void uploadLights(entt::registry& registry) {
        std::vector<GPULight> gpuLights;

        registry.view<LightComponent, TransformComponent>().each(
            [&](entt::entity entity, LightComponent& lc, TransformComponent& tc) {
                GPULight l = {};
                l.type = static_cast<int>(lc.type);
                l.intensity = lc.intensity;
                l.color = lc.color;
                l.position = GLAdapter::toGL(tc.position);
                l.direction = GLAdapter::toGL(lc.direction);
                l.innerCone = lc.innerConeAngle;
                l.outerCone = lc.outerConeAngle;
                gpuLights.push_back(l);
            }
        );
      
    
        lightsUBO->bind();
        lightsUBO->update(gpuLights.data(), gpuLights.size() * sizeof(GPULight), 0);
        int lightCount = static_cast<int>(gpuLights.size());
        lightsUBO->update(&lightCount, sizeof(int), sizeof(GPULight) * 32);
    }

    void rebuildBatches(entt::registry& registry) {
        for (auto& [mat, meshMap] : renderBatches)
            for (auto& [mesh, batch] : meshMap)
                batch.instances.clear();

        // Pseudocode for ModelComponent-based batching
        registry.view<ModelComponent>().each(
            [&](entt::entity entity, ModelComponent& model) {
                Mat4 entityWorld = GLAdapter::toGL(getWorldTransform(entity, registry));

                for (auto& entry : model.meshes) {
                    if (!entry.mesh) continue;
                    Mat4 finalTransform = entry.localTransform* entityWorld;
                    renderBatches[entry.material][entry.mesh].instances.push_back(finalTransform);
                }
            }
        );
    }
};



class CubeMapRenderer : public RenderHandler {
public:
    CubeMapRenderer(Shader* s) : RenderHandler(s) {}

    void renderBatch(const RenderContext& context) override {
        glDisable(GL_CULL_FACE);
        auto& registry = *context.registry;

        
        entt::entity camEntity = entt::null;
        for (auto entity : registry.view<ActiveCameraTag>()) {
            camEntity = entity;
            break;
        }
        if (camEntity == entt::null) return;

        auto* cam = registry.try_get<CameraComponent>(camEntity);
        auto* camTrans = registry.try_get<TransformComponent>(camEntity);
        if (!cam || !camTrans) return;

        auto skyView = registry.view<CubeMapComponent>();
        if (skyView.empty()) return;
        CubeMapComponent& cmc = registry.get<CubeMapComponent>(skyView.front());
        if (!cmc.cubeMap) return;

        shader->Activate();

        glDepthFunc(GL_LEQUAL);
      

        Mat4 view_mat = cam->viewMatrix;
        view_mat.data[12] = 0.0f;
        view_mat.data[13] = 0.0f;
        view_mat.data[14] = 0.0f;

        shader->setInt("skybox", 0);
        shader->setMat4("projection", cam->projectionMatrix);
        shader->setMat4("view", view_mat);

        cmc.cubeMap->bind();
        cmc.cubeMap->bindTexture();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthFunc(GL_LESS);
       // glEnable(GL_CULL_FACE);
    }
};