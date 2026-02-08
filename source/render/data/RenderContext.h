#pragma once
#include "../Camera.h"
#include "../../core/Scene.h"
struct RenderContext {
    Camera* camera;              // View & projection matrices
    Scene* scene;                // Lights, sky info, etc.
    float ambientIntensity = 1.0f; // optional global light multiplier
    float deltaTime = 0.0f;       // for animated effects
    // add more fields later: shadows, fog, time, etc.
};