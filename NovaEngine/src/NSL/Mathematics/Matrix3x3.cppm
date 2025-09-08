export module Matrix3x3;
#include "Core.h"
import Vector3;

export namespace NSL
{
	class NSL_API Matrix3x3
	{
	public:
		Matrix3x3() = default;
		Matrix3x3(float col0X, float col1X, float col2X,
				  float col0Y, float col1Y, float col2Y,
				  float col0Z, float col1Z, float col2Z) NSL_NOEXCEPT
			: col0(col0X, col0Y, col0Z)
			, col1(col1X, col1Y, col1Z)
			, col2(col2X, col2Y, col2Z)
		{
		}
		Matrix3x3(const Vector3& col0, const Vector3& col1, const Vector3& col2) NSL_NOEXCEPT
			: col0(col0)
			, col1(col1)
			, col2(col2)
		{
		}

		Matrix3x3 operator*(const Matrix3x3& other) const NSL_NOEXCEPT
		{
#define ax1 col0.x
#define ax2 col1.x
#define ax3 col2.x

#define ay1 col0.y
#define ay2 col1.y
#define ay3 col2.y

#define az1 col0.z
#define az2 col1.z
#define az3 col2.z


#define bx1 other.col0.x
#define bx2 other.col1.x
#define bx3 other.col2.x

#define by1 other.col0.y
#define by2 other.col1.y
#define by3 other.col2.y

#define bz1 other.col0.z
#define bz2 other.col1.z
#define bz3 other.col2.z


#define resultMatrixCol0 ax1 * bx1 + ax2 * by1 + ax3 * bz1, \
						 ay1 * bx1 + ay2 * by1 + ay3 * bz1, \
						 az1 * bx1 + az2 * by1 + az3 * bz1

#define resultMatrixCol1 ax1 * bx2 + ax2 * by2 + ax3 * bz2, \
						 ay1 * bx2 + ay2 * by2 + ay3 * bz2, \
						 az1 * bx2 + az2 * by2 + az3 * bz2 \

#define resultMatrixCol2 ax1 * bx3 + ax2 * by3 + ax3 * bz3, \
						 ay1 * bx3 + ay2 * by3 + ay3 * bz3, \
						 az1 * bx3 + az2 * by3 + az3 * bz3

			return Matrix3x3(Vector3(resultMatrixCol0), Vector3(resultMatrixCol1), Vector3(resultMatrixCol2));
		}
		Vector3 operator*(const Vector3& other) const NSL_NOEXCEPT
		{
			return Vector3(col0.x * other.x + col1.x * other.y + col2.x * other.z,
				col0.y * other.x + col1.y * other.y + col2.y * other.z,
				col0.z * other.x + col1.z * other.y + col2.z * other.z);
		}
		Matrix3x3 operator*(float other) const NSL_NOEXCEPT
		{
			return Matrix3x3(col0 * other, col1 * other, col2 * other);
		}
		Matrix3x3 operator-() const NSL_NOEXCEPT
		{
			return Matrix3x3(-col0, -col1, -col2);
		}

		static const int size = 3;
		union
		{
			Vector3 components[size];
			struct
			{
				Vector3 col0, col1, col2;
			};
		};

		static const Matrix3x3 Unit;
		static const Matrix3x3 Zero;
	};

	const Matrix3x3 Matrix3x3::Unit(1.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f,
									0.0f, 0.0f, 1.0f);

	const Matrix3x3 Matrix3x3::Zero(0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f);
}