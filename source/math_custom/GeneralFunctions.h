#pragma once

#pragma once
#include <cmath>

class GeneralFunctions {
public:
    
    static float Derivative(float y2, float y1, float t2, float t1) {
        return (y2 - y1) / (t2 - t1);
    }

  
    static float EulerStep(float y, float dydt, float dt) {
        return y + dydt * dt;
    }

  
    template<typename Func>
    static float NumericalDerivative(Func f, float x, float h = 1e-5f) {
        return (f(x + h) - f(x - h)) / (2 * h);
    }

    static float Clamp(float value, float minVal, float maxVal) {
        if (value < minVal) return minVal;
        if (value > maxVal) return maxVal;
        return value;
    }
    
   
    static float Lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

   
    static void EulerStepVec(float* y, float* dydt, float dt, int n) {
        for (int i = 0; i < n; i++)
            y[i] += dydt[i] * dt;
    }
};
