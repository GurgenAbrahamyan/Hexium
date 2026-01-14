#pragma once
#include <cmath>
#include "Vector3.h"
#include "../math_custom/Mat3.h"
class Mat4 {
public:
    float data[16];

    Mat4() {
        for (int i = 0; i < 16; i++) data[i] = 0.0f;
        data[0] = data[5] = data[10] = data[15] = 1.0f;
    }


    static Mat4 multiply(const Mat4& a, const Mat4& b) {
        Mat4 r;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                r.data[col + row * 4] = 0;
                for (int k = 0; k < 4; k++)
                    r.data[col + row * 4] += a.data[k + row * 4] * b.data[col + k * 4];
            }
        }
        return r;
    }

    static Mat4 translate(const Vector3& v) {
        Mat4 m;
        m.data[12] = v.x;
        m.data[13] = v.y;
        m.data[14] = v.z;
        return m;
    }


    static Mat4 scale(const Vector3& v) {
        Mat4 m;
        m.data[0] = v.x;
        m.data[5] = v.y;
        m.data[10] = v.z;
        return m;
    }


    static Mat4 rotateX(float deg) {
        float rad = deg * 3.14159265f / 180.0f;
        Mat4 m;
        m.data[5] = cos(rad);
        m.data[6] = sin(rad);
        m.data[9] = -sin(rad);
        m.data[10] = cos(rad);
        return m;
    }

    static Mat4 rotateY(float deg) {
        float rad = deg * 3.14159265f / 180.0f;
        Mat4 m;
        m.data[0] = cos(rad);
        m.data[2] = -sin(rad);
        m.data[8] = sin(rad);
        m.data[10] = cos(rad);
        return m;
    }

    static Mat4 rotateZ(float deg) {
        float rad = deg * 3.14159265f / 180.0f;
        Mat4 m;
        m.data[0] = cos(rad);
        m.data[1] = sin(rad);
        m.data[4] = -sin(rad);
        m.data[5] = cos(rad);
        return m;
    }


    static Mat4 perspective(float fovDeg, float aspect, float near, float far) {
        float f = 1.0f / tan(fovDeg * 3.14159265f / 180.0f / 2.0f);
        Mat4 m;
        m.data[0] = f / aspect;
        m.data[5] = f;
        m.data[10] = (far + near) / (near - far);
        m.data[11] = -1.0f;
        m.data[14] = (2.0f * far * near) / (near - far);
        m.data[15] = 0.0f;
        return m;
    }

    static Mat4 ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
        Mat4 m;

        m.data[0] = 2.0f / (right - left);
        m.data[5] = 2.0f / (top - bottom);
        m.data[10] = -2.0f / (farPlane - nearPlane);

        m.data[12] = -(right + left) / (right - left);
        m.data[13] = -(top + bottom) / (top - bottom);
        m.data[14] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        m.data[15] = 1.0f;

        return m;
    }


    static Mat4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
        Vector3 f = (center.subtract(eye)).normalize();
        Vector3 s = f.cross(up).normalize();
        Vector3 u = s.cross(f);

        Mat4 m;
        m.data[0] = s.x;  m.data[1] = u.x;  m.data[2] = -f.x; m.data[3] = 0.0f;
        m.data[4] = s.y;  m.data[5] = u.y;  m.data[6] = -f.y; m.data[7] = 0.0f;
        m.data[8] = s.z;  m.data[9] = u.z;  m.data[10] = -f.z; m.data[11] = 0.0f;
        m.data[12] = -s.dot(eye); m.data[13] = -u.dot(eye); m.data[14] = f.dot(eye); m.data[15] = 1.0f;
        return m;
    }
    static Mat4 fromMat3(const Matrix3& mat3) {
        Mat4 m;  
        m.data[0] = mat3.m[0][0];
        m.data[1] = mat3.m[0][1];
        m.data[2] = mat3.m[0][2];

        m.data[4] = mat3.m[1][0];
        m.data[5] = mat3.m[1][1];
        m.data[6] = mat3.m[1][2];

        m.data[8] = mat3.m[2][0];
        m.data[9] = mat3.m[2][1];
        m.data[10] = mat3.m[2][2];

       
        return m;
    }


    Vector3 multiplyVec(const Vector3& v, float w = 1.0f) const {
        Vector3 r(0, 0, 0);
        r.x = data[0] * v.x + data[4] * v.y + data[8] * v.z + data[12] * w;
        r.y = data[1] * v.x + data[5] * v.y + data[9] * v.z + data[13] * w;
        r.z = data[2] * v.x + data[6] * v.y + data[10] * v.z + data[14] * w;
        float w_out = data[3] * v.x + data[7] * v.y + data[11] * v.z + data[15] * w;
        if (w_out != 0) {
            r.x /= w_out; r.y /= w_out; r.z /= w_out;
        }
        return r;
    }

    Mat4 operator*(const Mat4& other) const {
        Mat4 result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                float sum = 0.0f;
                for (int k = 0; k < 4; ++k)
                    sum += data[k + i * 4] * other.data[j + k * 4];
                result.data[j + i * 4] = sum;
            }
        return result;
    }

    float* getData() {
        return data;
    }

    static float radians(float deg) {
        return deg * 3.14159265f / 180.0f;
    }
};