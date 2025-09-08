export module Vector3;
#include "Core.h"
import std;
import Mathematics.Constants;
import Logger;

export namespace NSL
{
	struct NSL_API Vector3 final
	{
		Vector3() = default;
		explicit Vector3(float xyz) NSL_NOEXCEPT
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
			NSL_ASSERT(0 <= index && index <= size - 1, "Invalid index: " + std::to_string(index))
			return components[index];
		}
		float& operator[](int index) NSL_NOEXCEPT
		{
			NSL_ASSERT(0 <= index && index <= size - 1, "Invalid index: " + std::to_string(index))
			return components[index];
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

		static const int size = 3;
		union
		{
			float components[size];
			struct
			{
				float x, y, z;
			};
		};

		static const Vector3 Zero;
		static const Vector3 Unit;
		static const Vector3 UnitX;
		static const Vector3 UnitY;
		static const Vector3 UnitZ;
	};

	const Vector3 Vector3::Zero{ 0.0f, 0.0f, 0.0f};
	const Vector3 Vector3::Unit{ 1.0f, 1.0f, 1.0f};
	const Vector3 Vector3::UnitX{ 1.0f, 0.0f, 0.0f};
	const Vector3 Vector3::UnitY{ 0.0f, 1.0f, 0.0f};
	const Vector3 Vector3::UnitZ{ 0.0f, 0.0f, 1.0f};
}