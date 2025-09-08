export module NSL.BinaryStream;
#include "Core.h"
import std;

#define RETURN_IF_EMPTY if (Empty()) return result
#define LEFT_TO_RIGHT_CYCLE

#define READ_BYTES(type, bytes)\
const size_t BYTES_TO_READ = bytes;\
type result = 0;\
RETURN_IF_EMPTY;\
char* resultPtr = reinterpret_cast<char*>(&result);\
switch (order)\
{\
	case BinaryStream::BytesReadOrder::LeftToRight:\
		for (size_t i = 0; i < BYTES_TO_READ; ++i)\
		{\
			resultPtr[i] = _data[_pointer++];\
		}\
		break;\
	case BinaryStream::BytesReadOrder::RightToLeft:\
		for (size_t i = BYTES_TO_READ - 1; 0 <= i && i <= BYTES_TO_READ - 1; --i)\
		{\
			resultPtr[i] = _data[_pointer++];\
		}\
		break;\
	default:\
		break;\
}\
return result

#define PEEK_BYTES(type, bytes)\
const size_t BYTES_TO_READ = bytes;\
type result = 0;\
RETURN_IF_EMPTY;\
char* resultPtr = reinterpret_cast<char*>(&result);\
size_t pointerCopy = _pointer;\
switch (order)\
{\
	case BinaryStream::BytesReadOrder::LeftToRight:\
		for (size_t i = 0; i < BYTES_TO_READ; ++i)\
		{\
			resultPtr[i] = _data[pointerCopy++];\
		}\
		break;\
	case BinaryStream::BytesReadOrder::RightToLeft:\
		for (size_t i = BYTES_TO_READ - 1; 0 <= i && i <= BYTES_TO_READ - 1; --i)\
		{\
			resultPtr[i] = _data[pointerCopy++];\
		}\
		break;\
	default:\
		break;\
}\
return result

export namespace NSL
{
	class NSL_API BinaryStream final
	{
	public:
		BinaryStream() NSL_NOEXCEPT
			: _data()
			, _pointer(0)
		{
		}
		BinaryStream(const std::string& data) NSL_NOEXCEPT
			: _data(data)
			, _pointer(0)
		{
		}

	public:
		enum class NSL_API BytesReadOrder { LeftToRight, RightToLeft };

	public:
		std::string ReadBytes(size_t count) NSL_NOEXCEPT
		{
			std::string result(count, 0);
			RETURN_IF_EMPTY;
			for (size_t i = 0; i < count; ++i)
			{
				result[i] = _data[_pointer++];
			}
			return result;
		}
		std::string PeekBytes(size_t count) const NSL_NOEXCEPT
		{
			std::string result(count, 0);
			RETURN_IF_EMPTY;
			size_t pointerCopy = _pointer;
			for (size_t i = 0; i < count; ++i)
			{
				result[i] = _data[pointerCopy++];
			}
			return result;
		}
		void SkipBytes(size_t count) NSL_NOEXCEPT
		{
			for (size_t i = 0; i < count; ++i)
			{
				++_pointer;
			}
		}
		size_t Search(const std::string& sequence) const NSL_NOEXCEPT
		{
			for (size_t i = 0; i < _data.length(); ++i)
			{
				if (_data[i] == sequence[0])
				{
					size_t j;
					for (j = 0; j < sequence.length(); ++j)
					{
						if (_data[i + j] != sequence[j]) break;
					}
					if (j == sequence.length()) return i;
				}
			}

			return NullPointerPosition;
		}
		size_t SearchFromCurrentPosition(const std::string& sequence) const NSL_NOEXCEPT
		{
			for (size_t i = _pointer; i < _data.length(); ++i)
			{
				if (_data[i] == sequence[0])
				{
					size_t j;
					for (j = 0; j < sequence.length(); ++j)
					{
						if (_data[i + j] != sequence[j]) break;
					}
					if (j == sequence.length()) return i;
				}
			}

			return NullPointerPosition;
		}
		bool MovePointerTo(size_t position) NSL_NOEXCEPT
		{
			if (position > _data.length()) return false;

			_pointer = position;
			return true;
		}

