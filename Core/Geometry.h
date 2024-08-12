#pragma once

#include <vector>
#include <array>

#include "Math/MathHeaders.h"

namespace VenusEngine
{
	class Geometry
	{
	public:
		// A triangle consists of exactly 3 Vec3s (the coordinates of the vertices).
		using Triangle = std::array<Vec3, 3>;

		/// \brief Indexes some geometry.
		/// \param[in] geometry A collection containing floats defining some vertices.
		/// \param[in] floatsPerVertex The number of floats used for each vertex.
		/// \param[out] data A collection into which unique vertex data can be written.
		/// \param[out] indices A collection into which vector indexes for each
		///   triangle can be written.
		/// \post Each unique vertex in geometry has its data copied to data.
		/// \post indices contains the correct indices for each vertex to build
		///   triangles.
		/// This uses the two out parameters simply because we can't return two things.
		static void indexData(std::vector<float> const& geometry, unsigned int floatsPerVertex,
			std::vector<float>& data, std::vector<unsigned int>& indices)
		{
			unsigned int const VERTICES_PER_TRIANGLE = 3;
			float const EPSILON = 0.00001f;
			assert(geometry.size() % (floatsPerVertex * VERTICES_PER_TRIANGLE) == 0);
			// We must account for each vertex in the geometry vector.
			for (unsigned int geoIndex = 0; geoIndex < geometry.size() / floatsPerVertex; geoIndex++)
			{
				// Try to find a copy of it in the data vector.
				bool found = false;
				for (unsigned int dataIndex = 0; dataIndex < data.size() / floatsPerVertex && !found; dataIndex++)
				{
					bool matches = true;
					for (unsigned int part = 0; part < floatsPerVertex && matches; part++)
					{
						if (fabs(geometry[geoIndex * floatsPerVertex + part] -
							data[dataIndex * floatsPerVertex + part]) >= EPSILON)
						{
							matches = false;
						}
					}
					if (matches)
					{
						// Found it, just save that index!
						indices.push_back(dataIndex);
						found = true;
					}
				}
				if (!found)
				{
					// Didn't find it, so copy it to data vector and add new index.
					for (unsigned int part = 0; part < floatsPerVertex; part++)
					{
						data.push_back(geometry[geoIndex * floatsPerVertex + part]);
					}
					indices.push_back(data.size() / floatsPerVertex - 1);
				}
			}
		}

		/// \brief Computes a normal vector for each face of a mesh.
		/// \param[in] faces A collection of faces that are part of the mesh.
		/// \return A collection containing one normal vector per face.
		static std::vector<Vec3> computeFaceNormals(std::vector<Triangle> const& faces)
		{
			std::vector<Vec3> faceNormals;
			for (unsigned int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				Vec3 normal = (faces[faceIndex][1] - faces[faceIndex][0]).crossProduct(faces[faceIndex][2] - faces[faceIndex][0]);
				normal.normalise();
				faceNormals.push_back(normal);
			}
			return faceNormals;
		}

