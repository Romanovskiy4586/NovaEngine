export module PixelRGB;
#include "Core.h"

export namespace NSL
{
	struct NSL_API PixelRGB
	{
		PixelRGB() NSL_NOEXCEPT
			: R(0)
			, G(0)
			, B(0)
		{
		}
		PixelRGB(unsigned char r, unsigned char g, unsigned char b) NSL_NOEXCEPT
			: R(r)
			, G(g)
			, B(b)
		{
		}

		unsigned char R;
		unsigned char G;
		unsigned char B;

		PixelRGB operator+(const PixelRGB& other) const NSL_NOEXCEPT { return PixelRGB(static_cast<unsigned char>(R + other.R), static_cast<unsigned char>(G + other.G), static_cast<unsigned char>(B + other.B)); }
		PixelRGB operator-(const PixelRGB& other) const NSL_NOEXCEPT { return PixelRGB(static_cast<unsigned char>(R - other.R), static_cast<unsigned char>(G - other.G), static_cast<unsigned char>(B - other.B)); }
		PixelRGB operator*(const PixelRGB& other) const NSL_NOEXCEPT { return PixelRGB(static_cast<unsigned char>(R * other.R), static_cast<unsigned char>(G * other.G), static_cast<unsigned char>(B * other.B)); }
		PixelRGB operator/(const PixelRGB& other) const NSL_NOEXCEPT { return PixelRGB(static_cast<unsigned char>(R / other.R), static_cast<unsigned char>(G / other.G), static_cast<unsigned char>(B / other.B)); }
		PixelRGB& operator+=(const PixelRGB& other) NSL_NOEXCEPT     { static_cast<unsigned char>(R += other.R); static_cast<unsigned char>(G += other.G); static_cast<unsigned char>(B += other.B); return *this; }
		PixelRGB& operator-=(const PixelRGB& other) NSL_NOEXCEPT     { static_cast<unsigned char>(R -= other.R); static_cast<unsigned char>(G -= other.G); static_cast<unsigned char>(B -= other.B); return *this; }
		PixelRGB& operator*=(const PixelRGB& other) NSL_NOEXCEPT     { static_cast<unsigned char>(R *= other.R); static_cast<unsigned char>(G *= other.G); static_cast<unsigned char>(B *= other.B); return *this; }
		PixelRGB& operator/=(const PixelRGB& other) NSL_NOEXCEPT     { static_cast<unsigned char>(R /= other.R); static_cast<unsigned char>(G /= other.G); static_cast<unsigned char>(B /= other.B); return *this; }

		PixelRGB operator+(unsigned char other) const NSL_NOEXCEPT { return PixelRGB(static_cast<unsigned char>(R + other), static_cast<unsigned char>(G + other), static_cast<unsigned char>(B + other)); }
		PixelRGB operator-(unsigned char other) const NSL_NOEXCEPT { return PixelRGB(static_cast<unsigned char>(R - other), static_cast<unsigned char>(G - other), static_cast<unsigned char>(B - other)); }
		PixelRGB operator*(unsigned char other) const NSL_NOEXCEPT { return PixelRGB(static_cast<unsigned char>(R * other), static_cast<unsigned char>(G * other), static_cast<unsigned char>(B * other)); }
		PixelRGB operator/(unsigned char other) const NSL_NOEXCEPT { return PixelRGB(static_cast<unsigned char>(R / other), static_cast<unsigned char>(G / other), static_cast<unsigned char>(B / other)); }
		PixelRGB& operator+=(unsigned char other) NSL_NOEXCEPT     { static_cast<unsigned char>(R += other); static_cast<unsigned char>(G += other); static_cast<unsigned char>(B += other); return *this; }
		PixelRGB& operator-=(unsigned char other) NSL_NOEXCEPT     { static_cast<unsigned char>(R -= other); static_cast<unsigned char>(G -= other); static_cast<unsigned char>(B -= other); return *this; }
		PixelRGB& operator*=(unsigned char other) NSL_NOEXCEPT     { static_cast<unsigned char>(R *= other); static_cast<unsigned char>(G *= other); static_cast<unsigned char>(B *= other); return *this; }
		PixelRGB& operator/=(unsigned char other) NSL_NOEXCEPT     { static_cast<unsigned char>(R /= other); static_cast<unsigned char>(G /= other); static_cast<unsigned char>(B /= other); return *this; }

		PixelRGB operator-() const NSL_NOEXCEPT { return PixelRGB(static_cast<unsigned char>(-R), static_cast<unsigned char>(-G), static_cast<unsigned char>(-B)); }

		bool operator==(const PixelRGB& other) NSL_NOEXCEPT { return R == other.R && G == other.G && B == other.B; }
		bool operator!=(const PixelRGB& other) NSL_NOEXCEPT { return !(*this == other); }

		bool operator>(const PixelRGB& other) NSL_NOEXCEPT { return R > other.R && G > other.G && B > other.B; }
		bool operator>=(const PixelRGB& other) NSL_NOEXCEPT { return R >= other.R && G >= other.G && B >= other.B; }

		bool operator<(const PixelRGB& other) NSL_NOEXCEPT { return R < other.R && G < other.G && B < other.B; }
		bool operator<=(const PixelRGB& other) NSL_NOEXCEPT { return R <= other.R && G <= other.G && B <= other.B; }
	};
}