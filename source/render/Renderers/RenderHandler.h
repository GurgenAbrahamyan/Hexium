#pragma once
#include "../RenderHelpers/Shader.h"
#include "../data/BatchMap.h"
#include "../data/RenderContext.h"
#include "../math_custom/GLAdapter.h"
class RenderHandler {
protected:
    Shader* shader;
public:
    RenderHandler(Shader* s) : shader(s) {}
    
    virtual void renderBatch(const RenderContext& context) {};
    
};

class ObjectRenderer : public RenderHandler {
private:
    ShaderBatchMap renderBatches;
 

public:
    ObjectRenderer(Shader* s) : RenderHandler(s) {}

    void renderBatch(const RenderContext& context)  override {
    
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::vector<Light*>& lightsList = context.scene->lightsList();
		Scene* scene = context.scene;
		Camera* camera = context.camera;



        updateInstanceData(scene);


        shader->Activate();
        shader->setMat4("view", camera->getViewMatrix());
        shader->setMat4("projection", camera->getProjectionMatrix());
        shader->setVec3("cameraPos", camera->getPosition());

        for (Light* light : lightsList) {
            shader->setVec4("lightColor", light->getColor(), 1.0f);
            Vector3 lightPos_engine = light->getWorldPosition();
            Vector3 lightPos_gl = GLAdapter::toGL(lightPos_engine);
            shader->setVec3("lightPos", lightPos_gl);
        }

        for (auto& [shader, materialMap] : renderBatches) {
     

            for (auto& [material, meshMap] : materialMap) {
                material->Bind(shader);

                for (auto& [mesh, batch] : meshMap) {
                    if (batch.instances.empty()) continue;

                    mesh->bind();
                    mesh->setupInstanceVBO(batch.instances.size());

                    glBindBuffer(GL_ARRAY_BUFFER, mesh->getInstanceVBO());
                    glBufferSubData(
                        GL_ARRAY_BUFFER,
                        0,
                        batch.instances.size() * sizeof(Mat4),
                        batch.instances.data()
                    );

                    glDrawElementsInstanced(
                        GL_TRIANGLES,
                        mesh->indexCount(),
                        GL_UNSIGNED_INT,
                        0,
                        batch.instances.size()
                    );
                }
            }
        }

       
     
    }

    void rebuildBatches(Scene* scene) {

        renderBatches.clear();

        std::vector<Object3D*>& objectList = scene->objectList();


        for (auto* obj : objectList) {
            for (auto& sub : obj->getModel()->getSubMeshes()) {
                if (!sub.mesh || !sub.material) continue;


                auto& materialMap = renderBatches[shader];
                auto& meshMap = materialMap[sub.material];
                auto& batch = meshMap[sub.mesh];

                batch.mesh = sub.mesh;

            }
        }
    }
    void updateInstanceData(Scene* scene) {
        std::vector<Object3D*>& objectList = scene->objectList();

       
        for (auto& [shader, materialMap] : renderBatches) {
            for (auto& [material, meshMap] : materialMap) {
                for (auto& [mesh, batch] : meshMap) {
                    batch.instances.clear();
                }
            }
        }


        for (auto* obj : objectList) {
            for (auto& sub : obj->getModel()->getSubMeshes()) {
                if (!sub.mesh || !sub.material) continue;

                Mat4 model = sub.localTransform *
                    GLAdapter::toGL(
                        Mat4::fromMat3(obj->getOrientation()) *
                        Mat4::scale(obj->getScale()) *
                        Mat4::translate(obj->getPosition())
                    );

                renderBatches[shader][sub.material][sub.mesh].instances.push_back(model);
            }
        }
    }
};

class CubeMapRenderer : public RenderHandler {

public:
    CubeMapRenderer(Shader* s) : RenderHandler(s) {}

     void renderBatch(const RenderContext& context) override {
		 shader->Activate();

		 Camera* camera = context.camera;
         CubeMap* skybox = context.scene->getSkybox();


		 glDepthFunc(GL_LEQUAL);  
         glDisable(GL_CULL_FACE);

         Mat4 projection = camera->getProjectionMatrix();
         Mat4 view = camera->getViewMatrix();

         view.data[12] = 0.0f; // remove x translation
         view.data[13] = 0.0f; // remove y translation
         view.data[14] = 0.0f;

         shader->setInt("skybox", 0);
         shader->setMat4("projection", projection);
         shader->setMat4("view", view);
		

		 skybox->bind();
		 skybox->bindTexture();

         glDrawArrays(GL_TRIANGLES, 0, 36);
          
		 glDepthFunc(GL_LESS);
     
     };

};