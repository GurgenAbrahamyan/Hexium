#pragma once
#include <vector>
#include <cstdint>
#include <string>


class Texture;
class Shader;

class Material {
public:
    Material() = default;

    
    void SetTexture(int slot, Texture* texture);
    Texture* GetTexture(int slot) const;

    // unused 
    float metallic = 0.0f;
    float roughness = 1.0f;
    float ao = 1.0f;

  
    void Bind(Shader* shader) const;

	int getID() const { return ID; }
	void setID(int id) { ID = id; }

private:
    std::vector<Texture*> textures;
    int ID = -1;
};
