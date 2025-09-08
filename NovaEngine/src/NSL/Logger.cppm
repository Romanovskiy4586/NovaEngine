export module Logger;
#include "Core.h"
import std;
import Time;

export namespace NSL
{
	class NSL_API Logger final
	{
	public:
		enum class NSL_API MessageType
		{
			Info, Warning, Error
		};

	public:
		static void LogMessageInConsole(const std::string& text, MessageType type) NSL_NOEXCEPT
		{
			if (type < s_messagePrintingState)
				return;

			PrintMessageType(type);
			PrintTime();

			std::cout << '\t' << text << "\033[0m" << std::endl;
		}
		static void LogMessageInConsole(const char* text, MessageType type) NSL_NOEXCEPT
		{
			if (type < s_messagePrintingState)
				return;

			PrintMessageType(type);
			PrintTime();

			std::cout << '\t' << text << "\033[0m" << std::endl;
		}
		static void LogMessageInConsole(const std::ostringstream& text, MessageType type) NSL_NOEXCEPT
		{
			if (type < s_messagePrintingState)
				return;

			PrintMessageType(type);
			PrintTime();

			std::cout << '\t' << text.str() << "\033[0m" << std::endl;
		}
		static void LogMessageWithFileNameInConsole(const std::string& text, const std::string& filePath, MessageType type) NSL_NOEXCEPT
		{
			LogMessageWithFileNameInConsole(text.c_str(), filePath, type);
		}
		static void LogMessageWithFileNameInConsole(const char* text, const std::string& filePath, MessageType type) NSL_NOEXCEPT
		{
			if (type < s_messagePrintingState) return;

			PrintMessageType(type);
			PrintTime();

			std::string fileName;
			char delimiter = '\\';

			size_t indexOfLastSlash = filePath.find_last_of(delimiter, filePath.size());
			size_t indexOfFirstCharacterOfFolderName = indexOfLastSlash + 1;
			size_t lengthOfFolderName = filePath.size() - indexOfLastSlash - 1;
			fileName = filePath.substr(indexOfFirstCharacterOfFolderName, lengthOfFolderName);

			size_t indexOfDot = fileName.find('.');
			fileName = fileName.substr(0, indexOfDot);

			PrintFileName(fileName);

			std::string tabs = "\t";

			std::cout << tabs << text << "\033[0m" << std::endl;
		}
		static void LogMessageWithFileNameInConsole(const std::ostringstream& text, const std::string& filePath, MessageType type) NSL_NOEXCEPT
		{
			LogMessageWithFileNameInConsole(text.str(), filePath, type);
		}
		static void SetMessagePrintingState(MessageType state) NSL_NOEXCEPT
		{
			s_messagePrintingState = state;
		}

	private:
		static void PrintTime() NSL_NOEXCEPT
		{
			TimePoint timePoint = GetTime();
			std::stringstream stream;
			stream << "[UTC ";
			stream << timePoint.hour / 10	<< timePoint.hour % 10	 << ':';
			stream << timePoint.minute / 10 << timePoint.minute % 10 << ':';
			stream << timePoint.second / 10 << timePoint.second % 10 << "] ";
			std::cout << stream.str();
		}
		static void PrintMessageType(MessageType type) NSL_NOEXCEPT
		{
			const char* typeText;
			switch (type)
			{
			case MessageType::Info:
				typeText = "\033[32m[INFO] ";
				break;
			case MessageType::Warning:
				typeText = "\033[33m[WARNING] ";
				break;
			case MessageType::Error:
				typeText = "\033[31m[ERROR] ";
				break;
			default:
				typeText = "[]";
				break;
			}

			std::cout << typeText;
		}
		static void PrintFileName(const std::string& fileName) NSL_NOEXCEPT
		{
			std::cout << '[' + fileName + ']';
		}

	private:
		static MessageType s_messagePrintingState;
	};

	Logger::MessageType Logger::s_messagePrintingState = MessageType::Info;
}