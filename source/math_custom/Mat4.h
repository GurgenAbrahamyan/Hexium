#pragma once
#include <cmath>
#include "Mat3.h"
#include "Vector3.h"
#include "../math_custom/Quat.h"

class Mat4 {
public:
    float data[16]; // row-major: row 0 = data[0..3], row1 = data[4..7], etc.

    // ---------------- Constructor ----------------
    Mat4() {
        for (int i = 0; i < 16; i++) data[i] = 0.0f;
        data[0] = data[5] = data[10] = data[15] = 1.0f;
    }

    // ---------------- Multiplication ----------------
    Mat4 operator*(const Mat4& other) const {
        Mat4 r;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                r.data[row * 4 + col] = 0;
                for (int k = 0; k < 4; k++)
                    r.data[row * 4 + col] += data[row * 4 + k] * other.data[k * 4 + col];
            }
        }
        return r;
    }

    // ---------------- Translation ----------------
    static Mat4 translate(const Vector3& v) {
        Mat4 m;
        m.data[12] = v.x;
        m.data[13] = v.y;
        m.data[14] = v.z;
        return m;
    }

    // ---------------- Scaling ----------------
    static Mat4 scale(const Vector3& v) {
        Mat4 m;
        m.data[0] = v.x;
        m.data[5] = v.y;
        m.data[10] = v.z;
        return m;
    }

    // ---------------- Rotation ----------------
    static Mat4 rotateX(float deg) {
        float rad = radians(deg);
        float c = cos(rad);
        float s = sin(rad);
        Mat4 m; // identity
        m.data[5] = c;   m.data[6] = s;    // m[1][1], m[1][2]
        m.data[9] = -s;   m.data[10] = c;    // m[2][1], m[2][2]
        return m;
    }

    static Mat4 rotateY(float deg) {
        float rad = radians(deg);
        float c = cos(rad);
        float s = sin(rad);
        Mat4 m;
        m.data[0] = c;   m.data[2] = -s;    // m[0][0], m[0][2]
        m.data[8] = s;   m.data[10] = c;    // m[2][0], m[2][2]
        return m;
    }

    static Mat4 rotateZ(float deg) {
        float rad = radians(deg);
        float c = cos(rad);
        float s = sin(rad);
        Mat4 m;
        m.data[0] = c;   m.data[1] = s;     // m[0][0], m[0][1]
        m.data[4] = -s;   m.data[5] = c;     // m[1][0], m[1][1]
        return m;
    }

    // ---------------- Perspective ----------------
    static Mat4 perspective(float fovDeg, float aspect, float near, float far) {
        float f = 1.0f / tan(radians(fovDeg) / 2.0f);
        Mat4 m;
        m.data[0] = f / aspect;
        m.data[5] = f;
        m.data[10] = (far + near) / (near - far);
        m.data[11] = -1.0f;
        m.data[14] = (2.0f * far * near) / (near - far);
        m.data[15] = 0.0f;
        return m;
    }

    // ---------------- Orthographic ----------------
    static Mat4 ortho(float left, float right, float bottom, float top, float near, float far) {
        Mat4 m;
        m.data[0] = 2.0f / (right - left);
        m.data[5] = 2.0f / (top - bottom);
        m.data[10] = -2.0f / (far - near);
        m.data[12] = -(right + left) / (right - left);
        m.data[13] = -(top + bottom) / (top - bottom);
        m.data[14] = -(far + near) / (far - near);
        m.data[15] = 1.0f;
        return m;
    }

    static Mat4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
        Vector3 f = (center.subtract(eye)).normalize();           // forward
        Vector3 r = f.cross(up).normalize();                      // right
        Vector3 u = r.cross(f).normalize();                       // actual up (corrected)

        Mat4 m;  // starts as identity

        // Basis as **columns** (but since row-major storage, column 0 = data[0,4,8,12])
        m.data[0] = r.x;   m.data[1] = u.x;   m.data[2] = -f.x;   m.data[3] = 0.0f;
        m.data[4] = r.y;   m.data[5] = u.y;   m.data[6] = -f.y;   m.data[7] = 0.0f;
        m.data[8] = r.z;   m.data[9] = u.z;   m.data[10] = -f.z;   m.data[11] = 0.0f;

        // Translation in **last column** (but row-major: data[12,13,14] = -dot with basis)
        m.data[12] = -r.dot(eye);
        m.data[13] = -u.dot(eye);
        m.data[14] = f.dot(eye);   // note + here because we use -f as forward direction
        m.data[15] = 1.0f;

        return m;
    }

    static Mat4 fromQuat(const Quat& q) {
        Mat4 m;

        float xx = q.x * q.x;
        float yy = q.y * q.y;
        float zz = q.z * q.z;
        float xy = q.x * q.y;
        float xz = q.x * q.z;
        float yz = q.y * q.z;
        float wx = q.w * q.x;
        float wy = q.w * q.y;
        float wz = q.w * q.z;


        m.data[0] = 1.0f - 2.0f * (yy + zz);
        m.data[1] = 2.0f * (xy + wz);
        m.data[2] = 2.0f * (xz - wy);
        m.data[3] = 0.0f;

        m.data[4] = 2.0f * (xy - wz);
        m.data[5] = 1.0f - 2.0f * (xx + zz);
        m.data[6] = 2.0f * (yz + wx);
        m.data[7] = 0.0f;

        m.data[8] = 2.0f * (xz + wy);
        m.data[9] = 2.0f * (yz - wx);
        m.data[10] = 1.0f - 2.0f * (xx + yy);
        m.data[11] = 0.0f;

        m.data[12] = 0.0f;
        m.data[13] = 0.0f;
        m.data[14] = 0.0f;
        m.data[15] = 1.0f;

        return m;
    }

    Mat4 transpose() const {
        Mat4 result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result.data[row * 4 + col] = data[col * 4 + row];
            }
        }
        return result;
    }


    // ---------------- Vector multiply ----------------
    Vector3 multiplyVec(const Vector3& v, float w = 1.0f) const {
        Vector3 r;
        r.x = data[0] * v.x + data[1] * v.y + data[2] * v.z + data[3] * w;
        r.y = data[4] * v.x + data[5] * v.y + data[6] * v.z + data[7] * w;
        r.z = data[8] * v.x + data[9] * v.y + data[10] * v.z + data[11] * w;
        float w_out = data[12] * v.x + data[13] * v.y + data[14] * v.z + data[15] * w;
        if (w_out != 0.0f) { r.x /= w_out; r.y /= w_out; r.z /= w_out; }
        return r;
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




    float* getData() { return data; }

    static float radians(float deg) { return deg * 3.14159265359f / 180.0f; }
};
