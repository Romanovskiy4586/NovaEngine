#ifndef CORE_H
#define CORE_H


// Define compiler. Supported MSVC, Clang and GCC
#if defined(_MSC_VER)
	#define COMPILER_MSVC
	#define COMPILER "MSVC"
#elif defined(__clang__)
	#define COMPILER_CLANG
	#define COMPILER "Clang"
#elif defined(__GNUC__)
	#define COMPILER_GCC
	#define COMPILER "GCC"
#else
	#error Compiler cannot be determined or is not supported!
#endif


// Define processor architecture
#if defined(_M_IX86) || defined(__i386__)
	#define ARCHITECTURE_X86
	#define ARCHITECTURE "x86"
#elif defined(_M_X64) || defined(__x86_64__)
	#define ARCHITECTURE_X64
	#define ARCHITECTURE "x64"
#elif defined(_M_ARM) || defined(__arm__)
	#define ARCHITECTURE_ARM
	#define ARCHITECTURE "ARM"
#elif defined(_M_ARM64) || defined(__aarch64__)
	#define ARCHITECTURE_ARM64
	#define ARCHITECTURE "ARM64"
#else
	error Processor architecture cannot be determined or is not supported!
#endif


// Define OS. Supported Windows, Linux and macOS
#if defined(_WIN32)
	#define OS_WINDOWS
	#if defined(_WIN64)
		#define OS_WINDOWS64
		#define OS "Windows x64"
	#else
		#define OS_WINDOWS32
		#define OS "Windows x86"
	#endif
#elif defined(__linux__)
	#define OS_LINUX
	#define OS "Linux"
#elif defined(__APPLE__)
	#include <TargetConditionals.h>
	#if defined(TARGET_OS_MAC) && !defined(TARGET_OS_IPHONE)
		#define OS_MACOS
		#define OS "macOS"
	#else
		#error OS from Apple family, but not macOS, which is not supported!
	#endif
#else
	#error OS cannot be determined or is not supported!
#endif


// Static/Dynamic linking macros
#if defined(OS_WINDOWS)
	#if defined(SHARED_EXPORT)
		#define NSL_API __declspec(dllexport)
	#elif defined(SHARED_IMPORT)
		#define NSL_API __declspec(dllimport)
	#else
		#define NSL_API
	#endif // DLL_EXPORT || DLL_IMPORT
#else
	#if defined(SHARED_EXPORT)
		#pragma message Be sure that you compile with compiler flag -fvisibility=hidden
		#define NSL_API __attribute__((visibility("default")))
	#else
		#define NSL_API
	#endif
#endif // OS_WINDOWS


// Save definitions
#ifndef __FILE__
	#pragma warning Macro __FILE__ is not defined, Logging will not show file names
	#define __FILE__ "UNKNOWN"
#endif

#ifndef __LINE__
	#pragma warning Macro __LINE__ is not defined, NSL asserts and GL/AL calls will not show lines
	#define __LINE__ "UNKNOWN"
#endif


// Configuration dependened macros
#ifndef NDEBUG
	#define NSL_MAIN int main(int argc, char* argv[])
	#define NSL_ASSERT(expression, message) if (!(expression)) { LogError("Fatal error. NSL Assertion is failed. Expression was: "#expression". Line: " + std::to_string(__LINE__) + ". Assertion message:\n" + message); std::terminate(); }
	#define NSL_NOEXCEPT noexcept(false)
	#define NSL_DEBUG(Logic) Logic

	#define LogInfo(msg) NSL::Logger::LogMessageWithFileNameInConsole(msg, __FILE__, NSL::Logger::MessageType::Info)
	#define LogWarning(msg) NSL::Logger::LogMessageWithFileNameInConsole(msg, __FILE__, NSL::Logger::MessageType::Warning)
	#define LogError(msg) NSL::Logger::LogMessageWithFileNameInConsole(msg, __FILE__, NSL::Logger::MessageType::Error)

	#define ALCall(function)\
		function;\
		while(ALenum error = alGetError())\
		{\
			LogError(#function" throw an Error: " + std::to_string(error) + " at line " + std::to_string(__LINE__));\
		}
	#define GLCall(function)\
		function;\
		while(GLenum error = glGetError())\
		{\
			LogError(#function" throw an Error: " + std::to_string(error) + " at line " + std::to_string(__LINE__));\
		}
#else // !NDEBUG
	#ifdef OS_WINDOWS
		#define NSL_MAIN int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
	#else
		#define NSL_MAIN int main(int argc, char* argv[])
	#endif

	#define NSL_ASSERT(expression, message) {}
	#define NSL_NOEXCEPT noexcept(true)
	#define NSL_DEBUG(Logic) {}

	#define LogInfo(msg) {}
	#define LogWarning(msg) {}
	#define LogError(msg) {}

	#define ALCall(function) function;
	#define GLCall(function) function;
#endif // !NDEBUG

#endif // !CORE_H