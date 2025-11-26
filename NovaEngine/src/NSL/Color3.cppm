export module Color3;
#include "Core.h"
import std;
import Logger;

export namespace NSL
{
	struct NSL_API Color3
	{
		Color3() = default;
		Color3(unsigned char rgb) NSL_NOEXCEPT
			: r(rgb)
			, g(rgb)
			, b(rgb)
		{
		}
		Color3(unsigned char r, unsigned char g, unsigned char b) NSL_NOEXCEPT
			: r(r)
			, g(g)
			, b(b)
		{
		}

		Color3 operator+(const Color3& other) const NSL_NOEXCEPT { Color3 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) + other[i]; } return color; }
		Color3 operator-(const Color3& other) const NSL_NOEXCEPT { Color3 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) - other[i]; } return color; }
		Color3 operator*(const Color3& other) const NSL_NOEXCEPT { Color3 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) * other[i]; } return color; }
		Color3 operator/(const Color3& other) const NSL_NOEXCEPT { Color3 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) / other[i]; } return color; }
		Color3& operator+=(const Color3& other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) += other[i]; }; return *this; }
		Color3& operator-=(const Color3& other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) -= other[i]; }; return *this; }
		Color3& operator*=(const Color3& other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) *= other[i]; }; return *this; }
		Color3& operator/=(const Color3& other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) /= other[i]; }; return *this; }

		Color3 operator+(unsigned char other) const NSL_NOEXCEPT { Color3 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) + other; } return color; }
		Color3 operator-(unsigned char other) const NSL_NOEXCEPT { Color3 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) - other; } return color; }
		Color3 operator*(unsigned char other) const NSL_NOEXCEPT { Color3 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) * other; } return color; }
		Color3 operator/(unsigned char other) const NSL_NOEXCEPT { Color3 color; for (int i = 0; i < size; ++i) { color[i] = this->operator[](i) / other; } return color; }
		Color3& operator+=(unsigned char other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) += other; }; return *this; }
		Color3& operator-=(unsigned char other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) -= other; }; return *this; }
		Color3& operator*=(unsigned char other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) *= other; }; return *this; }
		Color3& operator/=(unsigned char other) NSL_NOEXCEPT { for (int i = 0; i < size; ++i) { this->operator[](i) /= other; }; return *this; }

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

		static const int size = 3;
		union
		{
			unsigned char components[size];
			struct
			{
				unsigned char r, g, b;
			};
		};

		static const Color3 Black;
		static const Color3 White;
	};

	const Color3 Color3::Black(0, 0, 0);
	const Color3 Color3::White(255, 255, 255);
}