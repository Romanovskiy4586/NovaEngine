module;
#include "glad.h"
#include "glfw3.h"
#include "miniaudio.h"
export module BaseWindow;
#include "Core.h"
import std;
import NSL;
import WindowState;
import Monitor;
import FPSController;
import KeyboardState;
import KeyboardKeys;
import MouseState;
import MouseButtons;
import ImGuiUI;

namespace IO
{
	void ErrorCallback(int error_code, const char* description)
	{
		std::stringstream ss;
		ss << "GLFW Error: " << error_code << ", " << description;
		LogError(ss.str());
	}
}

export namespace IO
{
	class NSL_API BaseWindow final
		: public NSL::INonCopyable
		, public NSL::INonMovable
	{
	public:
		BaseWindow() NSL_NOEXCEPT
			: _windowState()
			, monitor()
			, UI()
			, windowShouldClose(false)
			, renderWindowState(false)
			, renderUI(true)
			, _fpsController()
			, _glfwMonitor(nullptr)
			, _glfwWindow(nullptr)
			, _keyboardState()
			, _mouseState()
			, _windowCreationTimePoint(NSL::GetTime())
			, _deltaFrame(0.0)
		{
			// Order matter, if changed - may be bugs
			//-------------------------------//
			if (!_InitGLFW()) return;
			_InitMonitor();
			if (!_CreateGLFWwindow()) return;
			_Subscribe();
			UI.Init(_glfwWindow);
			if (!_InitGLAD()) return;
			if (!_InitMiniaudio()) return;
			//-------------------------------//

			_fpsController.SetFPSLock(_windowState.fpsLock);
			_fpsController.lockFPS = !_windowState.vsync;
		}
		~BaseWindow() NSL_NOEXCEPT
		{
			UI.Shutdown();
			_ShutdownGLFW();
			_ShutdownMiniaudio();
		}

	public: // Getters/Setters
		double GetDelta() const NSL_NOEXCEPT { return _deltaFrame; }
		NSL::JSON GetWindowAsJSON() const NSL_NOEXCEPT { return _windowState.AsJSON(); }
		double GetWindowLifetime() const NSL_NOEXCEPT { return static_cast<double>(NSL::GetTime().sinceEpoch - _windowCreationTimePoint.sinceEpoch) / 1000000.0; }
		const std::string& GetWindowName() const NSL_NOEXCEPT { return _windowState.name; }
		int GetWidth() const NSL_NOEXCEPT { return _windowState.width; }
		int GetHeight() const NSL_NOEXCEPT { return _windowState.height; }
		bool GetVsync() const NSL_NOEXCEPT { return _windowState.vsync; }
		int GetFPSLock() const NSL_NOEXCEPT { return _windowState.fpsLock; }
		bool GetFullscreen() const NSL_NOEXCEPT { return _windowState.fullscreen; }
		bool GetWindowMaximized() const NSL_NOEXCEPT { return _windowState.maximized; }
		int GetWindowPositionX() const NSL_NOEXCEPT { return _windowState.positionX; }
		int GetWindowPositionY() const NSL_NOEXCEPT { return _windowState.positionY; }
		bool GetCursorGrabbed() const NSL_NOEXCEPT { return _windowState.cursorGrabbed; }
		int GetFramebufferWidth() const NSL_NOEXCEPT { return _windowState.framebufferWidth; }
		int GetFramebufferHeight() const NSL_NOEXCEPT { return _windowState.framebufferHeight; }

		void SetWindowName(const std::string& value) NSL_NOEXCEPT
		{
			_windowState.name = value;
			glfwSetWindowTitle(_glfwWindow, value.c_str());
		}
		void SetWidth(int value) NSL_NOEXCEPT
		{
			_windowState.width = value; 
			glfwSetWindowSize(_glfwWindow, value, _windowState.height);
			
		}
		void SetHeight(int value) NSL_NOEXCEPT
		{
			_windowState.height = value;
			glfwSetWindowSize(_glfwWindow, _windowState.width, value);
		}
		void SetVsync(bool value) NSL_NOEXCEPT
		{
			_windowState.vsync = value;
			glfwSwapInterval(_windowState.vsync);
			_fpsController.lockFPS = !_windowState.vsync;
		}
		void SetFPSLock(int value) NSL_NOEXCEPT
		{
			_windowState.fpsLock = value; 
			_fpsController.SetFPSLock(_windowState.fpsLock);
		}
		void SetWindowPositionX(int value) NSL_NOEXCEPT
		{
			_windowState.positionX = value; 
			glfwSetWindowPos(_glfwWindow, value, _windowState.positionY);
		}
		void SetWindowPositionY(int value) NSL_NOEXCEPT
		{
			_windowState.positionY = value;
			glfwSetWindowPos(_glfwWindow, _windowState.positionX, value);
		}
		void SetWindowMaximized(bool value) NSL_NOEXCEPT
		{
			_windowState.maximized = value;
			value ? glfwMaximizeWindow(_glfwWindow) : glfwRestoreWindow(_glfwWindow);
		}
		void SetFullscreen(bool value) NSL_NOEXCEPT
		{
			_windowState.fullscreen = value;
			glfwSetWindowMonitor(_glfwWindow, value ? _glfwMonitor : 0, 0, 0, monitor.ResolutionWidth(), monitor.ResolutionWidth(), monitor.RefreshRate());

		}
		void SetCursorGrabbed(bool value) NSL_NOEXCEPT
		{
			_windowState.cursorGrabbed = value;
			glfwSetInputMode(_glfwWindow, GLFW_CURSOR, _windowState.cursorGrabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		}
		
	public: // Input state check getters
		bool IsKeyboardKeyPressed(KeyboardKey key) const NSL_NOEXCEPT
		{
			return _keyboardState.IsKeyPressed(key);
		}
		bool IsKeyboardKeyDown(KeyboardKey key) const NSL_NOEXCEPT
		{
			return _keyboardState.IsKeyDown(key);
		}
		bool IsKeyboardKeyReleased(KeyboardKey key) const NSL_NOEXCEPT
		{
			return _keyboardState.IsKeyReleased(key);
		}

		bool IsMouseButtonPressed(MouseButton button) const NSL_NOEXCEPT
		{
			return _mouseState.IsButtonPressed(button);
		}
		bool IsMouseButtonDown(MouseButton button) const NSL_NOEXCEPT
		{
			return _mouseState.IsButtonDown(button);
		}
		bool IsMouseButtonReleased(MouseButton button) const NSL_NOEXCEPT
		{
			return _mouseState.IsButtonReleased(button);
		}

		NSL::Vector2 GetCursorPosition() const NSL_NOEXCEPT
		{
			return _mouseState.GetPosition();
		}
		NSL::Vector2 GetCursorPositionDelta() const NSL_NOEXCEPT
		{
			return _mouseState.GetDelta();
		}
		float GetMouseScrollDelta() const NSL_NOEXCEPT
		{
			return _mouseState.GetScrollDelta();
		}

	public: // Loop start and end
		void StartFrame() NSL_NOEXCEPT
		{
			_fpsController.StartFrame();
			glfwPollEvents();
			if (renderUI) UI.StartRender();
		}
		void EndFrame() NSL_NOEXCEPT
		{
			static int width = _windowState.width;
			static int height = _windowState.height;
			static bool vsync = _windowState.vsync;
			static int fpsLock = _fpsController.GetFPSLock();

			_keyboardState.UpdateKeyboardState();
			_mouseState.UpdateMouseState();

			if (renderUI && renderWindowState)
			{
				UI.Begin("Window State");

				UI.Text("FPS: ");
				UI.SameLine();
				UI.Double(1000.0 / _deltaFrame);

				if (width != _windowState.width) width = _windowState.width;
				UI.InputInt("Width", width);
				if (width != _windowState.width) SetWidth(width);

				if (height != _windowState.height) height = _windowState.height;
				UI.InputInt("Height", height);
				if (height != _windowState.height) SetHeight(height);

				if (vsync != _windowState.vsync) vsync = _windowState.vsync;
				UI.Checkbox("VSync", vsync);
				if (vsync != _windowState.vsync) SetVsync(vsync);

				if (fpsLock != _fpsController.GetFPSLock()) fpsLock = _fpsController.GetFPSLock();
				UI.SliderInt("FPSLock", fpsLock, 0, 240);
				if (fpsLock != _fpsController.GetFPSLock()) SetFPSLock(fpsLock);

				UI.End();
			}

			if (renderUI) UI.EndRender();

			glfwSwapBuffers(_glfwWindow);

			// Controls FPS and writes delta frame
			_deltaFrame = _fpsController.EndFrame();
		}

	public: // Public members
		Monitor monitor;
		ImGuiUI UI;
		bool windowShouldClose;
		bool renderWindowState;
		bool renderUI;

	public: // Callbacks
		void KeyCallback(KeyboardKey key, KeyState action) NSL_NOEXCEPT
		{
			_keyboardState.SetButtonState(key, action);
		}
		void CursorPositionCallback(NSL::Vector2 position) NSL_NOEXCEPT
		{
			_mouseState.SetPosition(position);
		}
		void CursorEnterCallback(bool entered) NSL_NOEXCEPT
		{

		}
		void MouseButtonCallback(MouseButton button, ButtonState action) NSL_NOEXCEPT
		{
			_mouseState.SetButtonState(button, action);
		}
		void ScrollCallback(float offset) NSL_NOEXCEPT
		{
			_mouseState.SetScrollDelta(offset);
		}
		void DropCallback(int count, const char** paths) NSL_NOEXCEPT
		{

		}
		void WindowCloseCallback() NSL_NOEXCEPT
		{
			windowShouldClose = true;
		}
		void WindowSizeCallback(int width, int height) NSL_NOEXCEPT
		{
			_windowState.width = width;
			_windowState.height = height;
		}
		void WindowPosCallback(int x, int y) NSL_NOEXCEPT
		{
			_windowState.positionX = x;
			_windowState.positionY = y;
		}
		void WindowMaximizeCallback(bool maximized) NSL_NOEXCEPT
		{
			_windowState.maximized = maximized;
		}
		void FramebufferCallback(int width, int height) NSL_NOEXCEPT
		{
			_windowState.framebufferWidth = width;
			_windowState.framebufferHeight = height;
			_NotifyOnFramebufferSubscribers(width, height);
		}

		void SubscribeOnFramebuffer(std::function<void(int, int)> subscriber) NSL_NOEXCEPT
		{
			_framebufferSubscribers.push_back(subscriber);
		}
		void SubscribeOnWindowSize(std::function<void(int, int)> subscriber) NSL_NOEXCEPT
		{
			_windowSizeSubscribers.push_back(subscriber);
		}

	private: // Constructor/destructor helpers
		bool _InitGLFW() NSL_NOEXCEPT
		{
			glfwSetErrorCallback(ErrorCallback);
			if (!glfwInit())
			{
				LogError("Failed to initialize GLFW");
				return false;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_RESIZABLE, _windowState.resizable);
			glfwWindowHint(GLFW_SAMPLES, 4);
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
			//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
			//glfwSetWindowOpacity(_glfwWindow, 0.5f);
			
			LogInfo("GLFW is initialized");
			return true;
		}
		void _InitMonitor() NSL_NOEXCEPT
		{
			_glfwMonitor = glfwGetPrimaryMonitor();
			Monitor::Config config;

			glfwGetMonitorPhysicalSize(_glfwMonitor, &(config.width), &(config.height));

			auto mode = glfwGetVideoMode(_glfwMonitor);
			config.resolutionWidth = mode->width;
			config.resolutionHeight = mode->height;
			config.refreshRate = mode->refreshRate;
			glfwGetMonitorContentScale(_glfwMonitor, &(config.scale), nullptr);

			monitor.Init(config);
			LogInfo("Monitor is initialized");
		}
		bool _CreateGLFWwindow() NSL_NOEXCEPT
		{
			_glfwWindow = glfwCreateWindow(_windowState.width, _windowState.height, _windowState.name.c_str(), _windowState.fullscreen ? _glfwMonitor : 0, 0);
			if (!_glfwWindow)
			{
				LogError("Failed to create GLFW window");
				return false;
			}
			
			glfwMakeContextCurrent(_glfwWindow);
			glfwSetWindowPos(_glfwWindow, _windowState.positionX, _windowState.positionY);
			if (_windowState.maximized)
			{
				glfwMaximizeWindow(_glfwWindow);
			}
			glfwGetFramebufferSize(_glfwWindow, &(_windowState.framebufferWidth), &(_windowState.framebufferHeight));

			glfwShowWindow(_glfwWindow);
			LogInfo("GLFW Window is created");
			return true;
		}
		bool _InitGLAD() NSL_NOEXCEPT
		{
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				LogError("Failed to initialize GLAD");
				return false;
			}

			//glEnable(GL_FRAMEBUFFER_SRGB);
			glfwSwapInterval(_windowState.vsync);

			LogInfo("GLAD is initialized");
			return true;
		}
		bool _InitMiniaudio() NSL_NOEXCEPT
		{
			// Настройка конфигурации устройства для вывода
			_deviceConfig = ma_device_config_init(ma_device_type_playback);

			// Попытка использовать WASAPI
			_deviceConfig.playback.format = ma_format_f32;  // 32-bit float
			_deviceConfig.playback.channels = 2;             // стерео
			_deviceConfig.sampleRate = 48000;         // частота дискретизации
			_deviceConfig.dataCallback = NULL;          // ты можешь установить свой callback
			_deviceConfig.pUserData = NULL;

			ma_result result = ma_device_init(NULL, &_deviceConfig, &_device);

			if (result != MA_SUCCESS)
			{
				LogInfo("Cannot initialize device");
				return false;
			}

			// Запуск устройства
			if (ma_device_start(&_device) != MA_SUCCESS)
			{
				LogInfo("Fail to start device");
				ma_device_uninit(&_device);
				return false;
			}

			LogInfo("Audio device succsessfully started");
			LogInfo("Used backend: " + _GetMiniaudioUsedBackend());
			return true;
		}

		void _ShutdownGLFW() NSL_NOEXCEPT
		{
			if (_glfwWindow)
			{
				glfwDestroyWindow(_glfwWindow);
				LogInfo("Window destruction is completed");
			}

			glfwTerminate();
			LogInfo("GLFW Termination is completed");
		}
		void _ShutdownMiniaudio() NSL_NOEXCEPT
		{
			ma_device_uninit(&_device);
			LogInfo("Miniaudio devise is uninited");
		}
		std::string _GetMiniaudioUsedBackend() NSL_NOEXCEPT
		{
			switch (_device.pContext->backend)
			{
			case ma_backend_wasapi:		return "WASAPI";
			case ma_backend_dsound:		return "DirectSound";
			case ma_backend_winmm:		return "WinMM";
			case ma_backend_coreaudio:	return "Core Audio";
			case ma_backend_sndio:		return "sndio";
			case ma_backend_audio4:		return "audio(4)";
			case ma_backend_oss:		return "OSS";
			case ma_backend_pulseaudio: return "PulseAudio";
			case ma_backend_alsa:		return "ALSA";
			case ma_backend_jack:		return "JACK";
			case ma_backend_aaudio:		return "AAudio";
			case ma_backend_opensl:		return "OpenSL | ES";
			case ma_backend_webaudio:	return "Emscripten / WebAudio";
			case ma_backend_custom:		return "CUSTOM";
			case ma_backend_null:		return "NULL";
			default:					return "UNDEFINED";
			}
		}

	private:
		void _Subscribe() NSL_NOEXCEPT
		{
			glfwSetWindowUserPointer(_glfwWindow, this);
			glfwSetKeyCallback(_glfwWindow, _KeyCallback);
			glfwSetCursorPosCallback(_glfwWindow, _CursorPositionCallback);
			glfwSetCursorEnterCallback(_glfwWindow, _CursorEnterCallback);
			glfwSetMouseButtonCallback(_glfwWindow, _MouseButtonCallback);
			glfwSetScrollCallback(_glfwWindow, _ScrollCallback);
			glfwSetDropCallback(_glfwWindow, _DropCallback);
			glfwSetWindowCloseCallback(_glfwWindow, _WindowCloseCallback);
			glfwSetWindowSizeCallback(_glfwWindow, _WindowSizeCallback);
			glfwSetWindowPosCallback(_glfwWindow, _WindowPosCallback);
			glfwSetWindowMaximizeCallback(_glfwWindow, _WindowMaximizeCallback);
			glfwSetFramebufferSizeCallback(_glfwWindow, _FramebufferSizeCallback);
		}
		static void _KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) NSL_NOEXCEPT
		{
			KeyState state;
			if (action == GLFW_RELEASE)
			{
				state = KeyState::Released;
			}
			else
			{
				state = KeyState::Pressed;
			}
			((BaseWindow*)(glfwGetWindowUserPointer(window)))->KeyCallback(static_cast<KeyboardKey>(key), state);
		}
		static void _CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) NSL_NOEXCEPT
		{
			((BaseWindow*)(glfwGetWindowUserPointer(window)))->CursorPositionCallback(NSL::Vector2(static_cast<float>(xpos), static_cast<float>(ypos)));
		}
		static void _CursorEnterCallback(GLFWwindow* window, int entered) NSL_NOEXCEPT
		{
			((BaseWindow*)(glfwGetWindowUserPointer(window)))->CursorEnterCallback(static_cast<bool>(entered));
		}
		static void _MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) NSL_NOEXCEPT
		{
			ButtonState state;
			if (action == GLFW_RELEASE)
			{
				state = ButtonState::Released;
			}
			else
			{
				state = ButtonState::Pressed;
			}
			((BaseWindow*)(glfwGetWindowUserPointer(window)))->MouseButtonCallback(static_cast<MouseButton>(button), state);
		}
		static void _ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) NSL_NOEXCEPT
		{
			((BaseWindow*)(glfwGetWindowUserPointer(window)))->ScrollCallback(static_cast<float>(yoffset));
		}
		static void _DropCallback(GLFWwindow* window, int count, const char** paths) NSL_NOEXCEPT
		{
			((BaseWindow*)(glfwGetWindowUserPointer(window)))->DropCallback(count, paths);
		}
		static void _WindowCloseCallback(GLFWwindow* window) NSL_NOEXCEPT
		{
			((BaseWindow*)(glfwGetWindowUserPointer(window)))->WindowCloseCallback();
		}
		static void _WindowSizeCallback(GLFWwindow* window, int width, int height) NSL_NOEXCEPT
		{
			((BaseWindow*)(glfwGetWindowUserPointer(window)))->WindowSizeCallback(width, height);
		}
		static void _WindowPosCallback(GLFWwindow* window, int x, int y) NSL_NOEXCEPT
		{
			((BaseWindow*)(glfwGetWindowUserPointer(window)))->WindowPosCallback(x, y);
		}
		static void _WindowMaximizeCallback(GLFWwindow* window, int maximized) NSL_NOEXCEPT
		{
			((BaseWindow*)(glfwGetWindowUserPointer(window)))->WindowMaximizeCallback(static_cast<bool>(maximized));
		}
		static void _FramebufferSizeCallback(GLFWwindow* window, int width, int height) NSL_NOEXCEPT
		{
			((BaseWindow*)(glfwGetWindowUserPointer(window)))->FramebufferCallback(width, height);
		}

		void _NotifyOnFramebufferSubscribers(int width, int height) NSL_NOEXCEPT
		{
			for (auto& i : _framebufferSubscribers)
			{
				i(width, height);
			}
		}
		void _NotifyOnWindowSizeSubscribers(int width, int height) NSL_NOEXCEPT
		{
			for (auto& i : _windowSizeSubscribers)
			{
				i(width, height);
			}
		}

	private:
		//BaseWindow
		WindowState _windowState;
		FPSController _fpsController;
		NSL::TimePoint _windowCreationTimePoint;
		double _deltaFrame;
		KeyboardState _keyboardState;
		MouseState _mouseState;
		std::vector<std::function<void(int, int)>> _framebufferSubscribers;
		std::vector<std::function<void(int, int)>> _windowSizeSubscribers;

		//GLFW
		GLFWmonitor* _glfwMonitor;
		GLFWwindow* _glfwWindow;
		
		// miniaudio
		ma_context _context;
		ma_device _device;
		ma_device_config _deviceConfig;
		ma_engine _engine;
	};
}