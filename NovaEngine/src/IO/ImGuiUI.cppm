module;
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glfw3.h"
export module ImGuiUI;
#include "Core.h"

import std;
import NSL;

export namespace IO
{
	class NSL_API ImGuiUI
	{
	public:
		ImGuiUI() NSL_NOEXCEPT
			: _context(nullptr)
			, _scale(1.0f)
		{
		}

	public:
		void Init(GLFWwindow* window) NSL_NOEXCEPT
		{
			_context = ImGui::CreateContext();
			float rounding = 6.0f;
			ImGuiStyle& style = ImGui::GetStyle();
			//style.WindowPadding = ImVec2(16.0f, 10.0f);
			//style.FramePadding = ImVec2(16.0f, 6.0f);
			//style.ItemSpacing = ImVec2(20.0f, 6.0f);
			//style.ScrollbarSize = 20.0f;
			//style.WindowRounding = rounding;
			//style.ChildRounding = rounding;
			//style.FrameRounding = rounding;
			//style.PopupRounding = rounding;
			//style.ScrollbarRounding = rounding;
			//style.GrabRounding = rounding;
			//style.TabRounding = rounding;
			//style.SeparatorTextBorderSize = 5.0f;
			//style.WindowMenuButtonPosition = ImGuiDir_None;

			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
			//ImGui::GetIO().IniFilename = "imgui.ini";
			
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init(/*"#version 460"*/);
			LogInfo("ImGui initialized successfully!");
		}
		void Shutdown() NSL_NOEXCEPT
		{
			LogInfo("Starting to shutdown ImGui...");
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext(_context);
			LogInfo("ImGui context destruction is completed");
		}
		void SetScale(float scale) NSL_NOEXCEPT
		{
			_scale = scale;
			ImGui::GetIO().FontGlobalScale = scale;
			ImGui::GetStyle().ScaleAllSizes(scale);
		}

		void StartRender() NSL_NOEXCEPT
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();
			ImGui::DockSpaceOverViewport(0, 0, ImGuiDockNodeFlags_PassthruCentralNode);
		}
		void EndRender() NSL_NOEXCEPT
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		void LoadFont(const std::string& fontPath, float size = 16.0f) NSL_NOEXCEPT
		{
			ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath.c_str(), size);
		}

		void RenderDemoWindow() NSL_NOEXCEPT
		{
			ImGui::ShowDemoWindow();
		}
		void RenderViewportWindow(unsigned int textureID) NSL_NOEXCEPT
		{
			const static ImVec2 uv0(0, 0);
			const static ImVec2 uv1(1, 1);
			static ImVec2 windowSize;
			//static ImVec2 textureSize;
			static ImVec2 renderSize;
			static float windowAspectRatio;
			//static float textureAspectRatio;

			ImGui::Begin("Viewport");

			//textureSize.x = textureWidth;
			//textureSize.y = textureHeight;
			//textureAspectRatio = (float)textureWidth / textureHeight;

			windowSize = ImGui::GetWindowSize();
			windowAspectRatio = windowSize.x / windowSize.y;


			renderSize.x = windowSize.x;
			renderSize.y = windowSize.y * windowAspectRatio;

			renderSize.x -= 20.0f * _scale;

			ImGui::Image(textureID, renderSize, uv0, uv1);

			ImGui::End();
		}
		void RenderMetricsWindow() NSL_NOEXCEPT
		{
			ImGui::ShowMetricsWindow();
		}
		void RenderStyleEditor() NSL_NOEXCEPT
		{
			ImGui::ShowStyleEditor();
		}
		void RenderFPSOverlay() NSL_NOEXCEPT
		{
			static bool open = true;
			static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration
				| ImGuiWindowFlags_NoDocking
				| ImGuiWindowFlags_AlwaysAutoResize
				| ImGuiWindowFlags_NoSavedSettings
				| ImGuiWindowFlags_NoFocusOnAppearing
				| ImGuiWindowFlags_NoNav;

			ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
			ImGui::SetNextWindowPos(ImVec2(0.1f, 0.1f), ImGuiCond_Always);

			ImGui::Begin("Overlay", &open, window_flags);
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		void RenderJSON(const std::string& windowName, const NSL::JSONObject& json) NSL_NOEXCEPT
		{
			ImGui::Begin(windowName.c_str());
			std::string tabs;
			ImGui::Text(NSL::JSON::ParseObjectIntoString(json, tabs).c_str());
			ImGui::End();
		}

		inline void Begin(const std::string& name) NSL_NOEXCEPT
		{
			Begin(name.c_str());
		}
		inline void Begin(const char* name) NSL_NOEXCEPT
		{
			ImGui::Begin(name);
		}
		inline void End() NSL_NOEXCEPT
		{
			ImGui::End();
		}
		inline void Text(const std::string& text) NSL_NOEXCEPT
		{
			Text(text.c_str());
		}
		inline void Text(const char* text) NSL_NOEXCEPT
		{
			ImGui::Text(text);
		}
		inline void SameLine() NSL_NOEXCEPT
		{
			ImGui::SameLine();
		}
		inline void Float(float value) NSL_NOEXCEPT
		{
			ImGui::Text("%f", value);
		}
		inline void Double(double value) NSL_NOEXCEPT
		{
			ImGui::Text("%f", value);
		}
		inline void Int(int value) NSL_NOEXCEPT
		{
			ImGui::Text("%i", value);
		}
		inline void InputFloat(const std::string& name, float& value) NSL_NOEXCEPT
		{
			InputFloat(name.c_str(), value);
		}
		inline void InputFloat(const char* name, float& value) NSL_NOEXCEPT
		{
			ImGui::InputFloat(name, &value);
		}
		inline void InputInt(const std::string& name, int& value) NSL_NOEXCEPT
		{
			InputInt(name.c_str(), value);
		}
		inline void InputInt(const char* name, int& value) NSL_NOEXCEPT
		{
			ImGui::InputInt(name, &value);
		}
		inline void Checkbox(const std::string& name, bool& value) NSL_NOEXCEPT
		{
			Checkbox(name.c_str(), value);
		}
		inline void Checkbox(const char* name, bool& value) NSL_NOEXCEPT
		{
			ImGui::Checkbox(name, &value);
		}
		inline void SliderInt(const std::string& name, int& value, int min, int max) NSL_NOEXCEPT
		{
			SliderInt(name.c_str(), value, min, max);
		}
		inline void SliderInt(const char* name, int& value, int min, int max) NSL_NOEXCEPT
		{
			ImGui::SliderInt(name, &value, min, max);
		}

	private:
		ImGuiContext* _context;
		//ImGuiIO& _io;
		float _scale;
	};
}