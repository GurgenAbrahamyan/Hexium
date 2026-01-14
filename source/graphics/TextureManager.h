#pragma once
#include <iostream>
#include <unordered_map>

#include "Texture.h"

class TextureManager {

private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> textures;

	
public:
	TextureManager();

	Texture* getTexture(std::string path, TextureUsage usage);

};