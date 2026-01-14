#pragma once

#include <array>
#include <cstdint>
#include <vector>
class Texture;   
class Shader;





class Material {
public:
    Material() = default;

   
    void SetTexture(int slot, Texture* texture);
    Texture* GetTexture(int slot);

    
    float metallic = 0.0f;
    float roughness = 1.0f;
    float ao = 1.0f;

    void Bind(Shader* shader);

private:
    std::vector <Texture*> textures;
};