		char ReadInt8(BytesReadOrder order = BytesReadOrder::LeftToRight) NSL_NOEXCEPT
		{
			READ_BYTES(char, 1);
		}
		unsigned char ReadUint8(BytesReadOrder order = BytesReadOrder::LeftToRight) NSL_NOEXCEPT
		{
			READ_BYTES(unsigned char, 1);
		}
		short ReadInt16(BytesReadOrder order = BytesReadOrder::LeftToRight) NSL_NOEXCEPT
		{
			READ_BYTES(short, 2);
		}
		unsigned short ReadUint16(BytesReadOrder order = BytesReadOrder::LeftToRight) NSL_NOEXCEPT
		{
			READ_BYTES(unsigned short, 2);
		}
		int ReadInt32(BytesReadOrder order = BytesReadOrder::LeftToRight) NSL_NOEXCEPT
		{
			READ_BYTES(int, 4);
		}
		unsigned int ReadUint32(BytesReadOrder order = BytesReadOrder::LeftToRight) NSL_NOEXCEPT
		{
			READ_BYTES(unsigned int, 4);
		}
		long long ReadInt64(BytesReadOrder order = BytesReadOrder::LeftToRight) NSL_NOEXCEPT
		{
			READ_BYTES(long long, 8);
		}
		unsigned long long ReadUint64(BytesReadOrder order = BytesReadOrder::LeftToRight) NSL_NOEXCEPT
		{
			READ_BYTES(unsigned long long, 8);
		}
		float ReadFloat(BytesReadOrder order = BytesReadOrder::LeftToRight) NSL_NOEXCEPT
		{
			READ_BYTES(float, 4);
		}
		double ReadDouble(BytesReadOrder order = BytesReadOrder::LeftToRight) NSL_NOEXCEPT
		{
			READ_BYTES(double, 8);
		}
		char PeekInt8(BytesReadOrder order = BytesReadOrder::LeftToRight) const NSL_NOEXCEPT
		{
			PEEK_BYTES(char, 1);
		}
		unsigned char PeekUint8(BytesReadOrder order = BytesReadOrder::LeftToRight) const NSL_NOEXCEPT
		{
			PEEK_BYTES(unsigned char, 1);
		}
		short PeekInt16(BytesReadOrder order = BytesReadOrder::LeftToRight) const NSL_NOEXCEPT
		{
			PEEK_BYTES(short, 2);
		}
		unsigned short PeekUint16(BytesReadOrder order) const NSL_NOEXCEPT
		{
			PEEK_BYTES(unsigned short, 2);
		}
		int PeekInt32(BytesReadOrder order = BytesReadOrder::LeftToRight) const NSL_NOEXCEPT
		{
			PEEK_BYTES(int, 4);
		}
		unsigned int PeekUint32(BytesReadOrder order = BytesReadOrder::LeftToRight) const NSL_NOEXCEPT
		{
			PEEK_BYTES(unsigned int, 4);
		}
		long long PeekInt64(BytesReadOrder order = BytesReadOrder::LeftToRight) const NSL_NOEXCEPT
		{
			PEEK_BYTES(long long, 8);
		}
		unsigned long long PeekUint64(BytesReadOrder order = BytesReadOrder::LeftToRight) const NSL_NOEXCEPT
		{
			PEEK_BYTES(unsigned long long, 8);
		}
		float PeekFloat(BytesReadOrder order = BytesReadOrder::LeftToRight) const NSL_NOEXCEPT
		{
			PEEK_BYTES(float, 4);
		}
		double PeekDouble(BytesReadOrder order = BytesReadOrder::LeftToRight) const NSL_NOEXCEPT
		{
			PEEK_BYTES(double, 8);
		}
		size_t Length() const NSL_NOEXCEPT
		{
			return _data.size() - _pointer;
		}
		size_t BytesWasReaded() const NSL_NOEXCEPT
		{
			return _pointer;
		}
		bool Empty() const NSL_NOEXCEPT
		{
			return _data.size() <= _pointer;
		}
		void Reset() NSL_NOEXCEPT
		{
			_pointer = 0;
		}
		std::string Data() const NSL_NOEXCEPT
		{
			return _data;
		}

	public:
		static constexpr const size_t NullPointerPosition = std::string::npos;

	private:
		std::string _data;
		size_t _pointer;
	};
}