		/// \brief Computes a vertex normal for each vertex of a mesh.
		/// \param[in] faces A collection of faces that are part of the mesh.
		/// \param[in] faceNormals A collection of normal vectors for each face.
		/// \return A collection of normal vectors for each vertex.  This is the
		///   average of the face normals for each face that meets at that vertex,
		///   weighted by both the area of the face (larger faces have a larger weight)
		///   and by the angle at which the face meets the verrtex (larger angles have
		///   a larger weight).
		/// The same value will appear multiple times in the output collection, because
		///   there are (presumably) several faces meeting at the same vertex, and we
		///   are outputting a normal for each of the three vertices of each face.
		///   During indexing these will all be collapsed.
		static std::vector<Vec3> computeVertexNormals(std::vector<Triangle> const& faces,
			std::vector<Vec3> const& faceNormals)
		{
			assert(faces.size() == faceNormals.size());
			std::vector<Vec3> vertexNormals;
			for (unsigned int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				for (unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
				{
					// We need to find *every* vertex in any triangle that is at this
					//   position and average their face normals.
					Vec3 vertexNormal(0.0f, 0.0f, 0.0f);
					for (unsigned int otherFaceIndex = 0; otherFaceIndex < faces.size(); otherFaceIndex++)
					{
						for (unsigned int otherVertexIndex = 0; otherVertexIndex < 3; otherVertexIndex++)
						{
							if (faces[faceIndex][vertexIndex] == faces[otherFaceIndex][otherVertexIndex])
							{
								float area = 0.5f * ((faces[otherFaceIndex][1] - faces[otherFaceIndex][0]).crossProduct(faces[otherFaceIndex][2] - faces[otherFaceIndex][0])).length();
								unsigned int oppositeIndexA = (otherVertexIndex + 1) % 3;
								unsigned int oppositeIndexB = (otherVertexIndex + 2) % 3;
								float angle = float((faces[otherFaceIndex][oppositeIndexA] - faces[otherFaceIndex][otherVertexIndex]).angleBetween(faces[otherFaceIndex][oppositeIndexB] - faces[otherFaceIndex][otherVertexIndex]));
								// Weighting the average by area makes it so that lots of smaller
								//   faces don't overwhelm a few larger faces.
								// Weighting the average by angle makes it so that points where
								//   two 45 degree angles and points where one 90 degree angle meet
								//   get the same treatment.
								vertexNormal += faceNormals[otherFaceIndex] * fabs(area) * fabs(angle);
							}
						}
					}
					vertexNormal.normalise();
					vertexNormals.push_back(vertexNormal);
				}
			}
			return vertexNormals;
		}

		/// \brief Assigns a random color to each face of a mesh.
		/// \param[in] faces A collection of faces that are part of the mesh.
		/// \return A collection containing one color (R,G,B) per face.
		static std::vector<Vec3> generateRandomFaceColors(std::vector<Triangle> const& faces)
		{
			static std::default_random_engine generator;
			std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
			std::vector<Vec3> faceColors;
			for (unsigned int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				faceColors.push_back(Vec3(distribution(generator), distribution(generator), distribution(generator)));
			}
			return faceColors;
		}

		/// \brief Assigns a random color to each vertex of a mesh.
		/// \param[in] A collection of faces that are part of the mesh.
		/// \return A collection containing three colors per face.  When the same
		///   vertex is shared by multiple faces, each copy of the vertex will be
		///   assigned the same random color.
		static std::vector<Vec3> generateRandomVertexColors(std::vector<Triangle> const& faces)
		{
			static std::default_random_engine generator;
			std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
			std::vector<Vec3> vertexColors;
			for (unsigned int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				for (unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
				{
					// If we already assigned a color to that position, we need to copy it.
					bool foundMatch = false;
					for (unsigned int otherFaceIndex = 0; otherFaceIndex < faceIndex && !foundMatch; otherFaceIndex++)
					{
						for (unsigned int otherVertexIndex = 0; otherVertexIndex < 3; otherVertexIndex++)
						{
							if (faces[faceIndex][vertexIndex] == faces[otherFaceIndex][otherVertexIndex])
							{
								vertexColors.push_back(vertexColors[otherFaceIndex * 3 + otherVertexIndex]);
								foundMatch = true;
							}
						}
					}
					// If we never saw this position before, generate a new random color.
					if (!foundMatch)
					{
						vertexColors.push_back(Vec3(distribution(generator), distribution(generator), distribution(generator)));
					}
				}
			}
			return vertexColors;
		}

		/// \brief Assigns a random color to all faces of a mesh.
		/// \param[in] faces A collection of faces that are part of the mesh.
		/// \return A collection containing one color (R,G,B) for all faces.
		static std::vector<Vec3> generateRandomColors(std::vector<Triangle> const& faces)
		{
			static std::default_random_engine generator;
			std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
			return std::vector<Vec3>(faces.size(),
				Vec3(distribution(generator), distribution(generator), distribution(generator)));
		}

		/// \brief Produces a collection of interleaved position / color data from
		///   faces and face colors.
		/// \param[in] faces A collection of faces that are part of the mesh.
		/// \param[in] faceColors A collection of colors, one per face.
		/// \return A collection containing interleaved position / color data that is
		///   ready to be indexed / added to a Mesh.
		static std::vector<float> dataWithFaceColors(std::vector<Triangle> const& faces,
			std::vector<Vec3> const& faceColors)
		{
			assert(faces.size() == faceColors.size());
			std::vector<float> data;
			for (unsigned int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				for (unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
				{
					data.push_back(faces[faceIndex][vertexIndex].x);
					data.push_back(faces[faceIndex][vertexIndex].y);
					data.push_back(faces[faceIndex][vertexIndex].z);
					data.push_back(faceColors[faceIndex].x);
					data.push_back(faceColors[faceIndex].y);
					data.push_back(faceColors[faceIndex].z);
				}
			}
			return data;
		}

		/// \brief Produces a collection of interleaved position / color data from
		///   faces and vertex colors.
		/// \param[in] faces A collection of faces that are part of the mesh.
		/// \param[in] vertexColors A collection of colors, three per face.
		/// \return A collection containing interleaved position / color data that is
		///   ready to be indexed / added to a Mesh.
		static std::vector<float> dataWithVertexColors(std::vector<Triangle> const& faces,
			std::vector<Vec3> const& vertexColors)
		{
			assert(faces.size() * 3 == vertexColors.size());
			std::vector<float> data;
			for (unsigned int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				for (unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
				{
					data.push_back(faces[faceIndex][vertexIndex].x);
					data.push_back(faces[faceIndex][vertexIndex].y);
					data.push_back(faces[faceIndex][vertexIndex].z);
					data.push_back(vertexColors[faceIndex * 3 + vertexIndex].x);
					data.push_back(vertexColors[faceIndex * 3 + vertexIndex].y);
					data.push_back(vertexColors[faceIndex * 3 + vertexIndex].z);
				}
			}
			return data;
		}

		/// \brief Produces a collection of interleaved position / normal data from
		///   faces and face normals.
		/// \param[in] faces A collection of faces that are part of the mesh.
		/// \param[in] faceNormals A collection of normals, one per face.
		/// \return A collection containing interleaved position / normal data that is
		///   ready to be indexed / added to a Mesh.
		static std::vector<float> dataWithFaceNormals(std::vector<Triangle> const& faces,
			std::vector<Vec3> const& faceNormals)
		{
			assert(faces.size() == faceNormals.size());
			std::vector<float> data;
			for (unsigned int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				for (unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
				{
					data.push_back(faces[faceIndex][vertexIndex].x);
					data.push_back(faces[faceIndex][vertexIndex].y);
					data.push_back(faces[faceIndex][vertexIndex].z);
					data.push_back(faceNormals[faceIndex].x);
					data.push_back(faceNormals[faceIndex].y);
					data.push_back(faceNormals[faceIndex].z);
				}
			}
			return data;
		}

		/// \brief Produces a collection of interleaved position / normal data from
		///   faces and vertex normals.
		/// \param[in] faces A collection of faces that are part of the mesh.
		/// \param[in] vertexNormals A collection of normals, three per face.
		/// \return A collection containing interleaved position / normal data that is
		///   ready to be indexed / added to a Mesh.
		static std::vector<float> dataWithVertexNormals(std::vector<Triangle> const& faces,
			std::vector<Vec3> const& vertexNormals)
		{
			assert(faces.size() * 3 == vertexNormals.size());
			std::vector<float> data;
			for (unsigned int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				for (unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
				{
					data.push_back(faces[faceIndex][vertexIndex].x);
					data.push_back(faces[faceIndex][vertexIndex].y);
					data.push_back(faces[faceIndex][vertexIndex].z);
					data.push_back(vertexNormals[faceIndex * 3 + vertexIndex].x);
					data.push_back(vertexNormals[faceIndex * 3 + vertexIndex].y);
					data.push_back(vertexNormals[faceIndex * 3 + vertexIndex].z);
				}
			}
			return data;
		}

		/// \brief Produces a collection of interleaved position / normal / color data from
		///   faces, face normals and face colors.
		/// \param[in] faces A collection of faces that are part of the mesh.
		/// \param[in] faceNormals A collection of normals, one per face.
		/// /// \param[in] faceColors A collection of colors, one per face.
		/// \return A collection containing interleaved position / normal / color data that is
		///   ready to be indexed / added to a Mesh.
		static std::vector<float> dataWithFaceNormalsANDColors(std::vector<Triangle> const& faces,
			std::vector<Vec3> const& faceNormals, std::vector<Vec3> const& faceColors)
		{
			assert(faces.size() == faceNormals.size() && faces.size() == faceColors.size());
			std::vector<float> data;
			for (unsigned int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				for (unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
				{
					data.push_back(faces[faceIndex][vertexIndex].x);
					data.push_back(faces[faceIndex][vertexIndex].y);
					data.push_back(faces[faceIndex][vertexIndex].z);
					data.push_back(faceNormals[faceIndex].x);
					data.push_back(faceNormals[faceIndex].y);
					data.push_back(faceNormals[faceIndex].z);
					data.push_back(faceColors[faceIndex].x);
					data.push_back(faceColors[faceIndex].y);
					data.push_back(faceColors[faceIndex].z);
				}
			}
			return data;
		}

		/// \brief Creates a collection of triangles in a unit cube.
		/// \return A collection of triangles in a unit cube, centered on the origin.
		static std::vector<Triangle> buildCube()
		{
			std::vector<Triangle> triangles;
			// Front side (upper-left tri)
			triangles.push_back(Triangle{ Vec3(-0.5f, 0.5f, 0.5f), Vec3(-0.5f, -0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f) });
			// Front side (lower-right tri)
			triangles.push_back(Triangle{ Vec3(0.5f, -0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(-0.5f, -0.5f, 0.5f) });
			// Right side (upper-left tri)
			triangles.push_back(Triangle{ Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, -0.5f, 0.5f), Vec3(0.5f, 0.5f, -0.5f) });
			// Right side (lower-right tri)
			triangles.push_back(Triangle{ Vec3(0.5f, -0.5f, -0.5f), Vec3(0.5f, 0.5f, -0.5f), Vec3(0.5f, -0.5f, 0.5f) });
			// Back side (upper-left tri)
			triangles.push_back(Triangle{ Vec3(0.5f, 0.5f, -0.5f), Vec3(0.5f, -0.5f, -0.5f), Vec3(-0.5f, 0.5f, -0.5f) });
			// Back side (lower-right tri)
			triangles.push_back(Triangle{ Vec3(-0.5f, -0.5f, -0.5f), Vec3(-0.5f, 0.5f, -0.5f), Vec3(0.5f, -0.5f, -0.5f) });
			// Left side (upper-left tri)
			triangles.push_back(Triangle{ Vec3(-0.5f, 0.5f, -0.5f), Vec3(-0.5f, -0.5f, -0.5f), Vec3(-0.5f, 0.5f, 0.5f) });
			// Left side (lower-right tri)
			triangles.push_back(Triangle{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(-0.5f, 0.5f, 0.5f), Vec3(-0.5f, -0.5f, -0.5f) });
			// Top side (upper-left tri)
			triangles.push_back(Triangle{ Vec3(-0.5f, 0.5f, -0.5f), Vec3(-0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, -0.5f) });
			// Top side (lower-right tri)
			triangles.push_back(Triangle{ Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, -0.5f), Vec3(-0.5f, 0.5f, 0.5f) });
			// Bottom side (upper-left tri)
			triangles.push_back(Triangle{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.5f, -0.5f, 0.5f) });
			// Bottom side (lower-right tri)
			triangles.push_back(Triangle{ Vec3(0.5f, -0.5f, -0.5f), Vec3(0.5f, -0.5f, 0.5f), Vec3(-0.5f, -0.5f, -0.5f) });
			return triangles;
		}

		/// \brief Creates a collection of triangles in a sphere.
		/// \param subdivisions the number of triangles used.
		/// \return A collection of triangles in a sphere, centered on the origin.
		static std::vector<Triangle> buildSphere(int subdivisions)
		{
			float const X = 0.525731112119133606f;
			float const Z = 0.850650808352039932f;
			float const N = 0.0f;

			std::vector<Vec3> vertices =
			{
				Vec3(-X, N, Z), Vec3( X, N,  Z), Vec3(-X,  N, -Z), Vec3( X,  N, -Z),
				Vec3( N, Z, X), Vec3( N, Z, -X), Vec3( N, -Z,  X), Vec3( N, -Z, -X),
				Vec3( Z, X, N), Vec3(-Z, X,  N), Vec3( Z, -X,  N), Vec3(-Z, -X,  N)
			};

			std::vector<Triangle> triangles =
			{
				{vertices[ 0], vertices[ 1], vertices[ 4]},
				{vertices[ 0], vertices[ 4], vertices[ 9]},
				{vertices[ 9], vertices[ 4], vertices[ 5]},
				{vertices[ 4], vertices[ 8], vertices[ 5]},
				{vertices[ 4], vertices[ 1], vertices[ 8]},
				{vertices[ 8], vertices[ 1], vertices[10]},
				{vertices[ 8], vertices[10], vertices[ 3]},
				{vertices[ 5], vertices[ 8], vertices[ 3]},
				{vertices[ 5], vertices[ 3], vertices[ 2]},
				{vertices[ 2], vertices[ 3], vertices[ 7]},
				{vertices[ 7], vertices[ 3], vertices[10]},
				{vertices[ 7], vertices[10], vertices[ 6]},
				{vertices[ 7], vertices[ 6], vertices[11]},
				{vertices[11], vertices[ 6], vertices[ 0]},
				{vertices[ 0], vertices[ 6], vertices[ 1]},
				{vertices[ 6], vertices[10], vertices[ 1]},
				{vertices[ 9], vertices[11], vertices[ 0]},
				{vertices[ 9], vertices[ 2], vertices[11]},
				{vertices[ 9], vertices[ 5], vertices[ 2]},
				{vertices[ 7], vertices[11], vertices[ 2]}
			};

			std::vector<Triangle> sphereTriangles;
			for (auto const& tri : triangles)
			{
				// Note: Fix triangle facing direction issues. Not a good fix.
				std::vector<Triangle> subdividedTriangles = subdivisions % 2 == 0 ? subdivide(tri[0], tri[1], tri[2], subdivisions) : subdivide(tri[0], tri[2], tri[1], subdivisions);
				sphereTriangles.insert(sphereTriangles.end(), subdividedTriangles.begin(), subdividedTriangles.end());
			}

			return sphereTriangles;
		}

		/// \brief Creates a collection of triangles in a cylinder.
		/// \param segments the number of segments of the cylinder.
		/// \param height the height of the cylinder.
		/// \param radius the radius of the cylinder.
		/// \return A collection of triangles in a cylinder, centered on the origin.
		static std::vector<Triangle> buildCylinder(int segments, float height, float radius)
		{
			std::vector<Triangle> triangles;
			std::vector<Vec3> topVertices, bottomVertices;

			// Generate top and bottom vertices
			for (int i = 0; i < segments; ++i)
			{
				float theta = 2.0f * Math::PI * float(i) / float(segments);
				float x = radius * cos(theta);
				float z = radius * sin(theta);

				topVertices.push_back(Vec3(x, height / 2.0f, z));
				bottomVertices.push_back(Vec3(x, -height / 2.0f, z));
			}

			// Generate side faces
			for (int i = 0; i < segments; ++i)
			{
				int next = (i + 1) % segments;

				// Side triangle 1
				triangles.push_back( {topVertices[i], bottomVertices[next], bottomVertices[i] });

				// Side triangle 2
				triangles.push_back( {topVertices[i], topVertices[next], bottomVertices[next] });
			}

			// Generate top and bottom caps
			Vec3 topCenter(0.0f, height / 2.0f, 0.0f);
			Vec3 bottomCenter(0.0f, -height / 2.0f, 0.0f);

			for (int i = 0; i < segments; ++i)
			{
				int next = (i + 1) % segments;

				// Top cap
				triangles.push_back({ topCenter, topVertices[next], topVertices[i] });

				// Bottom cap
				triangles.push_back({ bottomCenter, bottomVertices[i], bottomVertices[next] });
			}

			return triangles;
		}

		/// \brief Creates a collection of triangles in a cone.
		/// \param segments the number of segments of the cone.
		/// \param height the height of the cone.
		/// \param radius the radius of the cone.
		/// \return A collection of triangles in a cone, centered on the origin.
		static std::vector<Triangle> buildCone(int segments, float height, float radius)
		{
			std::vector<Triangle> triangles;
			std::vector<Vec3> baseVertices;

			Vec3 apex(0.0f, height / 2.0f, 0.0f);
			Vec3 baseCenter(0.0f, -height / 2.0f, 0.0f);

			// Generate base vertices
			for (int i = 0; i < segments; ++i)
			{
				float theta = 2.0f * Math::PI * float(i) / float(segments);
				float x = radius * cos(theta);
				float z = radius * sin(theta);

				baseVertices.push_back(Vec3(x, -height / 2.0f, z));
			}

			// Generate side faces
			for (int i = 0; i < segments; ++i)
			{
				int next = (i + 1) % segments;

				// Side triangle
				triangles.push_back({ apex, baseVertices[next], baseVertices[i] });
			}

			// Generate base cap
			for (int i = 0; i < segments; ++i)
			{
				int next = (i + 1) % segments;

				// Base triangle
				triangles.push_back({ baseCenter, baseVertices[i], baseVertices[next] });
			}

			return triangles;
		}

		/// \brief Creates a collection of triangles in a torus.
		/// \param majorSegments the number of major segments of the torus.
		/// \param minorSegments the height of minor segments the torus.
		/// \param majorRadius the major radius of the torus.
		/// \param minorRadius the minor radius of the torus.
		/// \return A collection of triangles in a torus, centered on the origin.
		static std::vector<Triangle> buildTorus(int majorSegments, int minorSegments, float majorRadius, float minorRadius)
		{
			std::vector<Triangle> triangles;

			for (int i = 0; i < majorSegments; ++i)
			{
				float theta1 = 2.0f * Math::PI * float(i) / float(majorSegments);
				float theta2 = 2.0f * Math::PI * float(i + 1) / float(majorSegments);

				for (int j = 0; j < minorSegments; ++j)
				{
					float phi1 = 2.0f * Math::PI * float(j) / float(minorSegments);
					float phi2 = 2.0f * Math::PI * float(j + 1) / float(minorSegments);

					Vec3 p1((majorRadius + minorRadius * cos(phi1)) * cos(theta1),
						minorRadius * sin(phi1),
						(majorRadius + minorRadius * cos(phi1)) * sin(theta1));

					Vec3 p2((majorRadius + minorRadius * cos(phi2)) * cos(theta1),
						minorRadius * sin(phi2),
						(majorRadius + minorRadius * cos(phi2)) * sin(theta1));

					Vec3 p3((majorRadius + minorRadius * cos(phi2)) * cos(theta2),
						minorRadius * sin(phi2),
						(majorRadius + minorRadius * cos(phi2)) * sin(theta2));

					Vec3 p4((majorRadius + minorRadius * cos(phi1)) * cos(theta2),
						minorRadius * sin(phi1),
						(majorRadius + minorRadius * cos(phi1)) * sin(theta2));

					// Triangle 1
					triangles.push_back(Triangle{ p1, p2, p3 });

					// Triangle 2
					triangles.push_back(Triangle{ p1, p3, p4 });
				}
			}

			return triangles;
		}

		/// \brief Creates a collection of triangles in a pyramid.
		/// \param baseSize the base size of the pyramid.
		/// \param height the height of the pyramid.
		/// \return A collection of triangles in a pyramid, centered on the origin.
		static std::vector<Triangle> buildPyramid(float baseSize, float height)
		{
			std::vector<Triangle> triangles;

			Vec3 apex(0.0f, height / 2.0f, 0.0f);
			Vec3 baseVertices[4] =
			{
				Vec3(-baseSize / 2.0f, -height / 2.0f, -baseSize / 2.0f),
				Vec3( baseSize / 2.0f, -height / 2.0f, -baseSize / 2.0f),
				Vec3( baseSize / 2.0f, -height / 2.0f,  baseSize / 2.0f),
				Vec3(-baseSize / 2.0f, -height / 2.0f,  baseSize / 2.0f)
			};

			// Side faces
			for (int i = 0; i < 4; ++i)
			{
				int next = (i + 1) % 4;
				triangles.push_back({ apex, baseVertices[next], baseVertices[i] });
			}

			// Base face
			triangles.push_back({ baseVertices[0], baseVertices[1], baseVertices[2] });
			triangles.push_back({ baseVertices[0], baseVertices[2], baseVertices[3] });

			return triangles;
		}

	private:
		// helper to generate subdivision for sphere
		static std::vector<Triangle> subdivide(Vec3 const& v1, Vec3 const& v2, Vec3 const& v3, int depth)
		{
			if (depth == 0)
			{
				return { { v1, v2, v3 } }; // Swap v2 and v3 to change the winding order
			}
			else
			{
				Vec3 v12 = (v1 + v2);
				v12.normalise();
				Vec3 v23 = (v2 + v3);
				v23.normalise();
				Vec3 v31 = (v3 + v1);
				v31.normalise();

				std::vector<Triangle> triangles;
				auto t1 = subdivide( v1, v31, v12, depth - 1);
				auto t2 = subdivide( v2, v12, v23, depth - 1);
				auto t3 = subdivide( v3, v23, v31, depth - 1);
				auto t4 = subdivide(v12, v31, v23, depth - 1);

				triangles.insert(triangles.end(), t1.begin(), t1.end());
				triangles.insert(triangles.end(), t2.begin(), t2.end());
				triangles.insert(triangles.end(), t3.begin(), t3.end());
				triangles.insert(triangles.end(), t4.begin(), t4.end());

				return triangles;
			}
		}
	};
}