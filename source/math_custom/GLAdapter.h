#pragma once
#include "Vector3.h"
#include "Mat4.h"
#include "Quat.h"

namespace GLAdapter {

    // Basis matrix to convert from Engine space (X right, Y forward, Z up)
    // to OpenGL space (X right, Y up, Z backward)
    inline Mat4 basisMatrix() {
        Mat4 B;
        // Engine -> OpenGL mapping:
        // Engine X (right)   -> OpenGL X (right)
        // Engine Y (forward) -> OpenGL -Z (backward becomes forward in OpenGL)
        // Engine Z (up)      -> OpenGL Y (up)

        B.data[0] = 1;   B.data[1] = 0;   B.data[2] = 0;   B.data[3] = 0;   // X stays X
        B.data[4] = 0;   B.data[5] = 0;   B.data[6] = 1;   B.data[7] = 0;   // Z -> Y
        B.data[8] = 0;   B.data[9] = -1;  B.data[10] = 0;  B.data[11] = 0;  // -Y -> Z
        B.data[12] = 0;  B.data[13] = 0;  B.data[14] = 0;  B.data[15] = 1;

        return B;
    }

    // Inverse: OpenGL -> Engine
    inline Mat4 inverseBasisMatrix() {
        Mat4 Binv;
        // OpenGL -> Engine mapping:
        // OpenGL X (right)    -> Engine X (right)
        // OpenGL Y (up)       -> Engine Z (up)
        // OpenGL Z (backward) -> Engine -Y (forward)

        Binv.data[0] = 1;   Binv.data[1] = 0;   Binv.data[2] = 0;   Binv.data[3] = 0;   // X stays X
        Binv.data[4] = 0;   Binv.data[5] = 0;   Binv.data[6] = 1;  Binv.data[7] = 0;   // -Z -> Y
        Binv.data[8] = 0;   Binv.data[9] = 1;   Binv.data[10] = 0;  Binv.data[11] = 0;  // Y -> Z
        Binv.data[12] = 0;  Binv.data[13] = 0;  Binv.data[14] = 0;  Binv.data[15] = 1;

        return Binv;
    }

    // Convert a position/direction vector from Engine to OpenGL
    inline Vector3 toGL(const Vector3& v) {
        // Engine (X, Y, Z) -> OpenGL (X, -Z, Y)
        return Vector3(
            v.x,   // X right -> X right
            v.z,   // Z up -> Y up
            -v.y   // Y forward -> -Z backward
        );
    }

    // Convert a position/direction vector from OpenGL to Engine
    inline Vector3 fromGL(const Vector3& v) {
        // OpenGL (X, Y, Z) -> Engine (X, -Z, Y)
        return Vector3(
            v.x,   // X right -> X right
            -v.z,  // Z backward -> Y forward
            v.y    // Y up -> Z up
        );
    }

    // Convert a transformation matrix from Engine to OpenGL
    inline Mat4 toGL(const Mat4& m) {
        static Mat4 B = basisMatrix();
        static Mat4 Binv = inverseBasisMatrix();
        return B * m * Binv;
    }

    // Convert a quaternion from Engine to OpenGL
    inline Quat toGL(const Quat& q) {
        // Rotation of -90 degrees around X axis to convert coordinate systems
        // This rotates Y->Z and Z->-Y
        static Quat Qconv = Quat::FromAxisAngle(1.0f, 0.0f, 0.0f, -1.57079632679f); // -90 degrees
        static Quat QconvInv = Quat(-Qconv.x, -Qconv.y, -Qconv.z, Qconv.w); // conjugate

        return Qconv * q * QconvInv;
    }
}