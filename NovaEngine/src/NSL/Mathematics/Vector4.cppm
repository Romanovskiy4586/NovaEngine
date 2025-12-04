export module Vector4;
#include "Core.h"
import std;
import Mathematics.Constants;
import Logger;

static constexpr int s_size = 4;

export namespace NSL
{
	struct NSL_API Vector4 final
	{
		Vector4() = default;
		explicit Vector4(float xyzw) NSL_NOEXCEPT
			: x(xyzw)
			, y(xyzw)
			, z(xyzw)
			, w(xyzw)
		{
		}
		Vector4(float x, float y, float z, float w) NSL_NOEXCEPT
			: x(x)
			, y(y)
			, z(z)
			, w(w)
		{
		}

		Vector4 operator+(const Vector4& other) const NSL_NOEXCEPT
		{
			return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
		}
		Vector4 operator-(const Vector4& other) const NSL_NOEXCEPT
		{
			return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
		}
		Vector4 operator*(const Vector4& other) const NSL_NOEXCEPT
		{
			return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
		}
		Vector4 operator/(const Vector4& other) const NSL_NOEXCEPT
		{
			return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
		}
		Vector4& operator+=(const Vector4& other) NSL_NOEXCEPT
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		Vector4& operator-=(const Vector4& other) NSL_NOEXCEPT
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		Vector4& operator*=(const Vector4& other) NSL_NOEXCEPT
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}
		Vector4& operator/=(const Vector4& other) NSL_NOEXCEPT
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
			return *this;
		}

		Vector4 operator+(float other) const NSL_NOEXCEPT
		{
			return Vector4(x + other, y + other, z + other, w + other);
		}
		Vector4 operator-(float other) const NSL_NOEXCEPT
		{
			return Vector4(x - other, y - other, z - other, w - other);
		}
		Vector4 operator*(float other) const NSL_NOEXCEPT
		{
			return Vector4(x * other, y * other, z * other, w * other);
		}
		Vector4 operator/(float other) const NSL_NOEXCEPT
		{
			return Vector4(x / other, y / other, z / other, w / other);
		}
		Vector4& operator+=(float other) NSL_NOEXCEPT
		{
			x += other;
			y += other;
			z += other;
			w += other;
			return *this;
		}
		Vector4& operator-=(float other) NSL_NOEXCEPT
		{
			x -= other;
			y -= other;
			z -= other;
			w -= other;
			return *this;
		}
		Vector4& operator*=(float other) NSL_NOEXCEPT
		{
			x *= other;
			y *= other;
			z *= other;
			w *= other;
			return *this;
		}
		Vector4& operator/=(float other) NSL_NOEXCEPT
		{
			x /= other;
			y /= other;
			z /= other;
			w /= other;
			return *this;
		}

		const float& operator[](int index) const NSL_NOEXCEPT
		{
			static_assert(sizeof(Vector4) == 16, "Unexpected Vector4 size");
			static_assert(alignof(Vector4) == 4, "Unexpected Vector4 alignment");

			NSL_ASSERT(0 <= index && index < s_size, "Invalid index: " + std::to_string(index))
			return *(reinterpret_cast<const float*>(this) + index);
		}
		float& operator[](int index) NSL_NOEXCEPT
		{
			static_assert(sizeof(Vector4) == 16, "Unexpected Vector4 size");
			static_assert(alignof(Vector4) == 4, "Unexpected Vector4 alignment");

			NSL_ASSERT(0 <= index && index < s_size, "Invalid index: " + std::to_string(index))
			return *(reinterpret_cast<float*>(this) + index);
		}
		Vector4 operator-() const NSL_NOEXCEPT
		{
			return Vector4(-x, -y, -z, -w);
		}
		bool operator==(const Vector4& other) const NSL_NOEXCEPT
		{
			return std::fabs(x - other.x) <= MATHF_EPSILON && std::fabs(y - other.y) <= MATHF_EPSILON && std::fabs(z - other.z) <= MATHF_EPSILON && std::fabs(w - other.w) <= MATHF_EPSILON;
		}
		bool operator!=(const Vector4& other) const NSL_NOEXCEPT
		{
			return !(*this == other);
		}

		float x, y, z, w;

		static const Vector4 Zero;
		static const Vector4 Unit;
		static const Vector4 UnitX;
		static const Vector4 UnitY;
		static const Vector4 UnitZ;
		static const Vector4 UnitW;
	};

	const Vector4 Vector4::Zero { 0.0f, 0.0f, 0.0f, 0.0f };
	const Vector4 Vector4::Unit { 1.0f, 1.0f, 1.0f, 1.0f };
	const Vector4 Vector4::UnitX{ 1.0f, 0.0f, 0.0f, 0.0f };
	const Vector4 Vector4::UnitY{ 0.0f, 1.0f, 0.0f, 0.0f };
	const Vector4 Vector4::UnitZ{ 0.0f, 0.0f, 1.0f, 0.0f };
	const Vector4 Vector4::UnitW{ 0.0f, 0.0f, 0.0f, 1.0f };
}