export module NSL.Interfaces;
#include "Core.h"

export namespace NSL
{
	class NSL_API INonCopyable
	{
	public:
		INonCopyable() NSL_NOEXCEPT = default ;
		INonCopyable(INonCopyable&) NSL_NOEXCEPT = delete;
		INonCopyable(const INonCopyable&) NSL_NOEXCEPT = delete;
		INonCopyable(INonCopyable&&) NSL_NOEXCEPT = default;
		INonCopyable& operator=(INonCopyable&) NSL_NOEXCEPT = delete;
		INonCopyable& operator=(const INonCopyable&) NSL_NOEXCEPT = delete;
		INonCopyable& operator=(INonCopyable&&) NSL_NOEXCEPT = default;
		~INonCopyable() NSL_NOEXCEPT = default;
	};

	class NSL_API INonMovable
	{
	public:
		INonMovable() NSL_NOEXCEPT = default;
		INonMovable(INonMovable&) NSL_NOEXCEPT = default;
		INonMovable(const INonMovable&) NSL_NOEXCEPT = default;
		INonMovable(INonMovable&&) NSL_NOEXCEPT = delete;
		INonMovable& operator=(INonMovable&) NSL_NOEXCEPT = default;
		INonMovable& operator=(const INonMovable&) NSL_NOEXCEPT = default;
		INonMovable& operator=(INonMovable&&) NSL_NOEXCEPT = delete;
		~INonMovable() NSL_NOEXCEPT = default;
	};
}