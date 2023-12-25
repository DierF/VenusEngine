#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace VenusEngine
{
	/// \brief An eye that is viewing the scene.
	class Camera
	{
	public:
		/// \brief Constructs a new Camera.
		///
		/// \param eyePosition The location (in world coordinates) where the camera
		///   is located.
		/// \param localBackDirection A vector pointing backwards from the camera.
		/// \param nearClipPlaneDistance The distance to the closest things the
		///   camera can "see".
		/// \param farClipPlaneDistance The distance to the farthest things the
		///   camera can "see".
		/// \param aspectRatio The window's width divided by height.
		/// \param verticalFieldOfViewDegrees The angle determining how much the
		///   camera "sees".
		/// \post The camera's pose has been set and a projection matrix generated.
		/// \post The three orientation vectors are orthonormal with each other.
		/// \post All of the parameters have been copied into their respective data
		///   members, and the local right and up have been calculated to be
		///   orthonormal to the local back.
		Camera(glm::vec3 const& position,
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

		/// \brief Destructs a Camera.
		~Camera()
		{
		}

		/// \brief Sets the position (eye point) of the camera.
		/// \param[in] position The new position of the camera.
		/// \post The camera's location has been changed.
		void setPosition(glm::vec3 const& position)
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

		/// \brief Gets the view matrix, recalculating it only if necessary.
		/// \return A view matrix based on the camera's location and axis vectors.
		glm::mat4 getViewMatrix()
		{
			return glm::lookAt(m_position, m_position + m_frontDirection, m_upDirection);
		}

		/// \brief Gets the projection matrix.
		/// \return The projection matrix.
		glm::mat4 getProjectionMatrix()
		{
			return glm::perspective(glm::radians(m_verticalFieldOfViewDegrees), m_aspectRatio, m_nearClipPlaneDistance, m_farClipPlaneDistance);
		}

		glm::mat4 getViewProjectionMatrix()
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
			m_position = glm::vec3(0.0f, 0.0f, 1.0f);
			m_yaw   = 0.0f;
			m_pitch = 0.0f;
			updateCameraOrientation();
		}

	private:
		void updateCameraOrientation()
		{
			glm::quat pitchQuat = glm::angleAxis(glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::quat yawQuat   = glm::angleAxis(glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::quat orientation = yawQuat * pitchQuat;
			glm::mat4 rotationMatrix = glm::toMat4(orientation);
			m_frontDirection = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
			m_rightDirection = glm::normalize(glm::cross(m_frontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
			m_upDirection    = glm::normalize(glm::cross(m_rightDirection, m_frontDirection));
		}

	private:
		/// The location of the camera.
		glm::vec3 m_position;

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
		glm::vec3 m_frontDirection;
		/// A vector pointing right from the camera.
		glm::vec3 m_rightDirection;
		/// A vector pointing up from the camera.
		glm::vec3 m_upDirection;
	};
}