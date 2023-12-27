#pragma once

#include <vector>

#include "Render/ShaderProgram.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"

#include "Math/MathHeaders.h"

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
		Mesh() = default;

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
			std::size_t oldSize = m_vertices.size();
			std::size_t newSize = m_vertices.size() + geometry.size();
			m_vertices.resize(newSize);
			std::copy(geometry.begin(), geometry.end(), m_vertices.begin() + oldSize);
		}

		/// \brief Copies this Mesh's geometry into this Mesh's VBO and sets up its
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
			// Tell the shaders how the data in the array is laid out
			glEnableVertexAttribArray(0);
			// Positions have 2 parts, each are floats, and start at beginning of array
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
				reinterpret_cast<void*>(0));
			glEnableVertexAttribArray(1);
			// Colors have 3 parts, each are floats, and start at 7th position in array
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
				reinterpret_cast<void*>(3 * sizeof(float)));
			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		}

		/// \brief Draws this Mesh in OpenGL.
		/// \param[in] shaderProgram A pointer to the ShaderProgram that should
		///   be used.
		/// \param[in] viewMatrix The view matrix that should be used by itself as
		///   the model-view matrix(there is not yet any model part).
		/// \pre This Mesh has been prepared.
		/// \post While the ShaderProgram was enabled, the viewMatrix has been set as
		///   the "uModelView" uniform matrix and the geometry has been drawn.
		void draw(ShaderProgram& shaderProgram, Mat4 const& viewMatrix)
		{
			shaderProgram.enable();

			shaderProgram.setUniformMatrix("uModelView", viewMatrix);

			m_vertexArray.bind();
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()) / 6);
			m_vertexArray.unbind();

			shaderProgram.disable();
		}

	private:
		std::vector<float> m_vertices;
		VertexArray m_vertexArray;
		VertexBuffer m_vertexBuffer;
	};
}