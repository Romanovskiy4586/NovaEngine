export module Vector2ui;
#include "Core.h"
import std;
import Mathematics.Constants;
import Logger;

export namespace NSL
{
	struct NSL_API Vector2ui final
	{
		Vector2ui() = default;
		explicit Vector2ui(unsigned int xy) NSL_NOEXCEPT
			: x(xy)
			, y(xy)
		{
		}
		Vector2ui(unsigned int x, unsigned int y) NSL_NOEXCEPT
			: x(x)
			, y(y)
		{
		}

		Vector2ui operator+(const Vector2ui& other) const NSL_NOEXCEPT
		{
			return Vector2ui(x + other.x, y + other.y);
		}
		Vector2ui operator-(const Vector2ui& other) const NSL_NOEXCEPT
		{
			return Vector2ui(x - other.x, y - other.y);
		}
		Vector2ui operator*(const Vector2ui& other) const NSL_NOEXCEPT
		{
			return Vector2ui(x * other.x, y * other.y);
		}
		Vector2ui operator/(const Vector2ui& other) const NSL_NOEXCEPT
		{
			return Vector2ui(x / other.x, y / other.y);
		}
		Vector2ui& operator+=(const Vector2ui& other) NSL_NOEXCEPT
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		Vector2ui& operator-=(const Vector2ui& other) NSL_NOEXCEPT
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		Vector2ui& operator*=(const Vector2ui& other) NSL_NOEXCEPT
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
		Vector2ui& operator/=(const Vector2ui& other) NSL_NOEXCEPT
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		Vector2ui operator+(unsigned int other) const NSL_NOEXCEPT
		{
			return Vector2ui(x + other, y + other);
		}
		Vector2ui operator-(unsigned int other) const NSL_NOEXCEPT
		{
			return Vector2ui(x - other, y - other);
		}
		Vector2ui operator*(unsigned int other) const NSL_NOEXCEPT
		{
			return Vector2ui(x * other, y * other);
		}
		Vector2ui operator/(unsigned int other) const NSL_NOEXCEPT
		{
			return Vector2ui(x / other, y / other);
		}
		Vector2ui& operator+=(unsigned int other) NSL_NOEXCEPT
		{
			x += other;
			y += other;
			return *this;
		}
		Vector2ui& operator-=(unsigned int other) NSL_NOEXCEPT
		{
			x -= other;
			y -= other;
			return *this;
		}
		Vector2ui& operator*=(unsigned int other) NSL_NOEXCEPT
		{
			x *= other;
			y *= other;
			return *this;
		}
		Vector2ui& operator/=(unsigned int other) NSL_NOEXCEPT
		{
			x /= other;
			y /= other;
			return *this;
		}

		const unsigned int& operator[](int index) const NSL_NOEXCEPT
		{

			NSL_ASSERT(0 <= index && index <= size - 1, "Invalid index: " + std::to_string(index))
				return components[index];
		}
		unsigned int& operator[](int index) NSL_NOEXCEPT
		{
			NSL_ASSERT(0 <= index && index <= size - 1, "Invalid index: " + std::to_string(index))
				return components[index];
		}
		bool operator==(const Vector2ui& other) const NSL_NOEXCEPT
		{
			return x == other.x && y == other.y;
		}
		bool operator!=(const Vector2ui& other) const NSL_NOEXCEPT
		{
			return !(*this == other);
		}
		bool operator>(const Vector2ui& other) const NSL_NOEXCEPT
		{
			return (x > other.x && y >= other.y) || (x >= other.x && y > other.y);
		}
		bool operator<(const Vector2ui& other) const NSL_NOEXCEPT
		{
			return (x < other.x && y <= other.y) || (x <= other.x && y < other.y);
		}

		static const int size = 2;
		union
		{
			unsigned int components[size];
			struct
			{
				unsigned int x, y;
			};
		};

		static const Vector2ui Zero;
		static const Vector2ui Unit;
		static const Vector2ui UnitX;
		static const Vector2ui UnitY;
	};

	const Vector2ui Vector2ui::Zero{ 0, 0 };
	const Vector2ui Vector2ui::Unit{ 1, 1 };
	const Vector2ui Vector2ui::UnitX{ 1, 0 };
	const Vector2ui Vector2ui::UnitY{ 0, 1 };
}