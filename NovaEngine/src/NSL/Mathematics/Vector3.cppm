export module Vector3;
#include "Core.h"
import std;
import Mathematics.Constants;
import Logger;

static constexpr int s_size = 3;

export namespace NSL
{
	struct NSL_API Vector3 final
	{
		Vector3() = default;
		Vector3(float xyz) NSL_NOEXCEPT
			: x(xyz)
			, y(xyz)
			, z(xyz)
		{
		}
		Vector3(float x, float y, float z) NSL_NOEXCEPT
			: x(x)
			, y(y)
			, z(z)
		{
		}

		Vector3 operator+(const Vector3& other) const NSL_NOEXCEPT
		{
			return Vector3(x + other.x, y + other.y, z + other.z);
		}
		Vector3 operator-(const Vector3& other) const NSL_NOEXCEPT
		{
			return Vector3(x - other.x, y - other.y, z - other.z);
		}
		Vector3 operator*(const Vector3& other) const NSL_NOEXCEPT
		{
			return Vector3(x * other.x, y * other.y, z * other.z);
		}
		Vector3 operator/(const Vector3& other) const NSL_NOEXCEPT
		{
			return Vector3(x / other.x, y / other.y, z / other.z);
		}
		Vector3& operator+=(const Vector3& other) NSL_NOEXCEPT
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		Vector3& operator-=(const Vector3& other) NSL_NOEXCEPT
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		Vector3& operator*=(const Vector3& other) NSL_NOEXCEPT
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}
		Vector3& operator/=(const Vector3& other) NSL_NOEXCEPT
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}

		Vector3 operator+(float other) const NSL_NOEXCEPT
		{
			return Vector3(x + other, y + other, z + other);
		}
		Vector3 operator-(float other) const NSL_NOEXCEPT
		{
			return Vector3(x - other, y - other, z - other);
		}
		Vector3 operator*(float other) const NSL_NOEXCEPT
		{
			return Vector3(x * other, y * other, z * other);
		}
		Vector3 operator/(float other) const NSL_NOEXCEPT
		{
			return Vector3(x / other, y / other, z / other);
		}
		Vector3& operator+=(float other) NSL_NOEXCEPT
		{
			x += other;
			y += other;
			z += other;
			return *this;
		}
		Vector3& operator-=(float other) NSL_NOEXCEPT
		{
			x -= other;
			y -= other;
			z -= other;
			return *this;
		}
		Vector3& operator*=(float other) NSL_NOEXCEPT
		{
			x *= other;
			y *= other;
			z *= other;
			return *this;
		}
		Vector3& operator/=(float other) NSL_NOEXCEPT
		{
			x /= other;
			y /= other;
			z /= other;
			return *this;
		}

		const float& operator[](int index) const NSL_NOEXCEPT
		{
			static_assert(sizeof(Vector3) == 12, "Unexpected Vector3 size");
			static_assert(alignof(Vector3) == 4, "Unexpected Vector3 alignment");

			NSL_ASSERT(0 <= index && index < s_size, "Invalid index: " + std::to_string(index))
			return *(reinterpret_cast<const float*>(this) + index);
		}
		float& operator[](int index) NSL_NOEXCEPT
		{
			static_assert(sizeof(Vector3) == 12, "Unexpected Vector3 size");
			static_assert(alignof(Vector3) == 4, "Unexpected Vector3 alignment");

			NSL_ASSERT(0 <= index && index < s_size, "Invalid index: " + std::to_string(index))
			return *(reinterpret_cast<float*>(this) + index);
		}
		Vector3 operator-() const NSL_NOEXCEPT
		{
			return Vector3(-x, -y, -z);
		}
		bool operator==(const Vector3& other) const NSL_NOEXCEPT
		{
			return std::fabs(x - other.x) <= MATHF_EPSILON && std::fabs(y - other.y) <= MATHF_EPSILON && std::fabs(z - other.z) <= MATHF_EPSILON;
		}
		bool operator!=(const Vector3& other) const NSL_NOEXCEPT
		{
			return !(*this == other);
		}

		float x, y, z;

		static const Vector3 Zero;
		static const Vector3 Unit;
		static const Vector3 UnitX;
		static const Vector3 UnitY;
		static const Vector3 UnitZ;
	};

	const Vector3 Vector3::Zero { 0.0f, 0.0f, 0.0f };
	const Vector3 Vector3::Unit { 1.0f, 1.0f, 1.0f };
	const Vector3 Vector3::UnitX{ 1.0f, 0.0f, 0.0f };
	const Vector3 Vector3::UnitY{ 0.0f, 1.0f, 0.0f };
	const Vector3 Vector3::UnitZ{ 0.0f, 0.0f, 1.0f };
}