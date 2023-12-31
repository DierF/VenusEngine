#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Core/Scene.h"
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

		static void activeMeshWindow(Scene& scene)
		{
			ImGui::Begin("Active Mesh");
			if (scene.hasActiveMesh())
			{
				ImGui::Text("Name: %s", scene.activeMeshName().c_str());

				Transform& transform = scene.getActiveMesh()->getTransform();
				ImGui::Text("Translation:");
				ImGui::SliderFloat("X##TranslateX", &transform.m_position.x, -100.0f, 100.0f);
				ImGui::SliderFloat("Y##TranslateY", &transform.m_position.y, -100.0f, 100.0f);
				ImGui::SliderFloat("Z##TranslateZ", &transform.m_position.z, -100.0f, 100.0f);
			
				ImGui::Text("Rotation:");
				Radian deltaAngle(0.1f);
				Radian angleX(0.0f);
				ImGui::Button("X--");
				if (ImGui::IsItemActive())
				{
					angleX -= deltaAngle;
				}
				ImGui::SameLine();
				ImGui::Button("X++");
				if (ImGui::IsItemActive())
				{
					angleX += deltaAngle;
				}
				transform.m_rotation = Quaternion(angleX, Vec3::UNIT_X) * transform.m_rotation;
				Radian angleY(0.0f);
				ImGui::Button("Y--");
				if (ImGui::IsItemActive())
				{
					angleY -= deltaAngle;
				}
				ImGui::SameLine();
				ImGui::Button("Y++");
				if (ImGui::IsItemActive())
				{
					angleY += deltaAngle;
				}
				transform.m_rotation = Quaternion(angleY, Vec3::UNIT_Y) * transform.m_rotation;
				Radian angleZ(0.0f);
				ImGui::Button("Z--");
				if (ImGui::IsItemActive())
				{
					angleZ -= deltaAngle;
				}
				ImGui::SameLine();
				ImGui::Button("Z++");
				if (ImGui::IsItemActive())
				{
					angleZ += deltaAngle;
				}
				transform.m_rotation = Quaternion(angleZ, Vec3::UNIT_Z) * transform.m_rotation;
				if (ImGui::Button("Reset Rotation"))
				{
					transform.m_rotation = Quaternion::IDENTITY;
				}
				
				ImGui::Text("Scale:");
				float scale = transform.m_scale.x;
				ImGui::SliderFloat("##Scale", &scale, 0.1f, 10.0f);
				transform.m_scale.x = scale;
				transform.m_scale.y = scale;
				transform.m_scale.z = scale;

				if (ImGui::Button(("Delete##Delete")))
				{
					scene.remove(scene.activeMeshName());
				}
			}

			ImGui::End();
		}

		static std::pair<bool, std::string> allMeshWindow(std::string const& activeMeshName, std::vector<std::string> const& allMeshNames)
		{
			std::string selectedMeshName;
			ImGui::Begin("All Meshes");
			// Display a list of mesh names
			for (auto const& meshName : allMeshNames)
			{
				// Use a different color for the active mesh
				if (meshName == activeMeshName)
				{
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s (Active)", meshName.c_str());
				}
				else
				{
					ImGui::Text("%-50s", meshName.c_str());
					if (ImGui::IsItemClicked())
					{
						selectedMeshName = meshName;
					}
				}
			}
			ImGui::End();
			return { !selectedMeshName.empty(), selectedMeshName};
		}

		static std::pair<float, float> viewportWindow(uint64_t textureId)
		{
			ImGui::Begin("Viewport");
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset    = ImGui::GetWindowPos();
			//ImVec2 viewportBounds[2];
			//viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			//viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

			//bool viewportFocused = ImGui::IsWindowFocused();
			//bool viewportHovered = ImGui::IsWindowHovered();

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			ImVec2 viewportSize    = { viewportPanelSize.x, viewportPanelSize.y };

			ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();
			return { viewportSize.x, viewportSize.y };
		}
	};
}