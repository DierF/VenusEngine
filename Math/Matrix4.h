#pragma once

#include "Math/Math.h"
#include "Math/Matrix3.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace VenusEngine
{
    /** Class encapsulating a standard 4x4 homogeneous matrix.
    @remarks
    CHAOS uses column vectors when applying matrix multiplications,
    This means a vector is represented as a single column, 4-row
    matrix. This has the effect that the transformations implemented
    by the matrices happens right-to-left e.g. if vector V is to be
    transformed by M1 then M2 then M3, the calculation would be
    M3 * M2 * M1 * V. The order that matrices are concatenated is
    vital since matrix multiplication is not commutative, i.e. you
    can get a different result if you concatenate in the wrong order.
    @par
    The use of column vectors and right-to-left ordering is the
    standard in most mathematical texts, and is the same as used in
    OpenGL. It is, however, the opposite of Direct3D, which has
    inexplicably chosen to differ from the accepted standard and uses
    row vectors and left-to-right matrix multiplication.
    @par
    CHAOS deals with the differences between D3D and OpenGL etc.
    internally when operating through different render systems. CHAOS
    users only need to conform to standard maths conventions, i.e.
    right-to-left matrix multiplication, (CHAOS transposes matrices it
    passes to D3D to compensate).
    @par
    The generic form M * V which shows the layout of the matrix
    entries is shown below:
    <pre>
    [ m[0][0]  m[0][1]  m[0][2]  m[0][3] ]   {x}
    | m[1][0]  m[1][1]  m[1][2]  m[1][3] | * {y}
    | m[2][0]  m[2][1]  m[2][2]  m[2][3] |   {z}
    [ m[3][0]  m[3][1]  m[3][2]  m[3][3] ]   {1}
    </pre>
    */
    class Mat4_
    {
    public:
        Mat4_()  {}
        float v0 { 1.0f };
        float v1 { 0 };
        float v2 { 0 };
        float v3 { 0 };
        float v4 { 0 };
        float v5 { 1.0f };
        float v6 { 0 };
        float v7 { 0 };
        float v8 { 0 };
        float v9 { 0 };
        float v10{ 1.0f };
        float v11{ 0 };
        float v12{ 0 };
        float v13{ 0 };
        float v14{ 0 };
        float v15{ 1.0f };
    };

    class Mat4
    {
    public:
        /// The matrix entries, indexed by [row][col]
        float m_mat[4][4];

    public:
        /** Default constructor.
        @note
        It does <b>NOT</b> initialize the matrix for efficiency.
        */
        Mat4(Mat4_ const& mat)
        {
            m_mat[0][0] = mat.v0;
            m_mat[0][1] = mat.v1;
            m_mat[0][2] = mat.v2;
            m_mat[0][3] = mat.v3;
            m_mat[1][0] = mat.v4;
            m_mat[1][1] = mat.v5;
            m_mat[1][2] = mat.v6;
            m_mat[1][3] = mat.v7;
            m_mat[2][0] = mat.v8;
            m_mat[2][1] = mat.v9;
            m_mat[2][2] = mat.v10;
            m_mat[2][3] = mat.v11;
            m_mat[3][0] = mat.v12;
            m_mat[3][1] = mat.v13;
            m_mat[3][2] = mat.v14;
            m_mat[3][3] = mat.v15;
        }

        Mat4_ toMat4_()
        {
            Mat4_ res;

            res.v0 = m_mat[0][0];
            res.v1 = m_mat[0][1];
            res.v2 = m_mat[0][2];
            res.v3 = m_mat[0][3];
            res.v4 = m_mat[1][0];
            res.v5 = m_mat[1][1];
            res.v6 = m_mat[1][2];
            res.v7 = m_mat[1][3];
            res.v8 = m_mat[2][0];
            res.v9 = m_mat[2][1];
            res.v10 = m_mat[2][2];
            res.v11 = m_mat[2][3];
            res.v12 = m_mat[3][0];
            res.v13 = m_mat[3][1];
            res.v14 = m_mat[3][2];
            res.v15 = m_mat[3][3];
            return res;
        }

        Mat4() { operator=(IDENTITY); }

        Mat4(float const(&float_array)[16])
        {
            m_mat[0][0] = float_array[0];
            m_mat[0][1] = float_array[1];
            m_mat[0][2] = float_array[2];
            m_mat[0][3] = float_array[3];
            m_mat[1][0] = float_array[4];
            m_mat[1][1] = float_array[5];
            m_mat[1][2] = float_array[6];
            m_mat[1][3] = float_array[7];
            m_mat[2][0] = float_array[8];
            m_mat[2][1] = float_array[9];
            m_mat[2][2] = float_array[10];
            m_mat[2][3] = float_array[11];
            m_mat[3][0] = float_array[12];
            m_mat[3][1] = float_array[13];
            m_mat[3][2] = float_array[14];
            m_mat[3][3] = float_array[15];
        }

        Mat4(float m00,
            float m01,
            float m02,
            float m03,
            float m10,
            float m11,
            float m12,
            float m13,
            float m20,
            float m21,
            float m22,
            float m23,
            float m30,
            float m31,
            float m32,
            float m33)
        {
            m_mat[0][0] = m00;
            m_mat[0][1] = m01;
            m_mat[0][2] = m02;
            m_mat[0][3] = m03;
            m_mat[1][0] = m10;
            m_mat[1][1] = m11;
            m_mat[1][2] = m12;
            m_mat[1][3] = m13;
            m_mat[2][0] = m20;
            m_mat[2][1] = m21;
            m_mat[2][2] = m22;
            m_mat[2][3] = m23;
            m_mat[3][0] = m30;
            m_mat[3][1] = m31;
            m_mat[3][2] = m32;
            m_mat[3][3] = m33;
        }

        Mat4(Vec4 const& row0, Vec4 const& row1, Vec4 const& row2, Vec4 const& row3)
        {
            m_mat[0][0] = row0.x;
            m_mat[0][1] = row0.y;
            m_mat[0][2] = row0.z;
            m_mat[0][3] = row0.w;
            m_mat[1][0] = row1.x;
            m_mat[1][1] = row1.y;
            m_mat[1][2] = row1.z;
            m_mat[1][3] = row1.w;
            m_mat[2][0] = row2.x;
            m_mat[2][1] = row2.y;
            m_mat[2][2] = row2.z;
            m_mat[2][3] = row2.w;
            m_mat[3][0] = row3.x;
            m_mat[3][1] = row3.y;
            m_mat[3][2] = row3.z;
            m_mat[3][3] = row3.w;
        }

        Mat4(Vec3 const& position, Vec3 const& scale, Quaternion const& rotation)
        {
            makeTransform(position, scale, rotation);
        }

        void fromData(float const(&float_array)[16])
        {
            m_mat[0][0] = float_array[0];
            m_mat[0][1] = float_array[1];
            m_mat[0][2] = float_array[2];
            m_mat[0][3] = float_array[3];
            m_mat[1][0] = float_array[4];
            m_mat[1][1] = float_array[5];
            m_mat[1][2] = float_array[6];
            m_mat[1][3] = float_array[7];
            m_mat[2][0] = float_array[8];
            m_mat[2][1] = float_array[9];
            m_mat[2][2] = float_array[10];
            m_mat[2][3] = float_array[11];
            m_mat[3][0] = float_array[12];
            m_mat[3][1] = float_array[13];
            m_mat[3][2] = float_array[14];
            m_mat[3][3] = float_array[15];
        }

        void toData(float(&float_array)[16]) const
        {
            float_array[0] = m_mat[0][0];
            float_array[1] = m_mat[0][1];
            float_array[2] = m_mat[0][2];
            float_array[3] = m_mat[0][3];
            float_array[4] = m_mat[1][0];
            float_array[5] = m_mat[1][1];
            float_array[6] = m_mat[1][2];
            float_array[7] = m_mat[1][3];
            float_array[8] = m_mat[2][0];
            float_array[9] = m_mat[2][1];
            float_array[10] = m_mat[2][2];
            float_array[11] = m_mat[2][3];
            float_array[12] = m_mat[3][0];
            float_array[13] = m_mat[3][1];
            float_array[14] = m_mat[3][2];
            float_array[15] = m_mat[3][3];
        }

        /** Creates a standard 4x4 transformation matrix with a zero translation part from a rotation/scaling 3x3
         * matrix.
         */
        void setMat3(Mat3 const& mat3)
        {
            m_mat[0][0] = mat3.m_mat[0][0];
            m_mat[0][1] = mat3.m_mat[0][1];
            m_mat[0][2] = mat3.m_mat[0][2];
            m_mat[0][3] = 0;
            m_mat[1][0] = mat3.m_mat[1][0];
            m_mat[1][1] = mat3.m_mat[1][1];
            m_mat[1][2] = mat3.m_mat[1][2];
            m_mat[1][3] = 0;
            m_mat[2][0] = mat3.m_mat[2][0];
            m_mat[2][1] = mat3.m_mat[2][1];
            m_mat[2][2] = mat3.m_mat[2][2];
            m_mat[2][3] = 0;
            m_mat[3][0] = 0;
            m_mat[3][1] = 0;
            m_mat[3][2] = 0;
            m_mat[3][3] = 1;
        }

        /** Creates a standard 4x4 transformation matrix with a zero translation part from a rotation/scaling
         * Quaternion.
         */
        Mat4(Quaternion const& rot)
        {
            Mat3 m3x3;
            rot.toRotationMatrix(m3x3);
            operator=(IDENTITY);
            setMat3(m3x3);
        }

        float* operator[](size_t row_index)
        {
            assert(row_index < 4);
            return m_mat[row_index];
        }

        float const* operator[](size_t row_index) const
        {
            assert(row_index < 4);
            return m_mat[row_index];
        }

        Mat4 concatenate(Mat4 const& m2) const
        {
            Mat4 r;
            r.m_mat[0][0] = m_mat[0][0] * m2.m_mat[0][0] + m_mat[0][1] * m2.m_mat[1][0] + m_mat[0][2] * m2.m_mat[2][0] +
                m_mat[0][3] * m2.m_mat[3][0];
            r.m_mat[0][1] = m_mat[0][0] * m2.m_mat[0][1] + m_mat[0][1] * m2.m_mat[1][1] + m_mat[0][2] * m2.m_mat[2][1] +
                m_mat[0][3] * m2.m_mat[3][1];
            r.m_mat[0][2] = m_mat[0][0] * m2.m_mat[0][2] + m_mat[0][1] * m2.m_mat[1][2] + m_mat[0][2] * m2.m_mat[2][2] +
                m_mat[0][3] * m2.m_mat[3][2];
            r.m_mat[0][3] = m_mat[0][0] * m2.m_mat[0][3] + m_mat[0][1] * m2.m_mat[1][3] + m_mat[0][2] * m2.m_mat[2][3] +
                m_mat[0][3] * m2.m_mat[3][3];

            r.m_mat[1][0] = m_mat[1][0] * m2.m_mat[0][0] + m_mat[1][1] * m2.m_mat[1][0] + m_mat[1][2] * m2.m_mat[2][0] +
                m_mat[1][3] * m2.m_mat[3][0];
            r.m_mat[1][1] = m_mat[1][0] * m2.m_mat[0][1] + m_mat[1][1] * m2.m_mat[1][1] + m_mat[1][2] * m2.m_mat[2][1] +
                m_mat[1][3] * m2.m_mat[3][1];
            r.m_mat[1][2] = m_mat[1][0] * m2.m_mat[0][2] + m_mat[1][1] * m2.m_mat[1][2] + m_mat[1][2] * m2.m_mat[2][2] +
                m_mat[1][3] * m2.m_mat[3][2];
            r.m_mat[1][3] = m_mat[1][0] * m2.m_mat[0][3] + m_mat[1][1] * m2.m_mat[1][3] + m_mat[1][2] * m2.m_mat[2][3] +
                m_mat[1][3] * m2.m_mat[3][3];

            r.m_mat[2][0] = m_mat[2][0] * m2.m_mat[0][0] + m_mat[2][1] * m2.m_mat[1][0] + m_mat[2][2] * m2.m_mat[2][0] +
                m_mat[2][3] * m2.m_mat[3][0];
            r.m_mat[2][1] = m_mat[2][0] * m2.m_mat[0][1] + m_mat[2][1] * m2.m_mat[1][1] + m_mat[2][2] * m2.m_mat[2][1] +
                m_mat[2][3] * m2.m_mat[3][1];
            r.m_mat[2][2] = m_mat[2][0] * m2.m_mat[0][2] + m_mat[2][1] * m2.m_mat[1][2] + m_mat[2][2] * m2.m_mat[2][2] +
                m_mat[2][3] * m2.m_mat[3][2];
            r.m_mat[2][3] = m_mat[2][0] * m2.m_mat[0][3] + m_mat[2][1] * m2.m_mat[1][3] + m_mat[2][2] * m2.m_mat[2][3] +
                m_mat[2][3] * m2.m_mat[3][3];

            r.m_mat[3][0] = m_mat[3][0] * m2.m_mat[0][0] + m_mat[3][1] * m2.m_mat[1][0] + m_mat[3][2] * m2.m_mat[2][0] +
                m_mat[3][3] * m2.m_mat[3][0];
            r.m_mat[3][1] = m_mat[3][0] * m2.m_mat[0][1] + m_mat[3][1] * m2.m_mat[1][1] + m_mat[3][2] * m2.m_mat[2][1] +
                m_mat[3][3] * m2.m_mat[3][1];
            r.m_mat[3][2] = m_mat[3][0] * m2.m_mat[0][2] + m_mat[3][1] * m2.m_mat[1][2] + m_mat[3][2] * m2.m_mat[2][2] +
                m_mat[3][3] * m2.m_mat[3][2];
            r.m_mat[3][3] = m_mat[3][0] * m2.m_mat[0][3] + m_mat[3][1] * m2.m_mat[1][3] + m_mat[3][2] * m2.m_mat[2][3] +
                m_mat[3][3] * m2.m_mat[3][3];

            return r;
        }

        /** Matrix concatenation using '*'.
         */
        Mat4 operator*(Mat4 const& m2) const { return concatenate(m2); }

        /** Vector transformation using '*'.
        @remarks
        Transforms the given 3-D vector by the matrix, projecting the
        result back into <i>w</i> = 1.
        @note
        This means that the initial <i>w</i> is considered to be 1.0,
        and then all the three elements of the resulting 3-D vector are
        divided by the resulting <i>w</i>.
        */
        Vec3 operator*(Vec3 const& v) const
        {
            Vec3 r;

            float inv_w = 1.0f / (m_mat[3][0] * v.x + m_mat[3][1] * v.y + m_mat[3][2] * v.z + m_mat[3][3]);

            r.x = (m_mat[0][0] * v.x + m_mat[0][1] * v.y + m_mat[0][2] * v.z + m_mat[0][3]) * inv_w;
            r.y = (m_mat[1][0] * v.x + m_mat[1][1] * v.y + m_mat[1][2] * v.z + m_mat[1][3]) * inv_w;
            r.z = (m_mat[2][0] * v.x + m_mat[2][1] * v.y + m_mat[2][2] * v.z + m_mat[2][3]) * inv_w;

            return r;
        }

        Vec4 operator*(Vec4 const& v) const
        {
            return Vec4(m_mat[0][0] * v.x + m_mat[0][1] * v.y + m_mat[0][2] * v.z + m_mat[0][3] * v.w,
                m_mat[1][0] * v.x + m_mat[1][1] * v.y + m_mat[1][2] * v.z + m_mat[1][3] * v.w,
                m_mat[2][0] * v.x + m_mat[2][1] * v.y + m_mat[2][2] * v.z + m_mat[2][3] * v.w,
                m_mat[3][0] * v.x + m_mat[3][1] * v.y + m_mat[3][2] * v.z + m_mat[3][3] * v.w);
        }

        /** Matrix addition.
         */
        Mat4 operator+(Mat4 const& m2) const
        {
            Mat4 r;

            r.m_mat[0][0] = m_mat[0][0] + m2.m_mat[0][0];
            r.m_mat[0][1] = m_mat[0][1] + m2.m_mat[0][1];
            r.m_mat[0][2] = m_mat[0][2] + m2.m_mat[0][2];
            r.m_mat[0][3] = m_mat[0][3] + m2.m_mat[0][3];

            r.m_mat[1][0] = m_mat[1][0] + m2.m_mat[1][0];
            r.m_mat[1][1] = m_mat[1][1] + m2.m_mat[1][1];
            r.m_mat[1][2] = m_mat[1][2] + m2.m_mat[1][2];
            r.m_mat[1][3] = m_mat[1][3] + m2.m_mat[1][3];

            r.m_mat[2][0] = m_mat[2][0] + m2.m_mat[2][0];
            r.m_mat[2][1] = m_mat[2][1] + m2.m_mat[2][1];
            r.m_mat[2][2] = m_mat[2][2] + m2.m_mat[2][2];
            r.m_mat[2][3] = m_mat[2][3] + m2.m_mat[2][3];

            r.m_mat[3][0] = m_mat[3][0] + m2.m_mat[3][0];
            r.m_mat[3][1] = m_mat[3][1] + m2.m_mat[3][1];
            r.m_mat[3][2] = m_mat[3][2] + m2.m_mat[3][2];
            r.m_mat[3][3] = m_mat[3][3] + m2.m_mat[3][3];

            return r;
        }

        /** Matrix subtraction.
         */
        Mat4 operator-(Mat4 const& m2) const
        {
            Mat4 r;
            r.m_mat[0][0] = m_mat[0][0] - m2.m_mat[0][0];
            r.m_mat[0][1] = m_mat[0][1] - m2.m_mat[0][1];
            r.m_mat[0][2] = m_mat[0][2] - m2.m_mat[0][2];
            r.m_mat[0][3] = m_mat[0][3] - m2.m_mat[0][3];

            r.m_mat[1][0] = m_mat[1][0] - m2.m_mat[1][0];
            r.m_mat[1][1] = m_mat[1][1] - m2.m_mat[1][1];
            r.m_mat[1][2] = m_mat[1][2] - m2.m_mat[1][2];
            r.m_mat[1][3] = m_mat[1][3] - m2.m_mat[1][3];

            r.m_mat[2][0] = m_mat[2][0] - m2.m_mat[2][0];
            r.m_mat[2][1] = m_mat[2][1] - m2.m_mat[2][1];
            r.m_mat[2][2] = m_mat[2][2] - m2.m_mat[2][2];
            r.m_mat[2][3] = m_mat[2][3] - m2.m_mat[2][3];

            r.m_mat[3][0] = m_mat[3][0] - m2.m_mat[3][0];
            r.m_mat[3][1] = m_mat[3][1] - m2.m_mat[3][1];
            r.m_mat[3][2] = m_mat[3][2] - m2.m_mat[3][2];
            r.m_mat[3][3] = m_mat[3][3] - m2.m_mat[3][3];

            return r;
        }

        Mat4 operator*(float scalar) const
        {
            return Mat4(scalar * m_mat[0][0],
                scalar * m_mat[0][1],
                scalar * m_mat[0][2],
                scalar * m_mat[0][3],
                scalar * m_mat[1][0],
                scalar * m_mat[1][1],
                scalar * m_mat[1][2],
                scalar * m_mat[1][3],
                scalar * m_mat[2][0],
                scalar * m_mat[2][1],
                scalar * m_mat[2][2],
                scalar * m_mat[2][3],
                scalar * m_mat[3][0],
                scalar * m_mat[3][1],
                scalar * m_mat[3][2],
                scalar * m_mat[3][3]);
        }

        /** Tests 2 matrices for equality.
         */
        bool operator==(Mat4 const& m2) const
        {
            return !(m_mat[0][0] != m2.m_mat[0][0] || m_mat[0][1] != m2.m_mat[0][1] || m_mat[0][2] != m2.m_mat[0][2] ||
                m_mat[0][3] != m2.m_mat[0][3] || m_mat[1][0] != m2.m_mat[1][0] || m_mat[1][1] != m2.m_mat[1][1] ||
                m_mat[1][2] != m2.m_mat[1][2] || m_mat[1][3] != m2.m_mat[1][3] || m_mat[2][0] != m2.m_mat[2][0] ||
                m_mat[2][1] != m2.m_mat[2][1] || m_mat[2][2] != m2.m_mat[2][2] || m_mat[2][3] != m2.m_mat[2][3] ||
                m_mat[3][0] != m2.m_mat[3][0] || m_mat[3][1] != m2.m_mat[3][1] || m_mat[3][2] != m2.m_mat[3][2] ||
                m_mat[3][3] != m2.m_mat[3][3]);
        }

        /** Tests 2 matrices for inequality.
         */
        bool operator!=(Mat4 const& m2) const
        {
            return m_mat[0][0] != m2.m_mat[0][0] || m_mat[0][1] != m2.m_mat[0][1] || m_mat[0][2] != m2.m_mat[0][2] ||
                m_mat[0][3] != m2.m_mat[0][3] || m_mat[1][0] != m2.m_mat[1][0] || m_mat[1][1] != m2.m_mat[1][1] ||
                m_mat[1][2] != m2.m_mat[1][2] || m_mat[1][3] != m2.m_mat[1][3] || m_mat[2][0] != m2.m_mat[2][0] ||
                m_mat[2][1] != m2.m_mat[2][1] || m_mat[2][2] != m2.m_mat[2][2] || m_mat[2][3] != m2.m_mat[2][3] ||
                m_mat[3][0] != m2.m_mat[3][0] || m_mat[3][1] != m2.m_mat[3][1] || m_mat[3][2] != m2.m_mat[3][2] ||
                m_mat[3][3] != m2.m_mat[3][3];
        }

        Mat4 transpose() const
        {
            return Mat4(m_mat[0][0],
                m_mat[1][0],
                m_mat[2][0],
                m_mat[3][0],
                m_mat[0][1],
                m_mat[1][1],
                m_mat[2][1],
                m_mat[3][1],
                m_mat[0][2],
                m_mat[1][2],
                m_mat[2][2],
                m_mat[3][2],
                m_mat[0][3],
                m_mat[1][3],
                m_mat[2][3],
                m_mat[3][3]);
        }

        //-----------------------------------------------------------------------
        float getMinor(size_t r0, size_t r1, size_t r2, size_t c0, size_t c1, size_t c2) const
        {
            return m_mat[r0][c0] * (m_mat[r1][c1] * m_mat[r2][c2] - m_mat[r2][c1] * m_mat[r1][c2]) -
                m_mat[r0][c1] * (m_mat[r1][c0] * m_mat[r2][c2] - m_mat[r2][c0] * m_mat[r1][c2]) +
                m_mat[r0][c2] * (m_mat[r1][c0] * m_mat[r2][c1] - m_mat[r2][c0] * m_mat[r1][c1]);
        }

        /*
        -----------------------------------------------------------------------
        Translation Transformation
        -----------------------------------------------------------------------
        */
        /** Sets the translation transformation part of the matrix.
         */
        void setTrans(Vec3 const& v)
        {
            m_mat[0][3] = v.x;
            m_mat[1][3] = v.y;
            m_mat[2][3] = v.z;
        }

        /** Extracts the translation transformation part of the matrix.
         */
        Vec3 getTrans() const { return Vec3(m_mat[0][3], m_mat[1][3], m_mat[2][3]); }

        Mat4 buildViewportMatrix(uint32_t width, uint32_t height)
        {
            return Mat4(0.5f * (float)width,
                0.0f,
                0.0f,
                0.5f * (float)width,
                0.0f,
                -0.5f * (float)height,
                0.0f,
                0.5f * (float)height,
                0.0f,
                0.0f,
                -1.0f,
                1.0f,
                0.0f,
                0.0f,
                0.0f,
                1.0f);
        }

        static Mat4 mirrorMatrix(Vec4 mirror_plane)
        {
            Mat4 result;
            result.m_mat[0][0] = -2 * mirror_plane.x * mirror_plane.x + 1;
            result.m_mat[1][0] = -2 * mirror_plane.x * mirror_plane.y;
            result.m_mat[2][0] = -2 * mirror_plane.x * mirror_plane.z;
            result.m_mat[3][0] = 0;

            result.m_mat[0][1] = -2 * mirror_plane.y * mirror_plane.x;
            result.m_mat[1][1] = -2 * mirror_plane.y * mirror_plane.y + 1;
            result.m_mat[2][1] = -2 * mirror_plane.y * mirror_plane.z;
            result.m_mat[3][1] = 0;

            result.m_mat[0][2] = -2 * mirror_plane.z * mirror_plane.x;
            result.m_mat[1][2] = -2 * mirror_plane.z * mirror_plane.y;
            result.m_mat[2][2] = -2 * mirror_plane.z * mirror_plane.z + 1;
            result.m_mat[3][2] = 0;

            result.m_mat[0][3] = -2 * mirror_plane.w * mirror_plane.x;
            result.m_mat[1][3] = -2 * mirror_plane.w * mirror_plane.y;
            result.m_mat[2][3] = -2 * mirror_plane.w * mirror_plane.z;
            result.m_mat[3][3] = 1;

            return result;
        }

        static Mat4 rotationMatrix(Vec3 normal)
        {
            Vec3 up = Vec3(0, 0, 1);
            if (fabs(normal.z) > 0.999f)
            {
                up = Vec3(0, 1, 0);
            }

            Vec3 left = up.crossProduct(normal);
            up = normal.crossProduct(left);

            left.normalise();
            up.normalise();

            Mat4 result = Mat4::IDENTITY;
            result.setMat3(Mat3(left, up, normal));

            return result.transpose();
        }

        /** Builds a translation matrix
         */
        void makeTrans(Vec3 const& v)
        {
            m_mat[0][0] = 1.0;
            m_mat[0][1] = 0.0;
            m_mat[0][2] = 0.0;
            m_mat[0][3] = v.x;
            m_mat[1][0] = 0.0;
            m_mat[1][1] = 1.0;
            m_mat[1][2] = 0.0;
            m_mat[1][3] = v.y;
            m_mat[2][0] = 0.0;
            m_mat[2][1] = 0.0;
            m_mat[2][2] = 1.0;
            m_mat[2][3] = v.z;
            m_mat[3][0] = 0.0;
            m_mat[3][1] = 0.0;
            m_mat[3][2] = 0.0;
            m_mat[3][3] = 1.0;
        }

        void makeTrans(float tx, float ty, float tz)
        {
            m_mat[0][0] = 1.0;
            m_mat[0][1] = 0.0;
            m_mat[0][2] = 0.0;
            m_mat[0][3] = tx;
            m_mat[1][0] = 0.0;
            m_mat[1][1] = 1.0;
            m_mat[1][2] = 0.0;
            m_mat[1][3] = ty;
            m_mat[2][0] = 0.0;
            m_mat[2][1] = 0.0;
            m_mat[2][2] = 1.0;
            m_mat[2][3] = tz;
            m_mat[3][0] = 0.0;
            m_mat[3][1] = 0.0;
            m_mat[3][2] = 0.0;
            m_mat[3][3] = 1.0;
        }

        /** Gets a translation matrix.
         */
        static Mat4 getTrans(Vec3 const& v)
        {
            Mat4 r;

            r.m_mat[0][0] = 1.0;
            r.m_mat[0][1] = 0.0;
            r.m_mat[0][2] = 0.0;
            r.m_mat[0][3] = v.x;
            r.m_mat[1][0] = 0.0;
            r.m_mat[1][1] = 1.0;
            r.m_mat[1][2] = 0.0;
            r.m_mat[1][3] = v.y;
            r.m_mat[2][0] = 0.0;
            r.m_mat[2][1] = 0.0;
            r.m_mat[2][2] = 1.0;
            r.m_mat[2][3] = v.z;
            r.m_mat[3][0] = 0.0;
            r.m_mat[3][1] = 0.0;
            r.m_mat[3][2] = 0.0;
            r.m_mat[3][3] = 1.0;

            return r;
        }

        /** Gets a translation matrix - variation for not using a vector.
         */
        static Mat4 getTrans(float t_x, float t_y, float t_z)
        {
            Mat4 r;

            r.m_mat[0][0] = 1.0;
            r.m_mat[0][1] = 0.0;
            r.m_mat[0][2] = 0.0;
            r.m_mat[0][3] = t_x;
            r.m_mat[1][0] = 0.0;
            r.m_mat[1][1] = 1.0;
            r.m_mat[1][2] = 0.0;
            r.m_mat[1][3] = t_y;
            r.m_mat[2][0] = 0.0;
            r.m_mat[2][1] = 0.0;
            r.m_mat[2][2] = 1.0;
            r.m_mat[2][3] = t_z;
            r.m_mat[3][0] = 0.0;
            r.m_mat[3][1] = 0.0;
            r.m_mat[3][2] = 0.0;
            r.m_mat[3][3] = 1.0;

            return r;
        }

        /*
        -----------------------------------------------------------------------
        Scale Transformation
        -----------------------------------------------------------------------
        */
        /** Sets the scale part of the matrix.
         */
        void setScale(Vec3 const& v)
        {
            m_mat[0][0] = v.x;
            m_mat[1][1] = v.y;
            m_mat[2][2] = v.z;
        }

        /** Gets a scale matrix.
         */
        static Mat4 getScale(Vec3 const& v)
        {
            Mat4 r;
            r.m_mat[0][0] = v.x;
            r.m_mat[0][1] = 0.0;
            r.m_mat[0][2] = 0.0;
            r.m_mat[0][3] = 0.0;
            r.m_mat[1][0] = 0.0;
            r.m_mat[1][1] = v.y;
            r.m_mat[1][2] = 0.0;
            r.m_mat[1][3] = 0.0;
            r.m_mat[2][0] = 0.0;
            r.m_mat[2][1] = 0.0;
            r.m_mat[2][2] = v.z;
            r.m_mat[2][3] = 0.0;
            r.m_mat[3][0] = 0.0;
            r.m_mat[3][1] = 0.0;
            r.m_mat[3][2] = 0.0;
            r.m_mat[3][3] = 1.0;

            return r;
        }

        /** Gets a scale matrix - variation for not using a vector.
         */
        static Mat4 buildScaleMatrix(float s_x, float s_y, float s_z)
        {
            Mat4 r;
            r.m_mat[0][0] = s_x;
            r.m_mat[0][1] = 0.0;
            r.m_mat[0][2] = 0.0;
            r.m_mat[0][3] = 0.0;
            r.m_mat[1][0] = 0.0;
            r.m_mat[1][1] = s_y;
            r.m_mat[1][2] = 0.0;
            r.m_mat[1][3] = 0.0;
            r.m_mat[2][0] = 0.0;
            r.m_mat[2][1] = 0.0;
            r.m_mat[2][2] = s_z;
            r.m_mat[2][3] = 0.0;
            r.m_mat[3][0] = 0.0;
            r.m_mat[3][1] = 0.0;
            r.m_mat[3][2] = 0.0;
            r.m_mat[3][3] = 1.0;

            return r;
        }

        /** Extracts the rotation / scaling part of the Matrix as a 3x3 matrix.
        @param m3x3 Destination Matrix3
        */
        void extract3x3Matrix(Mat3& m3x3) const
        {
            m3x3.m_mat[0][0] = m_mat[0][0];
            m3x3.m_mat[0][1] = m_mat[0][1];
            m3x3.m_mat[0][2] = m_mat[0][2];
            m3x3.m_mat[1][0] = m_mat[1][0];
            m3x3.m_mat[1][1] = m_mat[1][1];
            m3x3.m_mat[1][2] = m_mat[1][2];
            m3x3.m_mat[2][0] = m_mat[2][0];
            m3x3.m_mat[2][1] = m_mat[2][1];
            m3x3.m_mat[2][2] = m_mat[2][2];
        }

        void extractAxes(Vec3& out_x, Vec3& out_y, Vec3& out_z) const
        {
            out_x = Vec3(m_mat[0][0], m_mat[1][0], m_mat[2][0]);
            out_x.normalise();
            out_y = Vec3(m_mat[0][1], m_mat[1][1], m_mat[2][1]);
            out_y.normalise();
            out_z = Vec3(m_mat[0][2], m_mat[1][2], m_mat[2][2]);
            out_z.normalise();
        }

        /** Determines if this matrix involves a scaling. */
        bool hasScale() const
        {
            // check magnitude of column vectors (==local axes)
            float t = m_mat[0][0] * m_mat[0][0] + m_mat[1][0] * m_mat[1][0] + m_mat[2][0] * m_mat[2][0];
            if (!Math::realEqual(t, 1.0, (float)1e-04))
                return true;
            t = m_mat[0][1] * m_mat[0][1] + m_mat[1][1] * m_mat[1][1] + m_mat[2][1] * m_mat[2][1];
            if (!Math::realEqual(t, 1.0, (float)1e-04))
                return true;
            t = m_mat[0][2] * m_mat[0][2] + m_mat[1][2] * m_mat[1][2] + m_mat[2][2] * m_mat[2][2];
            return !Math::realEqual(t, 1.0, (float)1e-04);
        }

        /** Determines if this matrix involves a negative scaling. */
        bool hasNegativeScale() const { return determinant() < 0; }

        /** Extracts the rotation / scaling part as a quaternion from the Matrix.
         */
        Quaternion extractQuaternion() const
        {
            Mat3 m3x3;
            extract3x3Matrix(m3x3);
            return Quaternion(m3x3);
        }

        Mat4 adjoint() const;

        float determinant() const
        {
            return m_mat[0][0] * getMinor(1, 2, 3, 1, 2, 3) - m_mat[0][1] * getMinor(1, 2, 3, 0, 2, 3) +
                m_mat[0][2] * getMinor(1, 2, 3, 0, 1, 3) - m_mat[0][3] * getMinor(1, 2, 3, 0, 1, 2);
        }

        /** Building a Matrix4 from orientation / scale / position.
        @remarks
        Transform is performed in the order scale, rotate, translation, i.e. translation is independent
        of orientation axes, scale does not affect size of translation, rotation and scaling are always
        centered on the origin.
        */
        void makeTransform(Vec3 const& position, Vec3 const& scale, Quaternion const& orientation);

        /** Building an inverse Matrix4 from orientation / scale / position.
        @remarks
        As makeTransform except it build the inverse given the same data as makeTransform, so
        performing -translation, -rotate, 1/scale in that order.
        */
        void makeInverseTransform(Vec3 const& position, Vec3 const& scale, Quaternion const& orientation);

        /** Decompose a Matrix4 to orientation / scale / position.
         */
        void decomposition(Vec3& position, Vec3& scale, Quaternion& orientation) const;

        void decompositionWithoutScale(Vec3& position, Quaternion& rotation) const;

        /** Check whether or not the matrix is affine matrix.
        @remarks
        An affine matrix is a 4x4 matrix with row 3 equal to (0, 0, 0, 1),
        e.g. no projective coefficients.
        */
        bool isAffine(void) const
        {
            return m_mat[3][0] == 0 && m_mat[3][1] == 0 && m_mat[3][2] == 0 && m_mat[3][3] == 1;
        }

        /** Returns the inverse of the affine matrix.
        @note
        The matrix must be an affine matrix. @see Matrix4::isAffine.
        */
        Mat4 inverseAffine() const;

        /** Concatenate two affine matrices.
        @note
        The matrices must be affine matrix. @see Matrix4::isAffine.
        */
        Mat4 concatenateAffine(Mat4 const& m2) const
        {
            assert(isAffine() && m2.isAffine());

            return Mat4(m_mat[0][0] * m2.m_mat[0][0] + m_mat[0][1] * m2.m_mat[1][0] + m_mat[0][2] * m2.m_mat[2][0],
                m_mat[0][0] * m2.m_mat[0][1] + m_mat[0][1] * m2.m_mat[1][1] + m_mat[0][2] * m2.m_mat[2][1],
                m_mat[0][0] * m2.m_mat[0][2] + m_mat[0][1] * m2.m_mat[1][2] + m_mat[0][2] * m2.m_mat[2][2],
                m_mat[0][0] * m2.m_mat[0][3] + m_mat[0][1] * m2.m_mat[1][3] +
                m_mat[0][2] * m2.m_mat[2][3] + m_mat[0][3],

                m_mat[1][0] * m2.m_mat[0][0] + m_mat[1][1] * m2.m_mat[1][0] + m_mat[1][2] * m2.m_mat[2][0],
                m_mat[1][0] * m2.m_mat[0][1] + m_mat[1][1] * m2.m_mat[1][1] + m_mat[1][2] * m2.m_mat[2][1],
                m_mat[1][0] * m2.m_mat[0][2] + m_mat[1][1] * m2.m_mat[1][2] + m_mat[1][2] * m2.m_mat[2][2],
                m_mat[1][0] * m2.m_mat[0][3] + m_mat[1][1] * m2.m_mat[1][3] +
                m_mat[1][2] * m2.m_mat[2][3] + m_mat[1][3],

                m_mat[2][0] * m2.m_mat[0][0] + m_mat[2][1] * m2.m_mat[1][0] + m_mat[2][2] * m2.m_mat[2][0],
                m_mat[2][0] * m2.m_mat[0][1] + m_mat[2][1] * m2.m_mat[1][1] + m_mat[2][2] * m2.m_mat[2][1],
                m_mat[2][0] * m2.m_mat[0][2] + m_mat[2][1] * m2.m_mat[1][2] + m_mat[2][2] * m2.m_mat[2][2],
                m_mat[2][0] * m2.m_mat[0][3] + m_mat[2][1] * m2.m_mat[1][3] +
                m_mat[2][2] * m2.m_mat[2][3] + m_mat[2][3],
                0,
                0,
                0,
                1);
        }

        /** 3-D Vector transformation specially for an affine matrix.
        @remarks
        Transforms the given 3-D vector by the matrix, projecting the
        result back into <i>w</i> = 1.
        @note
        The matrix must be an affine matrix. @see Matrix4::isAffine.
        */
        Vec3 transformAffine(Vec3 const& v) const
        {
            assert(isAffine());

            return Vec3(m_mat[0][0] * v.x + m_mat[0][1] * v.y + m_mat[0][2] * v.z + m_mat[0][3],
                m_mat[1][0] * v.x + m_mat[1][1] * v.y + m_mat[1][2] * v.z + m_mat[1][3],
                m_mat[2][0] * v.x + m_mat[2][1] * v.y + m_mat[2][2] * v.z + m_mat[2][3]);
        }

        /** 4-D Vector transformation specially for an affine matrix.
        @note
        The matrix must be an affine matrix. @see Matrix4::isAffine.
        */
        Vec4 transformAffine(Vec4 const& v) const
        {
            assert(isAffine());

            return Vec4(m_mat[0][0] * v.x + m_mat[0][1] * v.y + m_mat[0][2] * v.z + m_mat[0][3] * v.w,
                m_mat[1][0] * v.x + m_mat[1][1] * v.y + m_mat[1][2] * v.z + m_mat[1][3] * v.w,
                m_mat[2][0] * v.x + m_mat[2][1] * v.y + m_mat[2][2] * v.z + m_mat[2][3] * v.w,
                v.w);
        }

        Mat4 inverse() const
        {
            float m00 = m_mat[0][0], m01 = m_mat[0][1], m02 = m_mat[0][2], m03 = m_mat[0][3];
            float m10 = m_mat[1][0], m11 = m_mat[1][1], m12 = m_mat[1][2], m13 = m_mat[1][3];
            float m20 = m_mat[2][0], m21 = m_mat[2][1], m22 = m_mat[2][2], m23 = m_mat[2][3];
            float m30 = m_mat[3][0], m31 = m_mat[3][1], m32 = m_mat[3][2], m33 = m_mat[3][3];

            float v0 = m20 * m31 - m21 * m30;
            float v1 = m20 * m32 - m22 * m30;
            float v2 = m20 * m33 - m23 * m30;
            float v3 = m21 * m32 - m22 * m31;
            float v4 = m21 * m33 - m23 * m31;
            float v5 = m22 * m33 - m23 * m32;

            float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
            float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
            float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
            float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

            float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

            float d00 = t00 * invDet;
            float d10 = t10 * invDet;
            float d20 = t20 * invDet;
            float d30 = t30 * invDet;

            float d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
            float d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
            float d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
            float d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

            v0 = m10 * m31 - m11 * m30;
            v1 = m10 * m32 - m12 * m30;
            v2 = m10 * m33 - m13 * m30;
            v3 = m11 * m32 - m12 * m31;
            v4 = m11 * m33 - m13 * m31;
            v5 = m12 * m33 - m13 * m32;

            float d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
            float d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
            float d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
            float d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

            v0 = m21 * m10 - m20 * m11;
            v1 = m22 * m10 - m20 * m12;
            v2 = m23 * m10 - m20 * m13;
            v3 = m22 * m11 - m21 * m12;
            v4 = m23 * m11 - m21 * m13;
            v5 = m23 * m12 - m22 * m13;

            float d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
            float d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
            float d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
            float d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

            return Mat4(d00, d01, d02, d03, d10, d11, d12, d13, d20, d21, d22, d23, d30, d31, d32, d33);
        }

        Vec3 transformCoord(Vec3 const& v)
        {
            Vec4 temp(v, 1.0f);
            Vec4 ret = (*this) * temp;
            if (ret.w == 0.0f)
            {
                return Vec3::ZERO;
            }
            else
            {
                ret /= ret.w;
                return Vec3(ret.x, ret.y, ret.z);
            }

            return Vec3::ZERO;
        }

        static Mat4 const ZERO;
        static Mat4 const ZEROAFFINE;
        static Mat4 const IDENTITY;
    };

    Vec4 operator*(Vec4 const& v, Mat4 const& mat);

} // namespace VenusEngine