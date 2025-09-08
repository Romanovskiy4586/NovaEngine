export module Vector2i;
#include "Core.h"
import std;
import Mathematics.Constants;
import Logger;

export namespace NSL
{
	struct NSL_API Vector2i final
	{
		Vector2i() = default;
		explicit Vector2i(int xy) NSL_NOEXCEPT
			: x(xy)
			, y(xy)
		{
		}
		Vector2i(int x, int y) NSL_NOEXCEPT
			: x(x)
			, y(y)
		{
		}

		Vector2i operator+(const Vector2i& other) const NSL_NOEXCEPT
		{
			return Vector2i(x + other.x, y + other.y);
		}
		Vector2i operator-(const Vector2i& other) const NSL_NOEXCEPT
		{
			return Vector2i(x - other.x, y - other.y);
		}
		Vector2i operator*(const Vector2i& other) const NSL_NOEXCEPT
		{
			return Vector2i(x * other.x, y * other.y);
		}
		Vector2i operator/(const Vector2i& other) const NSL_NOEXCEPT
		{
			return Vector2i(x / other.x, y / other.y);
		}
		Vector2i& operator+=(const Vector2i& other) NSL_NOEXCEPT
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		Vector2i& operator-=(const Vector2i& other) NSL_NOEXCEPT
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		Vector2i& operator*=(const Vector2i& other) NSL_NOEXCEPT
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
		Vector2i& operator/=(const Vector2i& other) NSL_NOEXCEPT
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		Vector2i operator+(int other) const NSL_NOEXCEPT
		{
			return Vector2i(x + other, y + other);
		}
		Vector2i operator-(int other) const NSL_NOEXCEPT
		{
			return Vector2i(x - other, y - other);
		}
		Vector2i operator*(int other) const NSL_NOEXCEPT
		{
			return Vector2i(x * other, y * other);
		}
		Vector2i operator/(int other) const NSL_NOEXCEPT
		{
			return Vector2i(x / other, y / other);
		}
		Vector2i& operator+=(int other) NSL_NOEXCEPT
		{
			x += other;
			y += other;
			return *this;
		}
		Vector2i& operator-=(int other) NSL_NOEXCEPT
		{
			x -= other;
			y -= other;
			return *this;
		}
		Vector2i& operator*=(int other) NSL_NOEXCEPT
		{
			x *= other;
			y *= other;
			return *this;
		}
		Vector2i& operator/=(int other) NSL_NOEXCEPT
		{
			x /= other;
			y /= other;
			return *this;
		}

		const int& operator[](int index) const NSL_NOEXCEPT
		{
			NSL_ASSERT(0 <= index && index <= size - 1, "Invalid index: " + std::to_string(index))
			return components[index];
		}
		int& operator[](int index) NSL_NOEXCEPT
		{
			NSL_ASSERT(0 <= index && index <= size - 1, "Invalid index: " + std::to_string(index))
			return components[index];
		}
		Vector2i operator-() const NSL_NOEXCEPT
		{
			return Vector2i(-x, -y);
		}
		bool operator==(const Vector2i& other) const NSL_NOEXCEPT
		{
			return x == other.x && y == other.y;
		}
		bool operator!=(const Vector2i& other) const NSL_NOEXCEPT
		{
			return !(*this == other);
		}
		bool operator>(const Vector2i& other) const NSL_NOEXCEPT
		{
			return (x > other.x && y >= other.y) || (x >= other.x && y > other.y);
		}
		bool operator<(const Vector2i& other) const NSL_NOEXCEPT
		{
			return (x < other.x && y <= other.y) || (x <= other.x && y < other.y);
		}

		static const int size = 2;
		union
		{
			int components[size];
			struct
			{
				int x, y;
			};
		};

		static const Vector2i Zero;
		static const Vector2i Unit;
		static const Vector2i UnitX;
		static const Vector2i UnitY;
	};

	const Vector2i Vector2i::Zero { 0, 0 };
	const Vector2i Vector2i::Unit { 1, 1 };
	const Vector2i Vector2i::UnitX{ 1, 0 };
	const Vector2i Vector2i::UnitY{ 0, 1 };
}