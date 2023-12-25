#pragma once

#include <glm/glm.hpp>

namespace VenusEngine
{
	struct Camera
	{
		/// The location of this Camera in world coordinates.
		glm::vec3 position;
		/// The point in world coordinates that this Camera is focusing on.
		glm::vec3 at;
		/// The direction in world coordinates that this Camera considers to be up.
		glm::vec3 up;
		/// A projection matrix for this Camera.
		glm::mat4 projection;
	};
}