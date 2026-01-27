#pragma once

#include <cmath>

struct Quat
{
    float x;
    float y;
    float z;
    float w;

    Quat()
        : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {
    }

    Quat(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w) {
    }

    void Normalize()
    {
        float len = std::sqrt(x * x + y * y + z * z + w * w);
        if (len == 0.0f) return;

        float inv = 1.0f / len;
        x *= inv;
        y *= inv;
        z *= inv;
        w *= inv;
    }

    
    Quat operator*(const Quat& r) const
    {
        return Quat(
            w * r.w - x * r.x - y * r.y - z * r.z,
            w * r.x + x * r.w + y * r.z - z * r.y,
            w * r.y - x * r.z + y * r.w + z * r.x,
            w * r.z + x * r.y - y * r.x + z * r.w
        );
    }
    Quat operator+(const Quat& r) const
    {
        return Quat(
            x + r.x,
            y + r.y,
            z + r.z,
            w + r.w
        );
    }


    Quat conjugate()  {
        return Quat(-x, -y, -z, w);
    }

   
    static Quat FromAxisAngle(float angleRad, float ax, float ay, float az)
    {
        float half = angleRad * 0.5f;
        float s = std::sin(half);

        return Quat(
            ax * s,
            ay * s,
            az * s, 
            std::cos(half)
        );
    }

    
     Vector3 rotate(const Vector3& v)  const
    {
        
        Vector3 qVec(x, y, z);
        Vector3 uv = qVec.cross(v);
        Vector3 uuv = qVec.cross(uv);

        return v.add(uv.multiply(2.0f * w)).add(uuv.multiply(2.0f));
    }
};
