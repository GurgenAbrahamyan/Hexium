#pragma once

#include "EnTT/entt.hpp"
struct RenderContext {
        
    entt::registry* registry = nullptr;                
    float ambientIntensity = 1.0f; 
    float deltaTime = 0.0f;       
    
};