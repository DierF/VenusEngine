#pragma once

#include <iostream>
#include <cmath>
#include <cassert>

#include "Math/Math.h"

namespace VenusEngine
{
	/// \brief A vector of 3 floating-point numbers.
	/// These should behave just like our normal mathematical understanding of
	///   vectors.
	/// We represent the vector as a linear combination of three basis vectors,
	///   storing the coefficients of each.
	/// They have public data members because any combination of x, y, and z
	///   values is a legal vector, and because we would like this class to be
	///   easy and efficient to use.
	class Vec3
	{
	public:

		/// \brief Initializes a new vector to have all coefficients 0.0f.
		/// \post All coefficients are 0.0f.
		Vec3()
			: x(0.0f), y(0.0f), z(0.0f)
		{
		}

		/// \brief Initializes a new vector to have all coefficients identical.
		/// \param[in] xyz_ The value that should be used for all three coefficients.
		/// \post All coefficients are equal to xyz_.
		Vec3(float xyz_)
			: x(xyz_), y(xyz_), z(xyz_)
		{
		}

		/// \brief Initializes a new vector with custom coefficients.
		/// \param[in] x_ The coefficient for the basis vector i.
		/// \param[in] y_ The coefficient for the basis vector j.
		/// \param[in] z_ The coefficient for the basis vector k.
		/// \post The coefficients are equal to x_, y_, and z_ respectively.
		Vec3(float x_, float y_, float z_)
			: x(x_), y(y_), z(z_)
		{
		}

		/// \brief Sets each coefficient to the same value.
		/// \param[in] xyz The value that should be used for all three coefficients.
		/// \post All coefficients are equal to xyz_.
		void set(float xyz_)
		{
			x = xyz_;
			y = xyz_;
			z = xyz_;
		}

		/// \brief Sets each coefficient to(potentially) different values.
		/// \param[in] x_ The new coefficient for the basis vector i.
		/// \param[in] y_ The new coefficient for the basis vector j.
		/// \param[in] z_ The new coefficient for the basis vector k.
		/// \post The coefficients are equal to x_, y_, and z_ respectively.
		void set(float x_, float y_, float z_)
		{
			x = x_;
			y = y_;
			z = z_;
		}

		/// \brief Replaces the direction of this vector to its exact opposite.
		/// \post The vector has been negated.
		void negate()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		/// \brief Compute the dot product of this with another vector.
		/// \param[in] vec The other vector.
		/// \return The dot product of this and vec.
		float dot(Vec3 const& vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z;
		}

		/// \brief Computes the angle(in radians) between this and another vector.
		/// \param[in] vec The other vector.
		/// \return The angle between this and vec, expressed in radians.
		float angleBetween(Vec3 const& vec) const
		{
			float lengthProduct = length() * vec.length();
			// Divide by zero check
			if (lengthProduct < 1e-6f)
			{
				lengthProduct = 1e-6f;
			}
			float cos = dot(vec) / lengthProduct;
			cos = std::clamp(cos, -1.0f, 1.0f);
			return std::acos(cos);
		}

		/// \brief Computes the cross product between this and another vector.
		/// \param[in] vec The other vector.
		/// \return The cross product of this vector with vec.
		Vec3 cross(Vec3 const& vec) const
		{
			return Vec3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
		}

		/// \brief Computes the length of this vector.
		/// \return The length of this vector.
		float length() const
		{
			return std::hypot(x, y, z);
		}

		/// \brief Normalizes this vector.
		/// \post This vector points in the same direction, but has a length of 1.
		void normalize()
		{
			float len = length();
			// Divide by zero check
			if (len < 1e-6f)
			{
				return;
			}
			float inv_lenh = 1.0f / len;
			x *= inv_lenh;
			y *= inv_lenh;
			z *= inv_lenh;
		}

		/// \brief Adds another vector to this one.
		/// \param[in] vec Another vector.
		/// \post This vector has been replaced by itself plus vec.
		/// \return This vector.
		Vec3& operator+=(Vec3 const& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}

		/// \brief Subtracts another vector from this one.
		/// \param[in] vec Another vector.
		/// \post This vector has been replaced by itself minus vec.
		/// \return This vector.
		Vec3& operator-=(Vec3 const& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}

		/// \brief Multiplies this vector by a scalar.
		/// \param[in] scalar A scalar.
		/// \post This vector has been replaced with itself times scalar.
		/// \return This vector.
		Vec3& operator*=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		/// \brief Divies this vector by a scalar.
		/// \param[in] scalar A scalar.
		/// \post This vector has been replaced with itself divided by scalar.
		/// \return This vector.
		Vec3& operator/=(float scalar)
		{
			assert(scalar != 0.0f && "Divided by zero");
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}
		
	public:
		/// \brief The coefficient of the basis vector i.
		float x;
		/// \brief The coefficient of the basis vector j.
		float y;
		/// \brief The coefficient of the basis vector k.
		float z;
	};

	/// \brief Adds two vectors.
	/// \param[in] lhs The first addend.
	/// \param[in] rhs The second addend.
	/// \return A new vector that is lhs + rhs.
	Vec3 operator+(Vec3 const& lhs, Vec3 const& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	}

	/// \brief Subtracts two vectors.
	/// \param[in] lhs The minuend.
	/// \param[in] rhs The subtrahend.
	/// \return A new vector that is lhs - rhs.
	Vec3 operator-(Vec3 const& lhs, Vec3 const& rhs)
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
	}

	/// \brief Negates a vector.
	/// \param[in] vec A vector.
	/// \return A new vector that is the negation of vec.
	Vec3 operator-(Vec3 const& vec)
	{
		return { -vec.x, -vec.y, -vec.z };
	}

	/// \brief Multiplies a scalar by a vector.
	/// \param[in] scalar A scalar.
	/// \param[in] vec A vector.
	/// \return A new vector that is scalar * vec.
	Vec3 operator*(float scalar, Vec3 const& vec)
	{
		return vec * scalar;
	}

	/// \brief Multiplies a vector by a scalar.
	/// \param[in] vec A vector.
	/// \param[in] scalar A scalar.
	/// \return A new vector that is vec * scalar.
	Vec3 operator*(Vec3 const& vec, float scalar)
	{
		return { vec.x * scalar, vec.y * scalar, vec.z * scalar };
	}

	/// \brief Divides a vector by a scalar.
	/// \param[in] vec A vector.
	/// \param[in] scalar A scalar.
	/// \return A new vector that is vec / scalar.
	Vec3 operator/(Vec3 const& vec, float scalar)
	{
		assert(scalar != 0.0f && "Divided by zero");
		return { vec.x / scalar, vec.y / scalar, vec.z / scalar };
	}

	/// \brief Inserts a vector into an output stream.
	/// \param[in] out An output stream.
	/// \param[in] vec A vector.
	/// \return The output stream.
	/// \post The vector has been inserted into the output stream.
	std::ostream& operator<<(std::ostream& out, Vec3 const& vec)
	{
		return out << "Vec3( " << vec.x << ", " << vec.y << ", " << vec.z << " )";
	}

	/// \brief Checks whether or not two vectors are equal.
	/// Vectors are equal if each of their respective components are within
	///   0.000001f of each other due to floating-point imprecision.
	/// \param[in] lhs A vector.
	/// \param[in] rhs Another vector.
	/// \return Whether or not lhs and rhs are equal.
	bool operator==(Vec3 const& lhs, Vec3 const& rhs)
	{
		return std::abs(lhs.x - rhs.x) < 1e-6f && std::abs(lhs.y - rhs.y) < 1e-6f && std::abs(lhs.z - rhs.z) < 1e-6f;
	}
}