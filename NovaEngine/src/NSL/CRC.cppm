export module CRC;
#include "Core.h"
import std;

export namespace NSL
{
	class NSL_API CRC
	{
	public:
		CRC() NSL_NOEXCEPT
		{
			unsigned int c;

			for (unsigned int n = 0; n < 256; n++)
			{
				c = n;
				for (int k = 0; k < 8; k++)
				{
					c = (c & 1) ? (0xedb88320U ^ (c >> 1)) : (c >> 1);
				}
				_crcTable[n] = c;
			}
		}

		unsigned int operator()(const void* const buf, int len) NSL_NOEXCEPT
		{
			unsigned int c = 0xffffffffU;
			const unsigned char* const ptr = static_cast<const unsigned char* const>(buf);

			for (int n = 0; n < len; n++)
			{
				c = _crcTable[(c ^ ptr[n]) & 0xffU] ^ (c >> 8);
			}

			return c ^ 0xffffffffU;
		}

	private:
		unsigned int _crcTable[256];
	};
}