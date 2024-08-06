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
			   float yaw,
			   float pitch,
			   float nearClipPlaneDistance,
			   float farClipPlaneDistance,
			   float aspectRatio,
			   float verticalFieldOfViewDegrees)
			: m_position(position),
			  m_yaw(yaw),
			  m_pitch(pitch),
			  m_nearClipPlaneDistance(nearClipPlaneDistance),
			  m_farClipPlaneDistance(farClipPlaneDistance),
			  m_aspectRatio(aspectRatio),
			  m_verticalFieldOfViewDegrees(verticalFieldOfViewDegrees)
		{
			updateCameraOrientation();
		}

		~Camera()
		{
		}

		/// \brief Sets the position (eye point) of the camera.
		/// \param[in] position The new position of the camera.
		/// \post The camera's location has been changed.
		void setPosition(Vec3 const& position)
		{
			m_position = position;
		}

		/// \brief Moves the position (eye point) of the camera right or left.
		/// \param[in] distance How far to move along the right vector.
		/// \post The camera's location has been changed.
		void moveRight(float distance)
		{
			m_position += m_rightDirection * distance;
		}

		/// \brief Moves the position (eye point) of the camera up or down.
		/// \param[in] distance How far to move along the up vector.
		/// \post The camera's location has been changed.
		void moveUp(float distance)
		{
			m_position += m_upDirection * distance;
		}

		/// \brief Moves the position (eye point) of the camera back or forward.
		/// \param[in] distance How far to move along the back vector.
		/// \post The camera's location has been changed.
		void moveFront(float distance)
		{
			m_position += m_frontDirection * distance;
		}

		void moveYawAndPitch(float deltaYaw, float deltaPitch)
		{
			m_yaw   += deltaYaw;
			m_pitch += deltaPitch;
			m_pitch = Math::clamp(m_pitch, -89.0f, 89.0f);

			updateCameraOrientation();
		}

		/// \brief Gets the view matrix, recalculating it only if necessary.
		/// \return A view matrix based on the camera's location and axis vectors.
		Mat4 getViewMatrix()
		{
			return Math::makeLookAtMatrix(m_position, m_position + m_frontDirection, m_upDirection);
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
			m_position = Vec3(0.0f, 0.0f, 12.0f);
			m_yaw   = 0.0f;
			m_pitch = 0.0f;
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
			// Create quaternions for pitch and yaw rotations
			Quaternion pitchQuat = Quaternion::getQuaternionFromAngleAxis(Radian(Math::degreesToRadians(m_pitch)), Vec3(1.0f, 0.0f, 0.0f));
			Quaternion yawQuat = Quaternion::getQuaternionFromAngleAxis(Radian(Math::degreesToRadians(m_yaw)), Vec3(0.0f, 1.0f, 0.0f));

			// Combine the quaternions
			Quaternion orientation = yawQuat * pitchQuat;

			// Convert the quaternion to a rotation matrix
			Mat4 rotationMatrix;
			orientation.toRotationMatrix(rotationMatrix);

			// Calculate the forward direction of the camera
			Vec4 front = rotationMatrix * Vec4(0.0f, 0.0f, -1.0f, 0.0f);
			m_frontDirection = Vec3(front.x, front.y, front.z);
			m_frontDirection.normalise();

			// Calculate the right and up directions
			m_rightDirection = m_frontDirection.crossProduct(Vec3(0.0f, 1.0f, 0.0f));
			m_rightDirection.normalise();

			m_upDirection = m_rightDirection.crossProduct(m_frontDirection);
			m_upDirection.normalise();
		}

	private:
		/// The location of the camera.
		Vec3 m_position;

		/// horizontal (in degree)
		float m_yaw;
		/// vertical (in degree)
		float m_pitch;

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
	};
}