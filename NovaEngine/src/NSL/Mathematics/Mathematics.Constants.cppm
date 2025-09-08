module;
#include <limits>
export module Mathematics.Constants;
#include "Core.h"

export namespace NSL
{
	constexpr double MATH_E = 2.71828182845904523536;			// e
	constexpr double MATH_LOG2E = 1.44269504088896340736;		// log2(e)
	constexpr double MATH_LOG10E = 0.434294481903251827651;		// log10(e)
	constexpr double MATH_LN2 = 0.693147180559945309417;		// ln(2)
	constexpr double MATH_LN10 = 2.30258509299404568402;		// ln(10)
	constexpr double MATH_PI = 3.14159265358979323846;			// pi
	constexpr double MATH_PI_2 = 1.57079632679489661923;		// pi/2
	constexpr double MATH_PI_4 = 0.785398163397448309616;		// pi/4
	constexpr double MATH_1_PI = 0.318309886183790671538;		// 1/pi
	constexpr double MATH_2_PI = 0.636619772367581343076;		// 2/pi
	constexpr double MATH_2_SQRTPI = 1.12837916709551257390;	// 2/sqrt(pi)
	constexpr double MATH_SQRT2 = 1.41421356237309504880;		// sqrt(2)
	constexpr double MATH_SQRT1_2 = 0.707106781186547524401;	// 1/sqrt(2)
	constexpr double MATH_INFINITY = std::numeric_limits<double>::infinity();
	constexpr double MATH_NAN = std::numeric_limits<double>::quiet_NaN();
	constexpr double MATH_DECIMAL_DIG = 17;						// # of decimal digits of rounding precision
	constexpr double MATH_DIG = 15;								// # of decimal digits of precision
	constexpr double MATH_EPSILON = 2.2204460492503131e-016;	// smallest such that 1.0+MATH_EPSILON != 1.0
	constexpr double MATH_MANT_DIG = 53;						// # of bits in mantissa
	constexpr double MATH_MAX = 1.7976931348623158e+308;		// max value
	constexpr double MATH_MAX_10_EXP = 308;						// max decimal exponent
	constexpr double MATH_MAX_EXP = 1024;						// max binary exponent
	constexpr double MATH_MIN = 2.2250738585072014e-308;		// min positive value
	constexpr double MATH_MIN_10_EXP = (-307);					// min decimal exponent
	constexpr double MATH_MIN_EXP = (-1021);					// min binary exponent
	constexpr double MATH_RADIX = 2;							// exponent radix
	constexpr double MATH_TRUE_MIN = 4.9406564584124654e-324;	// min positive value

	constexpr float MATHF_E = 2.71828182845904523536f;			// e
	constexpr float MATHF_LOG2E = 1.44269504088896340736f;		// log2(e)
	constexpr float MATHF_LOG10E = 0.434294481903251827651f;	// log10(e)
	constexpr float MATHF_LN2 = 0.693147180559945309417f;		// ln(2)
	constexpr float MATHF_LN10 = 2.30258509299404568402f;		// ln(10)
	constexpr float MATHF_PI = 3.14159265358979323846f;			// pi
	constexpr float MATHF_PI_2 = 1.57079632679489661923f;		// pi/2
	constexpr float MATHF_PI_4 = 0.785398163397448309616f;		// pi/4
	constexpr float MATHF_1_PI = 0.318309886183790671538f;		// 1/pi
	constexpr float MATHF_2_PI = 0.636619772367581343076f;		// 2/pi
	constexpr float MATHF_2_SQRTPI = 1.12837916709551257390f;	// 2/sqrt(pi)
	constexpr float MATHF_SQRT2 = 1.41421356237309504880f;		// sqrt(2)
	constexpr float MATHF_SQRT1_2 = 0.707106781186547524401f;	// 1/sqrt(2)
	constexpr float MATHF_INFINITY = std::numeric_limits<float>::infinity();
	constexpr float MATHF_NAN = std::numeric_limits<float>::quiet_NaN();
	constexpr float MATHF_DECIMAL_DIG = 9;						// # of decimal digits of rounding precision
	constexpr float MATHF_DIG = 6;								// # of decimal digits of precision
	constexpr float MATHF_EPSILON = 1.192092896e-07F;			// smallest such that 1.0+MATHF_EPSILON != 1.0
	constexpr float MATHF_GUARD = 0;
	constexpr float MATHF_MANT_DIG = 24;						// # of bits in mantissa
	constexpr float MATHF_MAX = 3.402823466e+38F;				// max value
	constexpr float MATHF_MAX_10_EXP = 38;						// max decimal exponent
	constexpr float MATHF_MAX_EXP = 128;						// max binary exponent
	constexpr float MATHF_MIN = 1.175494351e-38F;				// min normalized positive value
	constexpr float MATHF_MIN_10_EXP = (-37);					// min decimal exponent
	constexpr float MATHF_MIN_EXP = (-125);						// min binary exponent
	constexpr float MATHF_RADIX = 2;							// exponent radix
	constexpr float MATHF_TRUE_MIN = 1.401298464e-45F;			// min positive value
}