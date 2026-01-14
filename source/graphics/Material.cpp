#include "Material.h"
#include "Texture.h"
#include <iostream>
#include "../render/RenderHelpers/Shader.h"
void Material::SetTexture(int slot, Texture* texture) {
    if (slot >= textures.size()) {
        textures.resize(slot + 1, nullptr);
    }
    textures[slot] = texture;

}

Texture* Material::GetTexture(int slot) {
    if (slot >= 0 && slot < textures.size()) {
        return textures[slot];
    }
    return nullptr;
}

void Material::Bind(Shader* shader) {
    for (int i = 0; i < textures.size(); i++) {
        if (!textures[i]) continue;
        // Activate the texture slot
        textures[i]->Bind(i);

        // Assuming your shader uniform names are tex0, tex1, tex2, etc.
        std::string uniformName = "tex" + std::to_string(i);
        GLint texUni = glGetUniformLocation(shader->ID, uniformName.c_str());
        shader->Activate();
        glUniform1i(texUni, i);
    }
}
