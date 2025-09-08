export module Matrix2x2;
#include "Core.h"
import Vector2;

export namespace NSL
{
	class NSL_API Matrix2x2
	{
	public:
		Matrix2x2() = default;
		Matrix2x2(float col0X, float col1X,
				  float col0Y, float col1Y) NSL_NOEXCEPT
			: col0(col0X, col0Y)
			, col1(col1X, col1Y)
		{
		}
		Matrix2x2(const Vector2& col0, const Vector2& col1) NSL_NOEXCEPT
			: col0(col0)
			, col1(col1)
		{
		}

		Matrix2x2 operator*(const Matrix2x2& other) const NSL_NOEXCEPT
		{
#define ax1 col0.x
#define ax2 col1.x

#define ay1 col0.y
#define ay2 col1.y


#define bx1 other.col0.x
#define bx2 other.col1.x

#define by1 other.col0.y
#define by2 other.col1.y


#define resultMatrixCol0 ax1 * bx1 + ax2 * by1, \
						 ay1 * bx1 + ay2 * by1

#define resultMatrixCol1 ax1 * bx2 + ax2 * by2, \
						 ay1 * bx2 + ay2 * by2

			return Matrix2x2(Vector2(resultMatrixCol0), Vector2(resultMatrixCol1));
		}
		Vector2 operator*(const Vector2& other) const NSL_NOEXCEPT
		{
			return Vector2(col0.x * other.x + col1.x * other.y,
				col0.y * other.x + col1.y * other.y);
		}
		Matrix2x2 operator*(float other) const NSL_NOEXCEPT
		{
			return Matrix2x2(col0 * other, col1 * other);
		}
		Matrix2x2 operator-() const NSL_NOEXCEPT
		{
			return Matrix2x2(-col0, -col1);
		}

		static const int size = 2;
		union
		{
			Vector2 components[size];
			struct
			{
				Vector2 col0, col1;
			};
		};

		static const Matrix2x2 Unit;
		static const Matrix2x2 Zero;
	};

	const Matrix2x2 Matrix2x2::Unit(1.0f, 0.0f,
									0.0f, 1.0f);

	const Matrix2x2 Matrix2x2::Zero(0.0f, 0.0f,
									0.0f, 0.0f);
}