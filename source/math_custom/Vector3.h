#pragma once

#include <cmath>

class Vector3 {
public:
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    Vector3 add(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 add(float& x1, float& y1, float& z1) const {
        return Vector3(x + x1, y + y1, z + z1);
    }

    Vector3 subtract(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3 multiply(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

   

    Vector3 operator*(const Vector3& other) const {
        return Vector3(
            x * other.x,
            y * other.y,
            z * other.z
        );
    }

    Vector3 operator/(float scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
	}

    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 normalize() const {
        float len = length();
        return Vector3(x / len, y / len, z / len);
    }

    Vector3 perpendicular() const {
        float ax = std::abs(x);
        float ay = std::abs(y);
        float az = std::abs(z);


        if (ax <= ay && ax <= az) {
          
            return Vector3(0.0f, -z, y);
        }
        else if (ay <= ax && ay <= az) {
           
            return Vector3(-z, 0.0f, x);
        }
        else {
            
            return Vector3(-y, x, 0.0f);
        }
    }
    float lengthSquared() const { return x * x + y * y + z * z; }

    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }

    void setX(float newx) { x = newx; }
    void setY(float newy) { y = newy; }
    void setZ(float newz) { z = newz; }
};
