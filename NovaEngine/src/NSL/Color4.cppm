export module Color4;
#include "Core.h"
import std;
import Logger;

export namespace NSL
{
	struct NSL_API Color4
	{
		Color4() = default;
		Color4(unsigned char rgba) NSL_NOEXCEPT
			: r(rgba)
			, g(rgba)
			, b(rgba)
			, a(rgba)
		{
		}
		Color4(unsigned char r, unsigned char g, unsigned char b, unsigned char a) NSL_NOEXCEPT
			: r(r)
			, g(g)
			, b(b)
			, a(a)
		{
		}

		Color4 operator+(const Color4& other) const NSL_NOEXCEPT { Color4 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) + other[i]; } return color; }
		Color4 operator-(const Color4& other) const NSL_NOEXCEPT { Color4 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) - other[i]; } return color; }
		Color4 operator*(const Color4& other) const NSL_NOEXCEPT { Color4 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) * other[i]; } return color; }
		Color4 operator/(const Color4& other) const NSL_NOEXCEPT { Color4 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) / other[i]; } return color; }
		Color4& operator+=(const Color4& other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) += other[i]; }; return *this; }
		Color4& operator-=(const Color4& other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) -= other[i]; }; return *this; }
		Color4& operator*=(const Color4& other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) *= other[i]; }; return *this; }
		Color4& operator/=(const Color4& other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) /= other[i]; }; return *this; }

		Color4 operator+(unsigned char other) const NSL_NOEXCEPT { Color4 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) + other; } return color; }
		Color4 operator-(unsigned char other) const NSL_NOEXCEPT { Color4 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) - other; } return color; }
		Color4 operator*(unsigned char other) const NSL_NOEXCEPT { Color4 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) * other; } return color; }
		Color4 operator/(unsigned char other) const NSL_NOEXCEPT { Color4 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) / other; } return color; }
		Color4& operator+=(unsigned char other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) += other; }; return *this; }
		Color4& operator-=(unsigned char other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) -= other; }; return *this; }
		Color4& operator*=(unsigned char other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) *= other; }; return *this; }
		Color4& operator/=(unsigned char other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) /= other; }; return *this; }

		const unsigned char& operator[](int index) const NSL_NOEXCEPT
		{
			NSL_ASSERT(0 <= index && index <= size - 1, "Invalid index: " + std::to_string(index))
			return components[index];
		}
		unsigned char& operator[](int index) NSL_NOEXCEPT
		{
			NSL_ASSERT(0 <= index && index <= size - 1, "Invalid index: " + std::to_string(index))
			return components[index];
		}

		static const int size = 4;
		union
		{
			unsigned char components[size];
			struct
			{
				unsigned char r, g, b, a;
			};
		};

		static const Color4 Black;
		static const Color4 White;
	};

	const Color4 Color4::Black(0, 0, 0, 255);
	const Color4 Color4::White(255, 255, 255, 255);
}