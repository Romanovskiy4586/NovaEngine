export module Vector2;
#include "Core.h"
import std;
import Mathematics.Constants;
import Logger;

static constexpr int s_size = 2;

export namespace NSL
{
	struct NSL_API Vector2 final
	{
		Vector2() = default;
		explicit Vector2(float xy) NSL_NOEXCEPT
			: x(xy)
			, y(xy)
		{
		}
		Vector2(float x, float y) NSL_NOEXCEPT
			: x(x)
			, y(y)
		{
		}

		Vector2 operator+(const Vector2& other) const NSL_NOEXCEPT
		{
			return Vector2(x + other.x, y + other.y);
		}
		Vector2 operator-(const Vector2& other) const NSL_NOEXCEPT
		{
			return Vector2(x - other.x, y - other.y);
		}
		Vector2 operator*(const Vector2& other) const NSL_NOEXCEPT
		{
			return Vector2(x * other.x, y * other.y);
		}
		Vector2 operator/(const Vector2& other) const NSL_NOEXCEPT
		{
			return Vector2(x / other.x, y / other.y);
		}
		Vector2& operator+=(const Vector2& other) NSL_NOEXCEPT
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		Vector2& operator-=(const Vector2& other) NSL_NOEXCEPT
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		Vector2& operator*=(const Vector2& other) NSL_NOEXCEPT
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
		Vector2& operator/=(const Vector2& other) NSL_NOEXCEPT
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		Vector2 operator+(float other) const NSL_NOEXCEPT
		{
			return Vector2(x + other, y + other);
		}
		Vector2 operator-(float other) const NSL_NOEXCEPT
		{
			return Vector2(x - other, y - other);
		}
		Vector2 operator*(float other) const NSL_NOEXCEPT
		{
			return Vector2(x * other, y * other);
		}
		Vector2 operator/(float other) const NSL_NOEXCEPT
		{
			return Vector2(x / other, y / other);
		}
		Vector2& operator+=(float other) NSL_NOEXCEPT
		{
			x += other;
			y += other;
			return *this;
		}
		Vector2& operator-=(float other) NSL_NOEXCEPT
		{
			x -= other;
			y -= other;
			return *this;
		}
		Vector2& operator*=(float other) NSL_NOEXCEPT
		{
			x *= other;
			y *= other;
			return *this;
		}
		Vector2& operator/=(float other) NSL_NOEXCEPT
		{
			x /= other;
			y /= other;
			return *this;
		}

		const float& operator[](int index) const NSL_NOEXCEPT
		{
			static_assert(sizeof(Vector2) == 8, "Unexpected Vector2 size");
			static_assert(alignof(Vector2) == 4, "Unexpected Vector2 alignment");

			NSL_ASSERT(0 <= index && index < s_size, "Invalid index: " + std::to_string(index))
			return *(reinterpret_cast<const float*>(this) + index);
		}
		float& operator[](int index) NSL_NOEXCEPT
		{
			static_assert(sizeof(Vector2) == 8, "Unexpected Vector2 size");
			static_assert(alignof(Vector2) == 4, "Unexpected Vector2 alignment");

			NSL_ASSERT(0 <= index && index < s_size, "Invalid index: " + std::to_string(index))
			return *(reinterpret_cast<float*>(this) + index);
		}
		Vector2 operator-() const NSL_NOEXCEPT
		{
			return Vector2(-x, -y);
		}
		bool operator==(const Vector2& other) const NSL_NOEXCEPT
		{
			return std::fabs(x - other.x) <= MATHF_EPSILON && std::fabs(y - other.y) <= MATHF_EPSILON;
		}
		bool operator!=(const Vector2& other) const NSL_NOEXCEPT
		{
			return !(*this == other);
		}
		bool operator>(const Vector2& other) const NSL_NOEXCEPT
		{
			return (x > other.x && y >= other.y) || (x >= other.x && y > other.y);
		}
		bool operator<(const Vector2& other) const NSL_NOEXCEPT
		{
			return (x < other.x && y <= other.y) || (x <= other.x && y < other.y);
		}

		float x, y;

		static const Vector2 Zero;
		static const Vector2 Unit;
		static const Vector2 UnitX;
		static const Vector2 UnitY;
	};

	const Vector2 Vector2::Zero { 0.0f, 0.0f };
	const Vector2 Vector2::Unit { 1.0f, 1.0f };
	const Vector2 Vector2::UnitX{ 1.0f, 0.0f };
	const Vector2 Vector2::UnitY{ 0.0f, 1.0f };
}