#pragma once

#include "Math/Matrix4.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"

namespace VenusEngine
{
    class Transform
    {
    public:
        Vec3       m_position { Vec3::ZERO };
        Vec3       m_scale    { Vec3::UNIT_SCALE };
        Quaternion m_rotation { Quaternion::IDENTITY };

        Transform() = default;
        Transform(Vec3 const& position, Quaternion const& rotation, Vec3 const& scale)
            : m_position{ position }, m_scale{ scale }, m_rotation{ rotation }
        {
        }

        Mat4 getMatrix() const
        {
            Mat4 temp;
            temp.makeTransform(m_position, m_scale, m_rotation);
            return temp;
        }
    };
} // namespace VenusEngine