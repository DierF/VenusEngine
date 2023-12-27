#include "Math/Radian.h"
#include "Math/Degree.h"
#include "Math/Math.h"

namespace VenusEngine
{
	Radian::Radian(Degree const& d)
		: m_radian(Math::degreesToRadians(float(d)))
	{
	}

	Radian& Radian::operator=(Degree const& d)
	{
        m_radian = Math::degreesToRadians(float(d));
		return *this;
	}

	Radian::operator Degree() const
	{
		return Degree(Math::radiansToDegrees(m_radian));
	}

	float Radian::toAngleUnits() const
	{
		return Math::radiansToAngleUnits(m_radian);
	}
} // namespace VenusEngine