#pragma once
#include "Vector3.h"
#include <array>
#include <cmath>
#include <iostream>

class Matrix3 {
public:

    std::array<std::array<float, 3>, 3> m;


    Matrix3() {
        setZero();
    }

    Matrix3(float diag) {
        setZero();
        m[0][0] = m[1][1] = m[2][2] = diag;
    }


    void setZero() {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                m[i][j] = 0.0f;
    }


    void setIdentity() {
        setZero();
        m[0][0] = m[1][1] = m[2][2] = 1.0f;
    }


    Matrix3 operator+(const Matrix3& other) const {
        Matrix3 result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.m[i][j] = m[i][j] + other.m[i][j];
        return result;
    }


    Matrix3 operator*(const Matrix3& other) const {
        Matrix3 result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                result.m[i][j] = 0;
                for (int k = 0; k < 3; ++k)
                    result.m[i][j] += m[i][k] * other.m[k][j];
            }
        return result;
    }



    Matrix3 operator*(float scalar) const {
        Matrix3 result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.m[i][j] = m[i][j] * scalar;
        return result;
    }

    Vector3 operator*(const Vector3& v) const {
        return Vector3(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
        );
    }

    Matrix3 makeOmegaMatrix(const Vector3& w)
    {
        Matrix3 m;
        m.set(
            0.0f, -w.z, w.y,
            w.z, 0.0f, -w.x,
            -w.y, w.x, 0.0f
        );
        return m;
    }

    Matrix3 transpose() const {
        Matrix3 result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.m[i][j] = m[j][i];
        return result;
    }


    void print() const {
        for (int i = 0; i < 3; ++i) {
            std::cout << "[ ";
            for (int j = 0; j < 3; ++j)
                std::cout << m[i][j] << " ";
            std::cout << "]\n";
        }
    }
    void set(float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22) {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
    }

    Vector3 getColumn(int col) const {
        return Vector3(m[0][col], m[1][col], m[2][col]);
    }

    void setColumn(int col, const Vector3& v) {
        m[0][col] = v.x;
        m[1][col] = v.y;
        m[2][col] = v.z;
    }

    void orthonormalize() {
        Vector3 x = getColumn(0);
        Vector3 y = getColumn(1);


        float lenX = x.length();
        if (lenX < 1e-6f) {
            setIdentity();
            return;
        }
        x = x.multiply(1.0f / lenX);


        float dotXY = x.dot(y);
        y = y.subtract(x.multiply(dotXY));

        float lenY = y.length();
        if (lenY < 1e-6f) {

            y = x.perpendicular();
            y = y.multiply(1.0f / y.length());
        }
        else {
            y = y.multiply(1.0f / lenY);
        }


        Vector3 z = x.cross(y);


        float lenZ = z.length();
        if (lenZ > 1e-6f) {
            z = z.multiply(1.0f / lenZ);
        }
        else {

            z = Vector3(0, 0, 1);
        }

        setColumn(0, x);
        setColumn(1, y);
        setColumn(2, z);
    }

    static Matrix3 rotationAxisAngle(const Vector3& axis, float angle) {
        Matrix3 R;

        
        float c = cosf(angle);
        float s = sinf(angle);
        float t = 1.0f - c;              
        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

        
        R.m[0][0] = t * x * x + c;
        R.m[0][1] = t * x * y - s * z;
        R.m[0][2] = t * x * z + s * y;

        R.m[1][0] = t * y * x + s * z;
        R.m[1][1] = t * y * y + c;
        R.m[1][2] = t * y * z - s * x;

        R.m[2][0] = t * z * x - s * y;
        R.m[2][1] = t * z * y + s * x;
        R.m[2][2] = t * z * z + c;

        return R;
    }
};