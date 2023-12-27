#include "Math/Degree.h"
#include "Math/Radian.h"
#include "Math/Math.h"

namespace VenusEngine
{
	Degree::Degree(Radian const& r)
		: m_degree(Math::radiansToDegrees(float(r)))
	{
	}

	Degree& Degree::operator=(Radian const& r)
	{
        m_degree = Math::radiansToDegrees(float(r));
		return *this;
	}

	Degree::operator Radian() const
	{
		return Radian(Math::degreesToRadians(m_degree));
	}

	float Degree::toAngleUnits() const
	{
		return Math::degreesToAngleUnits(m_degree);
	}
} // namespace VenusEngine