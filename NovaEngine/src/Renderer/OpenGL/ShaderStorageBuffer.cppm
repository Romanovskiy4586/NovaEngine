export module ShaderStorageBuffer;
#include "Core.h"
import std;
import NSL;

export namespace Renderer
{
	struct NSL_API ShaderStorageBuffer
	{
	public:
		ShaderStorageBuffer() NSL_NOEXCEPT
			: _data()
			, _count(0)
			, _id(0)
		{
		}

	public:
		void Reallocate(const void* dataPtr, unsigned long long size, unsigned int count) NSL_NOEXCEPT
		{
			Clear();

			_data.resize(size);
			std::memcpy(_data.data(), dataPtr, size);
			_count = count;
		}
		void* Get(unsigned long long offset = 0ull) NSL_NOEXCEPT
		{
			return _data.data() + offset;
		}
		void Clear() NSL_NOEXCEPT
		{
			_data.clear();
		}
		const void* Data() const NSL_NOEXCEPT
		{
			return _data.data();
		}
		unsigned long long Size() const NSL_NOEXCEPT
		{
			return _data.size();
		}
		unsigned int Count() const NSL_NOEXCEPT
		{
			return _count;
		}
	
	private:
		std::vector<char> _data;
		unsigned int _count;

		unsigned int _id;
		friend class GLBuffersContextManager;
	};
}