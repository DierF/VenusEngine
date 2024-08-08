#pragma once

#include "Render/ShaderProgram.h"
#include "Math/MathHeaders.h"

namespace VenusEngine
{
	/// \brief An eye that is viewing the scene.
	class Camera
	{
	public:
		Camera(Vec3 const& position,
			   Vec3 const& target,
			   float nearClipPlaneDistance,
			   float farClipPlaneDistance,
			   float aspectRatio,
			   float verticalFieldOfViewDegrees)
			: m_position(position),
			  m_target(target),
			  m_nearClipPlaneDistance(nearClipPlaneDistance),
			  m_farClipPlaneDistance(farClipPlaneDistance),
			  m_aspectRatio(aspectRatio),
			  m_verticalFieldOfViewDegrees(verticalFieldOfViewDegrees)
		{
			updateCameraOrientation();
		}

		~Camera() = default;

		/// \brief Sets the position (eye point) of the camera.
		/// \param[in] position The new position of the camera.
		/// \post The camera's location has been changed.
		void setPosition(Vec3 const& position)
		{
			m_position = position;
		}

		void setTarget(Vec3 const& target)
		{
			m_target = target;
		}

		/// \brief Moves the position (eye point) of the camera right or left.
		/// \param[in] distance How far to move along the right vector.
		/// \post The camera's location has been changed.
		void moveRight(float distance)
		{
			m_position += m_rightDirection * distance;
			m_target   += m_rightDirection * distance;
		}

		/// \brief Moves the position (eye point) of the camera up or down.
		/// \param[in] distance How far to move along the up vector.
		/// \post The camera's location has been changed.
		void moveUp(float distance)
		{
			m_position += m_upDirection * distance;
			m_target   += m_upDirection * distance;
		}

		/// \brief Moves the position (eye point) of the camera back or forward.
		/// \param[in] distance How far to move along the back vector.
		/// \post The camera's location has been changed.
		void moveFront(float distance)
		{
			m_position += m_frontDirection * distance;
			m_target   += m_frontDirection * distance;
		}

		/// \brief Moves the camera horizontally around the target position (orbit around y-axis).
		/// \param[in] angleDegrees The angle to rotate in degrees.
		void rotateAroundHorizontally(float angleDegrees)
		{
			// Step 1: Translate the camera and target to the origin
			Vec3 translatedPosition = m_position - m_target;

			// Step 2: Perform the rotation around the Y-axis (horizontal axis)
			float angleRadians = Math::degreesToRadians(angleDegrees);

			// Create a quaternion for the rotation around the Y-axis
			Quaternion rotationQuat = Quaternion::getQuaternionFromAngleAxis(Radian(angleRadians), Vec3(0.0f, 1.0f, 0.0f));

			// Rotate the translated position
			Vec3 rotatedPosition = rotationQuat * translatedPosition;

			// Step 3: Translate back to the original position
			m_position = m_target + rotatedPosition;

			updateCameraOrientation();
		}

		/// \brief Moves the camera vertically around the target position (pitch).
		/// \param[in] angleDegrees The angle to rotate in degrees.
		void rotateAroundVertically(float angleDegrees)
		{
			// Update the pitch angle
			m_pitch += angleDegrees;

			// Normalize the pitch to be within -180 to 180 degrees
			while (m_pitch > 180.0f)
			{
				m_pitch -= 360.0f;
			}
			while (m_pitch < -180.0f)
			{
				m_pitch += 360.0f;
			}

			// Step 1: Translate the camera and target to the origin
			Vec3 translatedPosition = m_position - m_target;

			// Step 2: Perform the rotation around the X-axis (vertical rotation)
			float angleRadians = Math::degreesToRadians(angleDegrees);

			// Create a quaternion for the rotation around the X-axis
			Quaternion rotationQuat = Quaternion::getQuaternionFromAngleAxis(Radian(angleRadians), m_rightDirection);

			// Rotate the translated position
			Vec3 rotatedPosition = rotationQuat * translatedPosition;

			// Step 3: Translate back to the original position
			m_position = m_target + rotatedPosition;

			updateCameraOrientation();
		}

		
		/// \brief Zooms the camera in or out by adjusting the distance from the target.
		/// \param[in] distance The distance to zoom in (positive) or out (negative).
		void zoom(float distance)
		{
			Vec3 direction = m_position - m_target;

			float currentRadius = direction.length();

			direction.normalise();

			// Update radius
			float newRadius = currentRadius + distance;
			newRadius = Math::max(newRadius, 0.1f); // Prevent the radius from becoming too small

			// Update position
			m_position = m_target + direction * newRadius;

			updateCameraOrientation();
		}

		/// \brief Gets the view matrix, recalculating it only if necessary.
		/// \return A view matrix based on the camera's location and axis vectors.
		Mat4 getViewMatrix()
		{
			return Math::makeLookAtMatrix(m_position, m_target, m_upDirection);
		}

		/// \brief Gets the projection matrix.
		/// \return The projection matrix.
		Mat4 getProjectionMatrix()
		{
			return Math::makePerspectiveMatrix(Radian(Math::degreesToRadians(m_verticalFieldOfViewDegrees)), m_aspectRatio, m_nearClipPlaneDistance, m_farClipPlaneDistance);
		}

		Mat4 getViewProjectionMatrix()
		{
			return getProjectionMatrix() * getViewMatrix();
		}

		/// \brief Resets the camera to its original pose.
		/// \post The position (eye point) is the same as what had been specified in
		///   the constructor.
		/// \post The back vector is the same as what had been specified in the
		///   constructor.
		/// \post The up and right vectors are computed as they had been in the
		///   constructor.
		void resetPose()
		{
			m_position = Vec3(0.0f, 0.0f, 10.0f);
			m_target = Vec3();
			updateCameraOrientation();
		}

		void draw(ShaderProgram& shaderProgram)
		{
			shaderProgram.enable();

			shaderProgram.setUniformMat4("uView"      , getViewMatrix());
			shaderProgram.setUniformMat4("uProjection", getProjectionMatrix());

			shaderProgram.setUniformVec3("uEyePosition", m_position);

			shaderProgram.disable();
		}

		void updateAspectRatio(float aspectRatio)
		{
			m_aspectRatio = aspectRatio;
		}

	private:
		void updateCameraOrientation()
		{
			// Calculate the front direction
			m_frontDirection = m_target - m_position;
			m_frontDirection.normalise();

			if (Math::abs(m_pitch) < 90.0f)
			{
				m_rightDirection = m_frontDirection.crossProduct(Vec3(0.0f, 1.0f, 0.0f));
			}
			else
			{
				m_rightDirection = m_frontDirection.crossProduct(Vec3(0.0f, -1.0f, 0.0f));
			}
			m_rightDirection.normalise();

			m_upDirection = m_rightDirection.crossProduct(m_frontDirection);
			m_upDirection.normalise();
		}

	private:
		/// The location of the camera
		Vec3 m_position;

		/// The location of the target
		Vec3 m_target;

		float m_nearClipPlaneDistance;
		float m_farClipPlaneDistance;
		float m_aspectRatio;
		/// fov (in degree)
		float m_verticalFieldOfViewDegrees;

		/// A vector pointing back from the camera.
		Vec3 m_frontDirection;
		/// A vector pointing right from the camera.
		Vec3 m_rightDirection;
		/// A vector pointing up from the camera.
		Vec3 m_upDirection;

	private:
		// Necessary if allow vertical rotation over 90 degrees
		float m_pitch = 0.0f;
	};
}