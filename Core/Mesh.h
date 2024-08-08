#pragma once

#include <vector>

#include "Render/ShaderProgram.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"
#include "Math/MathHeaders.h"
#include "Core/ID.h"

namespace VenusEngine
{
	class Mesh
	{
	public:
		/// \brief Constructs an empty Mesh with no triangles.
		/// \param context A pointer to an object through which the Mesh will be able
		///   to make OpenGL calls.
		/// \post A unique VAO and VBO have been generated for this Mesh and stored
		///   for later use.
		Mesh() : m_id(ID::generateID())
		{
		}

		/// \brief Destructs this Mesh.
		/// \post The VAO and VBO associated with this Mesh have been deleted.
		~Mesh() = default;

		/// \brief Copy constructor removed because you shouldn't be copying Meshes.
		Mesh(Mesh const&) = delete;

		/// \brief Assignment operator removed because you shouldn't be assigning
		///   Meshes.
		Mesh& operator=(Mesh const&) = delete;

		/// \brief Adds the geometry of [additional] triangles to this Mesh.
		/// \param[in] geometry A collection of vertex data for 1 or more triangles.
		///   Each vertex must contain interleaved 3-D position and 3-D color data
		///  (X, Y, Z, R, G, B) and the vector must contain complete triangles(3
		///   vertices each).
		/// \pre This Mesh has not yet been prepared.
		/// \post The geometry has been appended to this Mesh's internal geometry
		///   store for future use.
		void addGeometry(std::vector<float> const& geometry)
		{
			m_vertices.insert(m_vertices.end(), geometry.begin(), geometry.end());
		}

		/// \brief Copies this Mesh's geometry into this Mesh's VBO an d sets up its
		///   VAO.
		/// \pre This Mesh has not yet been prepared.
		/// \post The first two vertex attributes have been enabled, with
		///   interleaved 3-part positions and 3-part colors.
		/// \post This Mesh's geometry has been copied to its VBO.
		void prepareVao()
		{
			m_vertexArray.bind();
			m_vertexBuffer.bind();
			m_vertexBuffer.bufferData(m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);
			enableAttributes();
			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		}

		/// \brief Adds additional triangles to this Mesh.
		/// \param[in] indices A collection of indices into the vertex buffer for 1
		///   or more triangles.  There must be 3 indices per triangle.
		/// \pre This Mesh has not yet been prepared.
		/// \post The indices have been appended to this Mesh's internal index store
		///   for future use.
		void addIndices(std::vector<unsigned int> const& indices)
		{
			std::size_t oldSize = m_indices.size();
			std::size_t newSize = m_indices.size() + indices.size();
			m_indices.resize(newSize);
			std::copy(indices.begin(), indices.end(), m_indices.begin() + oldSize);
		}

		/// \brief Gets the number of floats used to represent each vertex.
		/// \return The number of floats used for each vertex.
		std::size_t getFloatsPerVertex() const
		{
			return m_vertices.size() / m_indices.size();
		}


		/// \brief Draws this Mesh in OpenGL.
		/// \param[in] shaderProgram A pointer to the ShaderProgram that should
		///   be used.
		/// \pre This Mesh has been prepared.
		/// \post While the ShaderProgram was enabled, the viewMatrix has been set as
		///   the "uModelView" uniform matrix and the geometry has been drawn.
		void draw(ShaderProgram& shaderProgram)
		{
			shaderProgram.enable();

			shaderProgram.setUniformMat4("uWorld", m_transform.getMatrix());
			shaderProgram.setUniformInt("objectID", m_id);

			m_vertexArray.bind();
			// 3 float position, 3 float normal, 3 float color
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()) / 9);
			m_vertexArray.unbind();

			shaderProgram.disable();
		}

		/// \brief Gets the mesh's world matrix.
		/// \return The world matrix.
		Transform& getTransform()
		{
			return m_transform;
		}

		unsigned getID() const
		{
			return m_id;
		}

	private:
		/// \brief Enables VAO attributes.
		/// \pre This Mesh's VAO has been bound.
		/// \post Any attributes (positions, colors, normals, texture coordinates)
		///   have been enabled and configured.
		/// This should only be called from the middle of prepareVao().
		void enableAttributes()
		{
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(0));

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
		}

	private:
		std::vector<float>        m_vertices;
		std::vector<unsigned int> m_indices;
		Transform                 m_transform;
		VertexArray               m_vertexArray;
		VertexBuffer              m_vertexBuffer;

		int m_id;
	};
}