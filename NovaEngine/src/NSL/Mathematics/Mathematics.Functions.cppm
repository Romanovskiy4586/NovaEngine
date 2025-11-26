export module Mathematics.Functions;
#include "Core.h"
import std;
import Mathematics.Constants;
import Matrix2x2;
import Matrix3x3;
import Matrix4x4;
import Vector2;
import Vector3;
import Vector4;

export namespace NSL
{
	template <class T>
	T Random(const T& min, const T& max) NSL_NOEXCEPT
	{
		thread_local std::mt19937 gen;
		thread_local bool initialized = false;

		if (!initialized)
		{
			gen.seed(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
			initialized = true;
		}

		if constexpr (std::is_integral<T>::value)
		{
			std::uniform_int_distribution<T> dstr(min, max);
			return dstr(gen);
		}
		else
		{
			std::uniform_real_distribution<T> dstr(min, max);
			return dstr(gen);
		}
	}
	template <class T>
	void Shuffle(std::vector<T>& vector) NSL_NOEXCEPT
	{
		thread_local std::mt19937 gen;
		thread_local bool initialized = false;

		if (!initialized)
		{
			gen.seed(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
			initialized = true;
		}

		std::shuffle(vector, gen);
	}
	template <class T>
	inline constexpr T Clamp(const T& value, const T& min, const T& max) NSL_NOEXCEPT
	{
		return value < min ? min : value > max ? max : value;
	}
	template <class T>
	inline constexpr T RoundUp(const T& value) NSL_NOEXCEPT
	{
		return std::ceil(value);
	}
	template <class T>
	inline constexpr T RoundDown(const T& value) NSL_NOEXCEPT
	{
		return std::floor(value);
	}
	template <class T>
	inline constexpr T Round(const T& value) NSL_NOEXCEPT
	{
		return std::round(value);
	}
	template <class T>
	inline constexpr bool InRangeIncluded(const T& value, const T& min, const T& max) NSL_NOEXCEPT
	{
		return min <= value && value <= max;
	}
	template <class T>
	inline constexpr bool InRangeExcluded(const T& value, const T& min, const T& max) NSL_NOEXCEPT
	{
		return min < value && value < max;
	}
	template <class T>
	inline constexpr T Min(const T& first, const T& second) NSL_NOEXCEPT
	{
		if (first < second) return first;
		else				return second;
	}
	template <class T>
	inline constexpr T Max(const T& first, const T& second) NSL_NOEXCEPT
	{
		if (first > second) return first;
		else				return second;
	}

	std::array<float, 16> GetRawData(const Matrix4x4& value) NSL_NOEXCEPT
	{
		return
		{
			value.col0.x, value.col0.y, value.col0.z, value.col0.w,
			value.col1.x, value.col1.y, value.col1.z, value.col1.w,
			value.col2.x, value.col2.y, value.col2.z, value.col2.w,
			value.col3.x, value.col3.y, value.col3.z, value.col3.w,
		};
		//   _				   _
		//	|					|
		//	|	1	5	9	13	|
		//	|	2	6	10	14	|
		//	|	3	7	11	15	|
		//	|	4	8	12	16	|
		//	|_				   _|
	}
	std::array<float, 9> GetRawData(const Matrix3x3& value) NSL_NOEXCEPT
	{
		return
		{
			value.col0.x, value.col0.y, value.col0.z,
			value.col1.x, value.col1.y, value.col1.z,
			value.col2.x, value.col2.y, value.col2.z
		};
		//   _			   _
		//	|				|
		//	|	1	4	7	|
		//	|	2	5	8	|
		//	|	3	6	9	|
		//	|_			   _|
	}
	float DiameterToCircumference(float diameter) NSL_NOEXCEPT
	{
		return diameter * MATHF_PI;
	}
	float CircumferenceToDiameter(float circumference) NSL_NOEXCEPT
	{
		return circumference / MATHF_PI;
	}
	Vector3 Cross(const Vector3& left, const Vector3& right) NSL_NOEXCEPT
	{
		Vector3 result;

		result.x = left.y * right.z - left.z * right.y;
		result.y = left.z * right.x - left.x * right.z;
		result.z = left.x * right.y - left.y * right.x;

		return result;
	}
	float Dot(const Vector3& left, const Vector3& right) NSL_NOEXCEPT
	{
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}
	float Distance(const Vector3& left, const Vector3& right) NSL_NOEXCEPT
	{
		Vector3 rightMinusLeft = right - left;
		return std::sqrtf(std::powf(rightMinusLeft.x, 2.0f) + std::powf(rightMinusLeft.y, 2.0f) + std::powf(rightMinusLeft.z, 2.0f));
	}
	float Distance(const Vector2& left, const Vector2& right) NSL_NOEXCEPT
	{
		Vector2 rightMinusLeft = right - left;
		return std::sqrtf(std::powf(rightMinusLeft.x, 2.0f) + std::powf(rightMinusLeft.y, 2.0f));
	}
	float Magnitude(const Vector3& value) NSL_NOEXCEPT
	{
		return std::sqrtf(std::powf(value.x, 2.0f) + std::powf(value.y, 2.0f) + std::powf(value.z, 2.0f));
	}
	float InverseSqrt(float number) NSL_NOEXCEPT
	{
		return 1.0f / sqrt(number);
	}
	Vector3 Normalize(const Vector3& value) NSL_NOEXCEPT
	{
		return value * InverseSqrt(Dot(value, value));
	}
	float AngleInDegrees(const Vector3& left, const Vector3& right) NSL_NOEXCEPT
	{
		float dotProduct = Dot(left, right);
		float leftMagnitude = Magnitude(left);
		float rightMagnitude = Magnitude(right);

		float cosAngle = dotProduct / (leftMagnitude * rightMagnitude);

		return std::acosf(cosAngle);
	}
	float DegreesToRadians(float value) NSL_NOEXCEPT
	{
		return (value * MATHF_PI) / 180.0f;
	}
	float RadiansToDegrees(float value) NSL_NOEXCEPT
	{
		return (value * 180.0f) / MATHF_PI;
	}
	Matrix4x4 Translate(const Matrix4x4& value, const Vector3& position) NSL_NOEXCEPT
	{
		Matrix4x4 result(value);

		result.col3 = value.col0 * position.x + value.col1 * position.y + value.col2 * position.z + value.col3;

		return result;
	}
	Matrix4x4 Rotate(const Matrix4x4& value, float rotationInRadians, const Vector3& axis) NSL_NOEXCEPT
	{
		const float c = cos(rotationInRadians);
		const float s = sin(rotationInRadians);

		Vector3 axisNormalized(Normalize(axis));
		Vector3 temp(axisNormalized * (1.0f - c));

		Matrix4x4 Rotate;
		Rotate.col0.x = c + temp.x * axisNormalized.x;
		Rotate.col0.y = temp.x * axisNormalized.y + s * axisNormalized.z;
		Rotate.col0.z = temp.x * axisNormalized.z - s * axisNormalized.y;

		Rotate.col1.x = temp.y * axisNormalized.y - s * axisNormalized.z;
		Rotate.col1.y = c + temp.y * axisNormalized.y;
		Rotate.col1.z = temp.y * axisNormalized.z + s * axisNormalized.x;

		Rotate.col2.x = temp.z * axisNormalized.x + s * axisNormalized.y;
		Rotate.col2.y = temp.z * axisNormalized.y - s * axisNormalized.x;
		Rotate.col2.z = c + temp.z * axisNormalized.z;

		Matrix4x4 Result;
		Result.col0 = value.col0 * Rotate.col0.x + value.col1 * Rotate.col0.y + value.col2 * Rotate.col0.z;
		Result.col1 = value.col0 * Rotate.col1.x + value.col1 * Rotate.col1.y + value.col2 * Rotate.col1.z;
		Result.col2 = value.col0 * Rotate.col2.x + value.col1 * Rotate.col2.y + value.col2 * Rotate.col2.z;
		Result.col3 = value.col3;
		return Result;
	}
	Matrix4x4 Scale(const Matrix4x4& value, const Vector3& scale) NSL_NOEXCEPT
	{
		Matrix4x4 Result;
		Result.col0 = value.col0 * scale.x;
		Result.col1 = value.col1 * scale.y;
		Result.col2 = value.col2 * scale.z;
		Result.col3 = value.col3;
		return Result;
	}
	Matrix4x4 Transpose(const Matrix4x4& value) NSL_NOEXCEPT
	{
		Matrix4x4 result(value);

		std::swap(result.col0.y, result.col1.x);
		std::swap(result.col0.z, result.col2.x);
		std::swap(result.col0.w, result.col3.x);

		std::swap(result.col1.z, result.col2.y);
		std::swap(result.col1.w, result.col3.y);

		std::swap(result.col2.w, result.col3.z);

		return result;
	}
	Matrix4x4 CreateView(const Vector3& position, const Vector3& forward, const Vector3& up) NSL_NOEXCEPT
	{
		Vector3 const f(Normalize(forward - position));
		Vector3 const s(Normalize(Cross(f, up)));
		Vector3 const u(Cross(s, f));

		Matrix4x4 Result(Matrix4x4::Unit);
		Result.col0.x = s.x;
		Result.col1.x = s.y;
		Result.col2.x = s.z;
		Result.col0.y = u.x;
		Result.col1.y = u.y;
		Result.col2.y = u.z;
		Result.col0.z = -f.x;
		Result.col1.z = -f.y;
		Result.col2.z = -f.z;
		Result.col3.x = -Dot(s, position);
		Result.col3.y = -Dot(u, position);
		Result.col3.z = Dot(f, position);
		return Result;
	}
	Matrix4x4 CreateOrthographic(float width, float height, float depthNear, float depthFar) NSL_NOEXCEPT
	{
		Matrix4x4 result(Matrix4x4::Zero);

		result.col0.x = 1.0f / width;
		result.col1.y = 1.0f / height;
		result.col2.z = -2.0f / (depthFar - depthNear);
		result.col3.z = -((depthFar + depthNear) / (depthFar - depthNear));
		result.col3.w = 1.0f;

		return result;
	}
	Matrix4x4 CreateOrthographic(float left, float right, float top, float bottom, float depthNear, float depthFar) NSL_NOEXCEPT
	{
		Matrix4x4 result(Matrix4x4::Zero);

		result.col0.x = 2.0f / (right - left);
		result.col1.y = 2.0f / (top - bottom);
		result.col2.z = -2.0f / (depthFar - depthNear);
		result.col3.x = -((right + left) / (right - left));
		result.col3.y = -((top + bottom) / (top - bottom));
		result.col3.z = -((depthFar + depthNear) / (depthFar - depthNear));
		result.col3.w = 1.0f;

		return result;
	}
	Matrix4x4 CreatePerspective(float fovInRadians, float aspectRatio, float depthNear, float depthFar) NSL_NOEXCEPT
	{
		Matrix4x4 result(Matrix4x4::Zero);

		result.col0.x = 1.0f / (aspectRatio * std::tanf(fovInRadians / 2.0f));
		result.col1.y = 1.0f / std::tanf(fovInRadians / 2.0f);
		result.col2.z = -((depthFar + depthNear) / (depthFar - depthNear));
		result.col3.z = -((2 * depthFar * depthNear) / (depthFar - depthNear));
		result.col2.w = -1.0f;

		return result;
	}
	Vector3 Unproject(float x, float y, float z, const Matrix4x4& inverseProjectionView) NSL_NOEXCEPT
	{
		Vector4 clipSpace(x, y, z, 1.0f);
		Vector4 eyeSpace = inverseProjectionView * clipSpace;
		eyeSpace /= eyeSpace.w;

		return Vector3(eyeSpace.x, eyeSpace.y, eyeSpace.z);
	}
	float determinant3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3) NSL_NOEXCEPT
	{
		return a1 * (b2 * c3 - b3 * c2) -
			a2 * (b1 * c3 - b3 * c1) +
			a3 * (b1 * c2 - b2 * c1);
	}
	Matrix4x4 Inverse(const Matrix4x4& matrix) NSL_NOEXCEPT
	{
		float det[6][6];

		det[0][0] = determinant3x3(matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[0][1] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[0][2] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[0][3] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z);
		det[0][4] = determinant3x3(matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[0][5] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[1][0] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[1][1] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[1][2] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[1][3] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z);
		det[1][4] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[1][5] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[2][0] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[2][1] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[2][2] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[2][3] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z);
		det[2][4] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z);
		det[2][5] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z,
			matrix.col3.x, matrix.col3.y, matrix.col3.z);
		det[3][0] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z);
		det[3][1] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z);
		det[3][2] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z);
		det[3][3] = determinant3x3(matrix.col0.x, matrix.col0.y, matrix.col0.z,
			matrix.col1.x, matrix.col1.y, matrix.col1.z,
			matrix.col2.x, matrix.col2.y, matrix.col2.z);

		// Вычисляем детерминант исходной матрицы
		float determinant = matrix.col0.x * det[0][0] -
			matrix.col0.y * det[0][1] +
			matrix.col0.z * det[0][2] -
			matrix.col0.w * det[0][3];

		Matrix4x4 inverse = Matrix4x4::Unit;

		// Если детерминант равен нулю, матрица необратима
		if (determinant == 0.0f) return inverse;

		// Вычисляем обратную матрицу
		float invDet = 1.0f / determinant;
		inverse.col0.x = det[0][0] * invDet;
		inverse.col0.y = -det[1][0] * invDet;
		inverse.col0.z = det[2][0] * invDet;
		inverse.col0.w = -det[3][0] * invDet;
		inverse.col1.x = -det[0][1] * invDet;
		inverse.col1.y = det[1][1] * invDet;
		inverse.col1.z = -det[2][1] * invDet;
		inverse.col1.w = det[3][1] * invDet;
		inverse.col2.x = det[0][2] * invDet;
		inverse.col2.y = -det[1][2] * invDet;
		inverse.col2.z = det[2][2] * invDet;
		inverse.col2.w = -det[3][2] * invDet;
		inverse.col3.x = -det[0][3] * invDet;
		inverse.col3.y = det[1][3] * invDet;
		inverse.col3.z = -det[2][3] * invDet;
		inverse.col3.w = det[3][3] * invDet;

		return inverse;
	}
	float MillimetersToInches(float millimeters) NSL_NOEXCEPT
	{
		return millimeters / 25.4f;
	}
	float InchesToMillimeters(float inches) NSL_NOEXCEPT
	{
		return inches * 25.4f;
	}
	float ConvertLinearToSRGB(float value, float gamma = 2.2f) NSL_NOEXCEPT
	{
		return value <= 0.0031308f ? value * 12.92f : std::powf(value, 1.0f / gamma) * 1.055f - 0.055f;
	}
	float ConvertSRGBToLinear(float value, float gamma = 2.2f) NSL_NOEXCEPT
	{
		return value <= 0.04045f ? value / 12.92f : std::powf((value + 0.055f) / 1.055f, gamma);
	}
	Vector3 ConvertLinearToSRGB(const Vector3& value, float gamma = 2.2f) NSL_NOEXCEPT
	{
		Vector3 result;

		result.x = ConvertLinearToSRGB(value.x, gamma);
		result.y = ConvertLinearToSRGB(value.y, gamma);
		result.z = ConvertLinearToSRGB(value.z, gamma);

		return result;
	}
	Vector3 ConvertSRGBToLinear(const Vector3& value, float gamma = 2.2f) NSL_NOEXCEPT
	{
		Vector3 result;

		result.x = ConvertSRGBToLinear(value.x, gamma);
		result.y = ConvertSRGBToLinear(value.y, gamma);
		result.z = ConvertSRGBToLinear(value.z, gamma);

		return result;
	}
}