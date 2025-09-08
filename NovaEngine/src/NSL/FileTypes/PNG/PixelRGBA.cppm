export module PixelRGBA;
#include "Core.h"

export namespace NSL
{
	struct NSL_API PixelRGBA
	{
		unsigned char R;
		unsigned char G;
		unsigned char B;
		unsigned char A;

		PixelRGBA operator+(const PixelRGBA& other) const NSL_NOEXCEPT	{ return PixelRGBA(static_cast<unsigned char>(R + other.R), static_cast<unsigned char>(G + other.G), static_cast<unsigned char>(B + other.B), static_cast<unsigned char>(A + other.A)); }
		PixelRGBA operator-(const PixelRGBA& other) const NSL_NOEXCEPT	{ return PixelRGBA(static_cast<unsigned char>(R - other.R), static_cast<unsigned char>(G - other.G), static_cast<unsigned char>(B - other.B), static_cast<unsigned char>(A - other.A)); }
		PixelRGBA operator*(const PixelRGBA& other) const NSL_NOEXCEPT	{ return PixelRGBA(static_cast<unsigned char>(R * other.R), static_cast<unsigned char>(G * other.G), static_cast<unsigned char>(B * other.B), static_cast<unsigned char>(A * other.A)); }
		PixelRGBA operator/(const PixelRGBA& other) const NSL_NOEXCEPT	{ return PixelRGBA(static_cast<unsigned char>(R / other.R), static_cast<unsigned char>(G / other.G), static_cast<unsigned char>(B / other.B), static_cast<unsigned char>(A / other.A)); }
		PixelRGBA& operator+=(const PixelRGBA& other) NSL_NOEXCEPT		{ static_cast<unsigned char>(R += other.R); static_cast<unsigned char>(G += other.G); static_cast<unsigned char>(B += other.B); static_cast<unsigned char>(A += other.A); return *this; }
		PixelRGBA& operator-=(const PixelRGBA& other) NSL_NOEXCEPT		{ static_cast<unsigned char>(R -= other.R); static_cast<unsigned char>(G -= other.G); static_cast<unsigned char>(B -= other.B); static_cast<unsigned char>(A -= other.A); return *this; }
		PixelRGBA& operator*=(const PixelRGBA& other) NSL_NOEXCEPT		{ static_cast<unsigned char>(R *= other.R); static_cast<unsigned char>(G *= other.G); static_cast<unsigned char>(B *= other.B); static_cast<unsigned char>(A *= other.A); return *this; }
		PixelRGBA& operator/=(const PixelRGBA& other) NSL_NOEXCEPT		{ static_cast<unsigned char>(R /= other.R); static_cast<unsigned char>(G /= other.G); static_cast<unsigned char>(B /= other.B); static_cast<unsigned char>(A /= other.A); return *this; }

		PixelRGBA operator+(unsigned char other) const NSL_NOEXCEPT { return PixelRGBA(static_cast<unsigned char>(R + other), static_cast<unsigned char>(G + other), static_cast<unsigned char>(B + other), static_cast<unsigned char>(A + other)); }
		PixelRGBA operator-(unsigned char other) const NSL_NOEXCEPT { return PixelRGBA(static_cast<unsigned char>(R - other), static_cast<unsigned char>(G - other), static_cast<unsigned char>(B - other), static_cast<unsigned char>(A - other)); }
		PixelRGBA operator*(unsigned char other) const NSL_NOEXCEPT { return PixelRGBA(static_cast<unsigned char>(R * other), static_cast<unsigned char>(G * other), static_cast<unsigned char>(B * other), static_cast<unsigned char>(A * other)); }
		PixelRGBA operator/(unsigned char other) const NSL_NOEXCEPT { return PixelRGBA(static_cast<unsigned char>(R / other), static_cast<unsigned char>(G / other), static_cast<unsigned char>(B / other), static_cast<unsigned char>(A / other)); }
		PixelRGBA& operator+=(unsigned char other) NSL_NOEXCEPT		{ static_cast<unsigned char>(R += other); static_cast<unsigned char>(G += other); static_cast<unsigned char>(B += other); static_cast<unsigned char>(A += other); return *this; }
		PixelRGBA& operator-=(unsigned char other) NSL_NOEXCEPT		{ static_cast<unsigned char>(R -= other); static_cast<unsigned char>(G -= other); static_cast<unsigned char>(B -= other); static_cast<unsigned char>(A -= other); return *this; }
		PixelRGBA& operator*=(unsigned char other) NSL_NOEXCEPT		{ static_cast<unsigned char>(R *= other); static_cast<unsigned char>(G *= other); static_cast<unsigned char>(B *= other); static_cast<unsigned char>(A *= other); return *this; }
		PixelRGBA& operator/=(unsigned char other) NSL_NOEXCEPT		{ static_cast<unsigned char>(R /= other); static_cast<unsigned char>(G /= other); static_cast<unsigned char>(B /= other); static_cast<unsigned char>(A /= other); return *this; }

		PixelRGBA operator-() const NSL_NOEXCEPT { return PixelRGBA(static_cast<unsigned char>(-R), static_cast<unsigned char>(-G), static_cast<unsigned char>(-B), static_cast<unsigned char>(-A)); }

		bool operator==(const PixelRGBA& other) NSL_NOEXCEPT { return R == other.R && G == other.G && B == other.B && A == other.A; }
		bool operator!=(const PixelRGBA& other) NSL_NOEXCEPT { return !(*this == other); }

		bool operator>(const PixelRGBA& other) NSL_NOEXCEPT { return R > other.R && G > other.G && B > other.B && A > other.A; }
		bool operator>=(const PixelRGBA& other) NSL_NOEXCEPT { return R >= other.R && G >= other.G && B >= other.B && A >= other.A; }

		bool operator<(const PixelRGBA& other) NSL_NOEXCEPT { return R < other.R && G < other.G && B < other.B && A < other.A; }
		bool operator<=(const PixelRGBA& other) NSL_NOEXCEPT { return R <= other.R && G <= other.G && B <= other.B && A <= other.A; }
	};
}