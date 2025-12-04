export module Matrix4x4;
#include "Core.h"
import std;
import Vector4;
import Logger;

static constexpr int s_size = 3;

export namespace NSL
{
	class NSL_API Matrix4x4
	{
	public:
		Matrix4x4() = default;
		Matrix4x4(float col0X, float col1X, float col2X, float col3X,
				  float col0Y, float col1Y, float col2Y, float col3Y,
				  float col0Z, float col1Z, float col2Z, float col3Z,
				  float col0W, float col1W, float col2W, float col3W) NSL_NOEXCEPT
			: col0(col0X, col0Y, col0Z, col0W)
			, col1(col1X, col1Y, col1Z, col1W)
			, col2(col2X, col2Y, col2Z, col2W)
			, col3(col3X, col3Y, col3Z, col3W)
		{
		}
		Matrix4x4(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3) NSL_NOEXCEPT
			: col0(col0)
			, col1(col1)
			, col2(col2)
			, col3(col3)
		{
		}

		Matrix4x4 operator*(const Matrix4x4& other) const NSL_NOEXCEPT
		{
#define ax1 col0.x
#define ax2 col1.x
#define ax3 col2.x
#define ax4 col3.x

#define ay1 col0.y
#define ay2 col1.y
#define ay3 col2.y
#define ay4 col3.y

#define az1 col0.z
#define az2 col1.z
#define az3 col2.z
#define az4 col3.z

#define aw1 col0.w
#define aw2 col1.w
#define aw3 col2.w
#define aw4 col3.w


#define bx1 other.col0.x
#define bx2 other.col1.x
#define bx3 other.col2.x
#define bx4 other.col3.x

#define by1 other.col0.y
#define by2 other.col1.y
#define by3 other.col2.y
#define by4 other.col3.y

#define bz1 other.col0.z
#define bz2 other.col1.z
#define bz3 other.col2.z
#define bz4 other.col3.z

#define bw1 other.col0.w
#define bw2 other.col1.w
#define bw3 other.col2.w
#define bw4 other.col3.w


#define resultMatrixCol0 ax1 * bx1 + ax2 * by1 + ax3 * bz1 + ax4 * bw1, \
						 ay1 * bx1 + ay2 * by1 + ay3 * bz1 + ay4 * bw1, \
						 az1 * bx1 + az2 * by1 + az3 * bz1 + az4 * bw1, \
						 aw1 * bx1 + aw2 * by1 + aw3 * bz1 + aw4 * bw1

#define resultMatrixCol1 ax1 * bx2 + ax2 * by2 + ax3 * bz2 + ax4 * bw2, \
						 ay1 * bx2 + ay2 * by2 + ay3 * bz2 + ay4 * bw2, \
						 az1 * bx2 + az2 * by2 + az3 * bz2 + az4 * bw2, \
						 aw1 * bx2 + aw2 * by2 + aw3 * bz2 + aw4 * bw2

#define resultMatrixCol2 ax1 * bx3 + ax2 * by3 + ax3 * bz3 + ax4 * bw3, \
						 ay1 * bx3 + ay2 * by3 + ay3 * bz3 + ay4 * bw3, \
						 az1 * bx3 + az2 * by3 + az3 * bz3 + az4 * bw3, \
						 aw1 * bx3 + aw2 * by3 + aw3 * bz3 + aw4 * bw3

#define resultMatrixCol3 ax1 * bx4 + ax2 * by4 + ax3 * bz4 + ax4 * bw4, \
						 ay1 * bx4 + ay2 * by4 + ay3 * bz4 + ay4 * bw4, \
						 az1 * bx4 + az2 * by4 + az3 * bz4 + az4 * bw4, \
						 aw1 * bx4 + aw2 * by4 + aw3 * bz4 + aw4 * bw4

			return Matrix4x4(Vector4(resultMatrixCol0), Vector4(resultMatrixCol1), Vector4(resultMatrixCol2), Vector4(resultMatrixCol3));
		}
		Vector4 operator*(const Vector4& other) const NSL_NOEXCEPT
		{
			return Vector4(col0.x * other.x + col1.x * other.y + col2.x * other.z + col3.x * other.w,
						   col0.y * other.x + col1.y * other.y + col2.y * other.z + col3.y * other.w,
						   col0.z * other.x + col1.z * other.y + col2.z * other.z + col3.z * other.w,
						   col0.w * other.x + col1.w * other.y + col2.w * other.z + col3.w * other.w);
		}
		Matrix4x4 operator*(float other) const NSL_NOEXCEPT
		{
			return Matrix4x4(col0 * other, col1 * other, col2 * other, col3 * other);
		}
		Matrix4x4 operator-() const NSL_NOEXCEPT
		{
			return Matrix4x4(-col0, -col1, -col2, -col3);
		}
		const Vector4& operator[](int index) const NSL_NOEXCEPT
		{
			static_assert(sizeof(Matrix4x4) == 64, "Unexpected Matrix4x4 size");
			static_assert(alignof(Matrix4x4) == 4, "Unexpected Matrix4x4 alignment");

			NSL_ASSERT(0 <= index && index < s_size, "Invalid index: " + std::to_string(index))
				return *(reinterpret_cast<const Vector4*>(this) + index);
		}
		Vector4& operator[](int index) NSL_NOEXCEPT
		{
			static_assert(sizeof(Matrix4x4) == 64, "Unexpected Matrix4x4 size");
			static_assert(alignof(Matrix4x4) == 4, "Unexpected Matrix4x4 alignment");

			NSL_ASSERT(0 <= index && index < s_size, "Invalid index: " + std::to_string(index))
				return *(reinterpret_cast<Vector4*>(this) + index);
		}

		Vector4 col0, col1, col2, col3;

		static const Matrix4x4 Unit;
		static const Matrix4x4 Zero;
	};

	const Matrix4x4 Matrix4x4::Unit(1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f);

	const Matrix4x4 Matrix4x4::Zero(0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f);
}