#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Core/Scene.h"
#include "Core/SceneLight.h"
#include "Core/Geometry.h"
#include "Core/KeyBuffer.h"
#include "Editor/Window.h"
#include "Math/Transform.h"

namespace VenusEngine
{
	class Gui
	{
	public:
		static void init(GLFWwindow* window)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 330");

			static bool dockspaceOpen = true;
		}

		static void destroy()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		static void newFrame()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGuizmo::BeginFrame();

			static bool dockspaceOpen = true;
		}

		static void begin(char const* name)
		{
			ImGui::Begin(name);
		}

		static void end()
		{
			ImGui::End();
		}

		static void draw()
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		static void beginDockspace()
		{
			static bool dockspaceOpen = true;
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			{
				window_flags |= ImGuiWindowFlags_NoBackground;
			}
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();
			if (opt_fullscreen)
			{
				ImGui::PopStyleVar(2);
			}
			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			float minWinSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 370.0f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			style.WindowMinSize.x = minWinSizeX;
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open Project..."))
					{
					}
					ImGui::Separator();
					if (ImGui::MenuItem("New Scene"))
					{
					}
					if (ImGui::MenuItem("Save Scene"))
					{
					}
					if (ImGui::MenuItem("Save Scene As..."))
					{
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit", "Esc"))
					{
						Window::get().closeWindow();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
		}

		static void endDockspace()
		{
			ImGui::End();
		}

		static void gizmo(std::pair<float, float> viewportPos, std::pair<float, float> viewportSize, float const* view, float const* projection, float* transform)
		{
			static enum Mode
			{
				NONE = 0, TRANSLATE, ROTATE, SCALE
			} mode = Mode::NONE;

			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(viewportPos.first, viewportPos.second, viewportSize.first, viewportSize.second);

			if (KeyBuffer::getPressedKey(GLFW_KEY_W))
			{
				mode = Mode::TRANSLATE;
			}
			if (KeyBuffer::getPressedKey(GLFW_KEY_E))
			{
				mode = Mode::ROTATE;
			}
			if (KeyBuffer::getPressedKey(GLFW_KEY_R))
			{
				mode = Mode::SCALE;
			}
			if (KeyBuffer::getPressedKey(GLFW_KEY_Q))
			{
				mode = Mode::NONE;
			}

			switch (mode)
			{
			case TRANSLATE:
				ImGuizmo::Manipulate(view, projection, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, transform);
				break;
			case ROTATE:
				ImGuizmo::Manipulate(view, projection, ImGuizmo::ROTATE, ImGuizmo::LOCAL, transform);
				break;
			case SCALE:
				ImGuizmo::Manipulate(view, projection, ImGuizmo::SCALE, ImGuizmo::LOCAL, transform);
				break;
			case NONE:
			default:
				break;
			}
		}

		static bool gizmoIsUsing()
		{
			return ImGuizmo::IsUsing();
		}

		static bool gizmoIsOver()
		{
			return ImGuizmo::IsOver();
		}

		static void activeObjectWindow(Scene& scene, SceneLight& sceneLight)
		{
			ImGui::Begin("Active Object");

			float totalWidth = ImGui::GetContentRegionAvail().x;

			//ImGuiStyle& style = ImGui::GetStyle();
			//ImVec2 itemSpacing = style.ItemInnerSpacing;
			// Note: I have not find the best way to fit this to inputFloat3 itemspacing
			float itemSpacing = 4.0f;

			if (scene.hasActiveMesh())
			{
				// Information about Acitve Mesh
				ImGui::Text("Name:");
				char buffer[256];
				std::strncpy(buffer, scene.activeMeshName().c_str(), sizeof(buffer));
				ImGui::PushItemWidth(totalWidth);
				if (ImGui::InputText("##", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue) &&
					std::strlen(buffer) > 0)
				{
					scene.changeActiveMeshName(buffer);
				}
				ImGui::PopItemWidth();

				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				ImGui::Text("ID:");
				ImGui::Text(std::to_string(scene.getActiveMesh()->getID()).c_str());

				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				Transform& transform = scene.getActiveMesh()->getTransform();

				ImGui::Text("Translation:");
				ImGui::PushItemWidth(totalWidth);
				ImGui::InputFloat3("##Translation", transform.m_position.ptr(),
					"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
				ImGui::PopItemWidth();
				if (ImGui::Button("Reset Translation"))
				{
					transform.m_position = Vec3::ZERO;
				}

				ImGui::Dummy(ImVec2(0.0f, 5.0f));
			
				ImGui::Text("Rotation:");
				// Rotation in world coordinates
				Radian deltaAngle(0.05f);
				Radian angleX = transform.m_rotation.getPitch();
				Radian angleY = transform.m_rotation.getYaw();
				Radian angleZ = transform.m_rotation.getRoll();
				// Update only when changed.
				// Note: Coversion between Quaternion and Euler angle makes UI not straight forward
				ImGui::PushItemWidth((totalWidth - 2.0f * itemSpacing) / 3.0f);
				bool changedX = ImGui::InputFloat("##RotationX", angleX.ptr(), 0.0f, 0.0f,
					"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
				ImGui::SameLine(0.0f, itemSpacing);
				bool changedY = ImGui::InputFloat("##RotationY", angleY.ptr(), 0.0f, 0.0f,
					"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
				ImGui::SameLine(0.0f, itemSpacing);
				bool changedZ = ImGui::InputFloat("##RotationZ", angleZ.ptr(), 0.0f, 0.0f,
					"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
				ImGui::PopItemWidth();
				if (changedX || changedY || changedZ)
				{
					transform.m_rotation.fromYawPitchRoll(angleY, angleX, angleZ);
				}
				if (ImGui::Button("Reset Rotation"))
				{
					transform.m_rotation = Quaternion::IDENTITY;
				}

				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				
				ImGui::Text("Scale:");
				ImGui::PushItemWidth(totalWidth);
				ImGui::InputFloat3("##Scale", transform.m_scale.ptr(),
					"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
				ImGui::PopItemWidth();
				if (ImGui::Button("Reset Scale"))
				{
					transform.m_scale = Vec3::UNIT_SCALE;
				}

				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				ImGui::Text("Color:");
				float* color = scene.getActiveMesh()->getFirstColorPtr();
				ImGui::PushItemWidth(totalWidth);
				ImGui::ColorPicker3("##Color", color, ImGuiColorEditFlags_NoSidePreview);
				ImGui::PopItemWidth();
				scene.getActiveMesh()->resetColorToFirst();

				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				if (ImGui::Button(("Delete##Delete")))
				{
					scene.remove(scene.activeMeshName());
				}
			}
			else if (sceneLight.hasActiveLightSource())
			{
				// Information about Active Light
				ImGui::Text("Name:");
				char buffer[32];
				std::strncpy(buffer, sceneLight.activeLightSourceName().c_str(), sizeof(buffer));
				ImGui::PushItemWidth(totalWidth);
				if (ImGui::InputText("##", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue) &&
					std::strlen(buffer) > 0)
				{
					sceneLight.changeActiveLightSourceName(buffer);
				}
				ImGui::PopItemWidth();

				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				std::shared_ptr<LightSource> activeLightPtr = sceneLight.getActiveLightSource();

				if (dynamic_cast<DirectionalLightSource*>(activeLightPtr.get()))
				{
					ImGui::Text("Light Type:");
					ImGui::Text("Directional");

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					DirectionalLightSource* specificLightPtr = dynamic_cast<DirectionalLightSource*>(activeLightPtr.get());

					ImGui::Text("DiffuseIntensity:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat3("##DiffuseIntensity:", specificLightPtr->getDiffuseIntensity().ptr(),
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("SpecularIntensity:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat3("##SpecularIntensity:", specificLightPtr->getSpecularIntensity().ptr(),
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("Direction:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat3("##Direction:", specificLightPtr->getDirection().ptr(),
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();
				}
				else if (dynamic_cast<PointLightSource*>(activeLightPtr.get()))
				{
					ImGui::Text("Light Type:");
					ImGui::Text("Point");

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					PointLightSource* specificLightPtr = dynamic_cast<PointLightSource*>(activeLightPtr.get());

					ImGui::Text("DiffuseIntensity:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat3("##DiffuseIntensity:", specificLightPtr->getDiffuseIntensity().ptr(),
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("SpecularIntensity:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat3("##SpecularIntensity:", specificLightPtr->getSpecularIntensity().ptr(),
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("Position:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat3("##Position:", specificLightPtr->getPosition().ptr(),
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("AttenuationCoefficients:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat3("##AttenuationCoefficients:", specificLightPtr->getAttenuationCoefficients().ptr(),
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();
				}
				else if (dynamic_cast<SpotLightSource*>(activeLightPtr.get()))
				{
					ImGui::Text("Light Type:");
					ImGui::Text("Spot");

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					SpotLightSource* specificLightPtr = dynamic_cast<SpotLightSource*>(activeLightPtr.get());

					ImGui::Text("DiffuseIntensity:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat3("##DiffuseIntensity:", specificLightPtr->getDiffuseIntensity().ptr(),
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("SpecularIntensity:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat3("##SpecularIntensity:", specificLightPtr->getSpecularIntensity().ptr(),
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("Position:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat3("##Position:", specificLightPtr->getPosition().ptr(),
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("AttenuationCoefficients:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat3("##AttenuationCoefficients:", specificLightPtr->getAttenuationCoefficients().ptr(),
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("Direction:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat3("##Direction:", specificLightPtr->getDirection().ptr(),
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("cutoffCosAngle:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat("##cutoffCosAngle", &specificLightPtr->getCutoffCosAngle(), 0.0f, 0.0f,
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("Falloff:");
					ImGui::PushItemWidth(totalWidth);
					ImGui::InputFloat("##Falloff", &specificLightPtr->getFalloff(), 0.0f, 0.0f,
						"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopItemWidth();
				}

				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				if (ImGui::Button(("Delete##Delete")))
				{
					sceneLight.remove(sceneLight.activeLightSourceName());
				}
			}

			ImGui::End();
		}

		// return { isMeshSelected, selectedObjectName }
		// 0 for Light, 1 for Mesh
		static std::pair<bool, std::string> allObjectWindow(Scene& scene, SceneLight& sceneLight)
		{
			bool isMeshSelected;
			std::string selectedObjectName;

			ImGui::Begin("All Objects");

			float totalWidth = ImGui::GetContentRegionAvail().x;

			//ImGuiStyle& style = ImGui::GetStyle();
			//ImVec2 itemSpacing = style.ItemInnerSpacing;
			// Note: I have not find the best way to fit this to inputFloat3 itemspacing
			float itemSpacing = 4.0f;

			ImGui::Text("Add an Object:");

			ImGui::Text("Cube:");
			if (ImGui::Button("Add Cube"))
			{
				int index = 1;
				auto name = std::string("Cube");
				while (scene.hasMesh(name + std::to_string(index)))
				{
					++index;
				}
				name += std::to_string(index);
				auto faces = Geometry::buildCube();
				auto geometry = Geometry::dataWithFaceNormalsANDColors(faces,
								Geometry::computeFaceNormals(faces),
								Geometry::generateRandomColors(faces));
				std::shared_ptr<Mesh> cube_mesh_ptr(new Mesh());
				cube_mesh_ptr->addGeometry(geometry);
				cube_mesh_ptr->prepareVao();
				scene.add(name, cube_mesh_ptr);
			}

			static int sphereSubdivisions = 2;
			ImGui::Text("Sphere: (Subdivision Depth)");
			ImGui::PushItemWidth(totalWidth);
			ImGui::InputInt("##Sphere Subdivision Depth", &sphereSubdivisions, 0, 0,
				ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::PopItemWidth();
			if (ImGui::Button("Add Sphere"))
			{
				int index = 1;
				auto name = std::string("Sphere");
				while (scene.hasMesh(name + std::to_string(index)))
				{
					++index;
				}
				name += std::to_string(index);
				auto faces = Geometry::buildSphere(sphereSubdivisions);
				auto geometry = Geometry::dataWithFaceNormalsANDColors(faces,
					Geometry::computeFaceNormals(faces),
					Geometry::generateRandomColors(faces));
				std::shared_ptr<Mesh> sphere_mesh_ptr(new Mesh());
				sphere_mesh_ptr->addGeometry(geometry);
				sphere_mesh_ptr->prepareVao();
				scene.add(name, sphere_mesh_ptr);
			}

			static int cylinderSegments = 50;
			static float cylinderHeight = 2.0f;
			static float cylinderRadius = 1.0f;
			ImGui::Text("Cylinder: (Segment/Height/Radius)");
			ImGui::PushItemWidth((totalWidth - 2.0f * itemSpacing) / 3.0f);
			ImGui::InputInt("##Cylinder Segment", &cylinderSegments, 0, 0,
				ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::SameLine(0.0f, itemSpacing);
			ImGui::InputFloat("##Cylinder Height", &cylinderHeight, 0.0f, 0.0f,
				"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::SameLine(0.0f, itemSpacing);
			ImGui::InputFloat("##Cylinder Radius", &cylinderRadius, 0.0f, 0.0f,
				"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::PopItemWidth();
			if (ImGui::Button("Add Cylinder"))
			{
				int index = 1;
				auto name = std::string("Cylinder");
				while (scene.hasMesh(name + std::to_string(index)))
				{
					++index;
				}
				name += std::to_string(index);
				auto faces = Geometry::buildCylinder(cylinderSegments, cylinderHeight, cylinderRadius);
				auto geometry = Geometry::dataWithFaceNormalsANDColors(faces,
					Geometry::computeFaceNormals(faces),
					Geometry::generateRandomColors(faces));
				std::shared_ptr<Mesh> cylinder_mesh_ptr(new Mesh());
				cylinder_mesh_ptr->addGeometry(geometry);
				cylinder_mesh_ptr->prepareVao();
				scene.add(name, cylinder_mesh_ptr);
			}

			static int coneSegments = 50;
			static float coneHeight = 2.0f;
			static float coneRadius = 1.0f;
			ImGui::Text("Cone: (Segment/Height/Radius)");
			ImGui::PushItemWidth((totalWidth - 2.0f * itemSpacing) / 3.0f);
			ImGui::InputInt("##Cone Segment", &coneSegments, 0, 0,
				ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::SameLine(0.0f, itemSpacing);
			ImGui::InputFloat("##Cone Height", &coneHeight, 0.0f, 0.0f,
				"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::SameLine(0.0f, itemSpacing);
			ImGui::InputFloat("##Cone Radius", &coneRadius, 0.0f, 0.0f,
				"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::PopItemWidth();
			if (ImGui::Button("Add Cone"))
			{
				int index = 1;
				auto name = std::string("Cone");
				while (scene.hasMesh(name + std::to_string(index)))
				{
					++index;
				}
				name += std::to_string(index);
				auto faces = Geometry::buildCone(coneSegments, coneHeight, coneRadius);
				auto geometry = Geometry::dataWithFaceNormalsANDColors(faces,
					Geometry::computeFaceNormals(faces),
					Geometry::generateRandomColors(faces));
				std::shared_ptr<Mesh> cone_mesh_ptr(new Mesh());
				cone_mesh_ptr->addGeometry(geometry);
				cone_mesh_ptr->prepareVao();
				scene.add(name, cone_mesh_ptr);
			}

			static int torusMajorSegments = 50;
			static int torusMinorSegments = 50;
			static float torusMajorRadius = 1.5f;
			static float torusMinorRadius = 0.5f;
			ImGui::Text("Torus: (Major Segment/Major Radius)");
			ImGui::PushItemWidth((totalWidth - 1.0f * itemSpacing) / 2.0f);
			ImGui::InputInt("##Torus Major Segment", &torusMajorSegments, 0, 0,
				ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::SameLine(0.0f, itemSpacing);
			ImGui::InputFloat("##Torus Major Radius", &torusMajorRadius, 0.0f, 0.0f,
				"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::PopItemWidth();
			ImGui::Text("       (Minor Segment/Minor Radius)");
			ImGui::PushItemWidth((totalWidth - 1.0f * itemSpacing) / 2.0f);
			ImGui::InputInt("##Torus Minor Segment", &torusMinorSegments, 0, 0,
				ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::SameLine(0.0f, itemSpacing);
			ImGui::InputFloat("##Torus Minor Radius", &torusMinorRadius, 0.0f, 0.0f,
				"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			if (ImGui::Button("Add Torus"))
			{
				int index = 1;
				auto name = std::string("Torus");
				while (scene.hasMesh(name + std::to_string(index)))
				{
					++index;
				}
				name += std::to_string(index);
				auto faces = Geometry::buildTorus(torusMajorSegments, torusMinorSegments,
					torusMajorRadius, torusMinorRadius);
				auto geometry = Geometry::dataWithFaceNormalsANDColors(faces,
					Geometry::computeFaceNormals(faces),
					Geometry::generateRandomColors(faces));
				std::shared_ptr<Mesh> torus_mesh_ptr(new Mesh());
				torus_mesh_ptr->addGeometry(geometry);
				torus_mesh_ptr->prepareVao();
				scene.add(name, torus_mesh_ptr);
			}

			static float pyramidHeight = 2.0f;
			static float pyramidRadius = 1.0f;
			ImGui::Text("Pyramid: (Height/Radius)");
			ImGui::PushItemWidth((totalWidth - 1.0f * itemSpacing) / 2.0f);
			ImGui::InputFloat("##Pyramid Height", &pyramidHeight, 0.0f, 0.0f,
				"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::SameLine(0.0f, itemSpacing);
			ImGui::InputFloat("##Pyramid Radius", &pyramidRadius, 0.0f, 0.0f,
				"%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			if (ImGui::Button("Add Pyramid"))
			{
				int index = 1;
				auto name = std::string("Pyramid");
				while (scene.hasMesh(name + std::to_string(index)))
				{
					++index;
				}
				name += std::to_string(index);
				auto faces = Geometry::buildPyramid(pyramidHeight, pyramidRadius);
				auto geometry = Geometry::dataWithFaceNormalsANDColors(faces,
					Geometry::computeFaceNormals(faces),
					Geometry::generateRandomColors(faces));
				std::shared_ptr<Mesh> pyramid_mesh_ptr(new Mesh());
				pyramid_mesh_ptr->addGeometry(geometry);
				pyramid_mesh_ptr->prepareVao();
				scene.add(name, pyramid_mesh_ptr);
			}
			
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			if (ImGui::Button("Add Directional Light"))
			{
				int index = 1;
				auto name = std::string("Directional");
				while (sceneLight.hasLightSource(name + std::to_string(index)))
				{
					++index;
				}
				name += std::to_string(index);
				// Diffuse intensity of the light source
				Vec3 diffuseIntensity(1.0f, 1.0f, 1.0f);
				// Specular intensity of the light source
				Vec3 specularIntensity(0.8f, 0.8f, 0.8f);
				// Direction of the light source in 3D space
				Vec3 direction(0.0f, 0.0f, -1.0f);
				std::shared_ptr<DirectionalLightSource> directional_light_ptr(
					new DirectionalLightSource(diffuseIntensity, specularIntensity, direction));
				if (!sceneLight.add(name, directional_light_ptr))
				{
					std::cout << "Reached light source number limits!" << std::endl;
				}
			}

			if (ImGui::Button("Add Point Light"))
			{
				int index = 1;
				auto name = std::string("Point");
				while (sceneLight.hasLightSource(name + std::to_string(index)))
				{
					++index;
				}
				name += std::to_string(index);
				// Diffuse intensity of the light source
				Vec3 diffuseIntensity(2.0f, 1.8f, 1.6f);
				// Specular intensity of the light source
				Vec3 specularIntensity(2.0f, 2.0f, 2.0f);
				// Position of the light source in 3D space
				Vec3 position(0.0f, 10.0f, 0.0f);
				Vec3 attenuationCoefficients(1.0f, 0.07f, 0.017f);
				std::shared_ptr<PointLightSource> point_light_ptr(
					new PointLightSource(diffuseIntensity, specularIntensity, position,
						attenuationCoefficients));
				if (!sceneLight.add(name, point_light_ptr))
				{
					std::cout << "Reached light source number limits!" << std::endl;
				}
			}

			if (ImGui::Button("Add Spot Light"))
			{
				int index = 1;
				auto name = std::string("Spot");
				while (sceneLight.hasLightSource(name + std::to_string(index)))
				{
					++index;
				}
				name += std::to_string(index);
				// Diffuse intensity of the light source
				Vec3 diffuseIntensity(2.0f, 1.8f, 1.6f);
				// Specular intensity of the light source
				Vec3 specularIntensity(2.0f, 2.0f, 2.0f);
				// Position of the light source in 3D space
				Vec3 position(0.0f, 5.0f, 0.0f);
				Vec3 attenuationCoefficients(1.0f, 0.07f, 0.017f);
				Vec3 direction(0.0f, -1.0f, 0.0f);
				float cutoffCosAngle = Math::cos(Radian(12.5f));
				float falloff = 1.0f;
				std::shared_ptr<SpotLightSource> spot_light_ptr(
					new SpotLightSource(diffuseIntensity, specularIntensity, position,
						attenuationCoefficients, direction, cutoffCosAngle, falloff));
				if (!sceneLight.add(name, spot_light_ptr))
				{
					std::cout << "Reached light source number limits!" << std::endl;
				}
			}

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			// Display Meshes
			ImGui::Text("All Meshes:");

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			for (auto const& meshName : scene.allMeshNames())
			{
				// Use a different color for the active mesh
				if (meshName == scene.activeMeshName())
				{
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s (Active)", meshName.c_str());
				}
				else
				{
					ImGui::Text("%-50s", meshName.c_str());
					if (ImGui::IsItemClicked())
					{
						isMeshSelected = true;
						selectedObjectName = meshName;
					}
				}
			}

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			// Display Lights
			ImGui::Text("All Lights: (Maximum: 16)");

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			for (auto const& lightName : sceneLight.allLightSourceNames())
			{
				// Use a different color for the active light
				if (lightName == sceneLight.activeLightSourceName())
				{
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s (Active)", lightName.c_str());
				}
				else
				{
					ImGui::Text("%-50s", lightName.c_str());
					if (ImGui::IsItemClicked())
					{
						isMeshSelected = false;
						selectedObjectName = lightName;
					}
				}
			}

			ImGui::End();
			
			return { isMeshSelected, selectedObjectName };
		}

		static std::tuple<bool, std::pair<float, float>, std::pair<float, float>, float> viewportWindow(Scene const& scene, uint64_t textureId, float const* view, float const* projection, float* transform)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Viewport");

			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			
			bool focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			ImVec2 viewportPos = ImGui::GetWindowPos();

			// Calculate the tab bar height
			float tabBarHeight = 0.0f;
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->DockNode)
			{
				ImRect windowRect = window->Rect();
				ImRect contentRect = window->ContentRegionRect;
				float windowContentHeight = contentRect.GetHeight();
				tabBarHeight = windowRect.GetHeight() - windowContentHeight;
			}

			if (focused && scene.hasActiveMesh())
			{
				gizmo({ viewportPos.x, viewportPos.y }, { viewportSize.x, viewportSize.y }, view, projection, transform);
			}

			ImGui::End();
			ImGui::PopStyleVar();

			// the size and pos include the title bar of the window
			return { focused, { viewportSize.x, viewportSize.y }, { viewportPos.x, viewportPos.y }, tabBarHeight };
		}
	};
}