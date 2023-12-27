#pragma once

#include <unordered_map>

#include "Core/Mesh.h"

namespace VenusEngine
{
	class Scene
	{
	public:
		Scene() = default;

		~Scene() = default;

		/// \brief Copy constructor removed because you shouldn't be copying Scenes.
		Scene(Scene const&) = delete;

		/// \brief Assignment operator removed because you shouldn't be assigning
		///   Scenes.
		void operator=(Scene const&) = delete;

		/// \brief Adds a new Mesh to this Scene.
		/// \param[in] meshName The name of the Mesh.  Unique names should be used so
		///   that you can access it in the future.
		/// \param[in] mesh A pointer to the Mesh that should be added.  This Mesh
		///   must have been dynamically allocated.  The Scene will now own this Mesh
		///   and be responsible for de-allocating it.
		/// \pre The Scene does not contain any Mesh associated with meshName.
		/// \post The Scene contains the mesh, associated with the meshName.
		void add(std::string const& meshName, std::shared_ptr<Mesh> mesh)
		{
			m_meshes[meshName] = mesh;
		}

		/// \brief Removes a Mesh from this Scene.
		/// \param[in] meshName The name of the Mesh that should be removed.
		/// \pre This Scene contains a Mesh associated with meshName.
		/// \post This Scene no longer associates meshName with anything.
		/// \post The Mesh that had been associated with meshName has been freed.
		void remove(std::string const& meshName)
		{
			m_meshes.erase(meshName);
		}

		/// \brief Removes all Meshes from this Scene.
		/// \post This Scene is empty.
		/// \post All Meshes that had been part of this Scene have been freed.
		void clear()
		{
			m_meshes.clear();
		}

		/// \brief Draws all of the elements in this Scene.
		/// \param[in] shaderProgram The ShaderProgram that should be used for
		///   drawing.
		/// \param[in] viewMatrix The view matrix that should be used when drawing
		///   the Scene.
		void draw(ShaderProgram& shaderProgram, Mat4 const& viewMatrix)
		{
			for (auto& pair : m_meshes)
			{
				pair.second->draw(shaderProgram, viewMatrix);
			}
		}

		/// \brief Tests whether or not this Scene contains a Mesh associated with a
		///   name.
		/// \param[in] meshName The name of the requested Mesh.
		/// \return Whether or not this Scene contains a Mesh associated with
		///   meshName.
		bool hasMesh(std::string const& meshName)
		{
			return m_meshes.count(meshName);
		}

		/// \brief Gets the Mesh associated with a name.
		/// \param[in] meshName The name of the requested Mesh.
		/// \return A pointer to the Mesh associated with meshName.  This pointer
		///   should not be permanently stored, as the Mesh it points to will be
		///   deallocated with this Scene.
		/// \pre This Scene has a Mesh assocated with meshName.
		std::shared_ptr<Mesh> getMesh(std::string const& meshName)
		{
			return m_meshes[meshName];
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshes;
	};
}