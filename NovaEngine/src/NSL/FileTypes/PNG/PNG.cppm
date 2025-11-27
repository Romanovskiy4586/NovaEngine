module;
#include "zlib.h"
export module PNG;
#include "Core.h"
import std;
import NSL.Functions;
import NSL.BinaryStream;
import Mathematics;
import Logger;
import CRC;
import Color3;

export namespace NSL
{
	struct NSL_API PNG
	{
		enum Channels : unsigned char
		{
			Red = 1, RG = 2, RGB = 3, RGBA = 4
		};
		struct Scanline
		{
			enum class Filtering : unsigned char
			{
				None = 0, Sub = 1, Up = 2, Average = 3, Paeth = 4
			};
			Scanline() = default;
			Scanline(size_t size) NSL_NOEXCEPT
				: filtering(Filtering::None)
				, scanline(size)
			{
			}
			Filtering filtering;
			std::vector<unsigned char> scanline;
		};

		PNG() NSL_NOEXCEPT
			: scanlines()
			, size(NSL::Vector2i::Unit)
			, gamma(0.0f)
			, channels(Channels::Red)
			, srgb(false)
		{

		}

		static PNG Load(const std::string& pngPath) NSL_NOEXCEPT
		{
			NSL::BinaryStream binaryStream = NSL::ReadBinaryFile(pngPath);
			return _Parse(binaryStream);
		}
		static PNG Parse(const std::string& pngContent) NSL_NOEXCEPT
		{
			NSL::BinaryStream binaryStream = pngContent;
			return _Parse(binaryStream);
		}

		std::vector<unsigned char> GetPixelsData() const NSL_NOEXCEPT
		{
			std::vector<unsigned char> result;

			for (size_t i = 0; i < scanlines.size(); ++i)
			{
				result.insert(result.end(), scanlines[i].scanline.begin(), scanlines[i].scanline.end());
			}

			return result;
		}

		std::vector<Scanline> scanlines;
		NSL::Vector2i size;
		float gamma;
		Channels channels;
		bool srgb;

	private:
		struct _IHDR
		{
			enum class ColorType : unsigned char
			{
				Grayscale = 0,
				RGB = 2,
				PaletteIndex = 3,
				GrayscaleAlpha = 4,
				RGBA = 6
			};

			unsigned int width = 0;
			unsigned int height = 0;
			unsigned char bitDepth = 0;
			ColorType colorType = ColorType::Grayscale;
			unsigned char compression = 0;
			unsigned char filter = 0;
			unsigned char interlace = 0;
		};
		struct _PLTE
		{
			std::vector<NSL::Color3> colors;
		};
		struct _gAMA
		{
			unsigned int gamma = 0;
		};
		struct _cHRM
		{
			unsigned int whitePointX = 0;
			unsigned int whitePointY = 0;
			unsigned int RedX = 0;
			unsigned int RedY = 0;
			unsigned int GreenX = 0;
			unsigned int GreenY = 0;
			unsigned int BlueX = 0;
			unsigned int BlueY = 0;
		};
		struct _sRGB
		{
			enum class Mode : unsigned char
			{
				Perceptual = 0,
				RelativeColorimetric = 1,
				Saturation = 2,
				AbsoluteColorimetric = 3,
				None = 255U
			};
			Mode mode = Mode::None;
		};
		struct _Chunk
		{
			std::string type;
			std::string data;
			unsigned int crc = 0;
		};

	private:
		static PNG _Parse(NSL::BinaryStream& binaryStream) NSL_NOEXCEPT
		{
			PNG result;

			// Validate stream
			if (binaryStream.Empty())
			{
				LogError("Fail to load PNG. Invalid path to PNG, or corrupt PNG");
				return result;
			}
			if (binaryStream.ReadBytes(8) != "\211PNG\r\n\032\n")
			{
				LogError("Fail to load PNG. First 8 signature bytes has invalid values");
				return result;
			}

			// Parse stream into Chunks
			std::vector<_Chunk> chunks;
			std::string chunkContent;
			_Chunk chunk;
			unsigned int dataLength;
			do
			{
				// As long as they are unprocessed chunks - read them
				dataLength = binaryStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
				chunkContent = binaryStream.PeekBytes(dataLength + 4);
				chunk.type = binaryStream.ReadBytes(4);
				chunk.data = binaryStream.ReadBytes(dataLength);
				chunk.crc = binaryStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
				// Check CRC
				_crc(reinterpret_cast<void*>(chunkContent.data()), chunkContent.size()) == chunk.crc
					? LogInfo(chunk.type + " chunk CRC correct")
					: LogError(chunk.type + " chunk CRC incorrect");

				chunks.push_back(chunk);

			} while (!binaryStream.Empty());

			// Parse Chunks
			NSL::BinaryStream dataStream;
			std::vector<unsigned char> pixels;
			_IHDR ihdr;
			_PLTE plte;
			_gAMA gama;
			_cHRM chrm;
			_sRGB srgb;
			do
			{
				if (chunks.back().type == "IHDR")
				{
					dataStream = chunks.back().data;
					ihdr.width = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
					ihdr.height = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
					ihdr.bitDepth = dataStream.ReadUint8();
					ihdr.colorType = (_IHDR::ColorType)(dataStream.ReadUint8());
					ihdr.compression = dataStream.ReadUint8();
					ihdr.filter = dataStream.ReadUint8();
					ihdr.interlace = dataStream.ReadUint8();
				}
				else if (chunks.back().type == "PLTE")
				{
					dataStream = chunks.back().data;
					for (int i = 0; i < chunks.back().data.size() / 3; ++i)
					{
						NSL::Color3 color;
						color.r = dataStream.ReadUint8();
						color.g = dataStream.ReadUint8();
						color.b = dataStream.ReadUint8();
						plte.colors.push_back(color);
					}
				}
				else if (chunks.back().type == "IDAT")
				{
					pixels.reserve(chunks.back().data.size());
					for (auto& i : chunks.back().data)
					{
						pixels.push_back(i);
					}
				}
				else if (chunks.back().type == "cHRM")
				{
					dataStream = chunks.back().data;
					chrm.whitePointX = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
					chrm.whitePointY = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
					chrm.RedX = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
					chrm.RedY = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
					chrm.GreenX = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
					chrm.GreenY = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
					chrm.BlueX = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
					chrm.BlueY = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
				}
				else if (chunks.back().type == "gAMA")
				{
					dataStream = chunks.back().data;
					gama.gamma = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
				}
				else if (chunks.back().type == "sRGB")
				{
					dataStream = chunks.back().data;
					srgb.mode = (_sRGB::Mode)(dataStream.ReadUint8());
				}
				chunks.pop_back();

			} while (!chunks.empty());

			// PNG size and channels
			result.size = NSL::Vector2i(ihdr.width, ihdr.height);
			result.gamma = gama.gamma / 100000.0f;
			switch (ihdr.colorType)
			{
			case _IHDR::ColorType::Grayscale:		result.channels = PNG::Red;		break;
			case _IHDR::ColorType::RGB:				result.channels = PNG::RGB;		break;
			case _IHDR::ColorType::PaletteIndex:	result.channels = PNG::RGB;		break;
			case _IHDR::ColorType::GrayscaleAlpha:	result.channels = PNG::RG;		break;
			case _IHDR::ColorType::RGBA:			result.channels = PNG::RGBA;	break;
			default: LogError("Unknown PNG color type"); break;
			}

			// Decompress
			pixels = NSL::DecompressData(pixels);

			// If palette - pixels indexing from palette table
			if (ihdr.colorType == _IHDR::ColorType::PaletteIndex)
			{
				std::vector<unsigned char> indexedPixels;
				for (unsigned int y = 0; y < result.size.y; ++y)
				{
					for (unsigned int x = 1; x < result.size.x + 1; ++x)
					{
						Color3 color = plte.colors[x + y * result.size.x];
						indexedPixels.push_back(color.r);
						indexedPixels.push_back(color.g);
						indexedPixels.push_back(color.b);
					}
				}
				pixels = indexedPixels;
			}

			// Parse pixels into PNG scanlines
			Scanline scanline;
			std::vector<unsigned char> rawScanline;
			for (unsigned int i = 0; i < result.size.y; ++i)
			{
				rawScanline = std::vector<unsigned char>(pixels.begin(), (pixels.begin() + result.size.x * result.channels) + 1);
				scanline.filtering = (NSL::PNG::Scanline::Filtering)rawScanline[0];
				scanline.scanline = std::vector<unsigned char>(rawScanline.begin() + 1, rawScanline.end());
				result.scanlines.push_back(scanline);
				pixels.erase(pixels.begin(), (pixels.begin() + result.size.x * result.channels) + 1);
			}

			result._Unfilter();

			// Flip image
			const unsigned int halfHeight = result.size.y / 2;
			unsigned int endScanlineIndex;
			for (unsigned int i = 0; i < halfHeight; ++i)
			{
				endScanlineIndex = result.scanlines.size() - 1 - i;
				scanline = result.scanlines[i];

				result.scanlines[i] = result.scanlines[endScanlineIndex];
				result.scanlines[endScanlineIndex] = scanline;
			}

			

			return result;
		}

		int _Paeth(int left, int above, int upperLeft) NSL_NOEXCEPT
		{
			int estimated = left + above - upperLeft;

			int pa = std::abs(estimated - left);
			int pb = std::abs(estimated - above);
			int pc = std::abs(estimated - upperLeft);

			if ((pa <= pb) && (pa <= pc)) return left;
			else if (pb <= pc) return above;
			else return upperLeft;
		}
		int _Average(int left, int up) NSL_NOEXCEPT
		{
			return (int)std::floor((left + up) / 2.0);
		}
		void _Unfilter() NSL_NOEXCEPT
		{
			//PNG::Scanline& firstScanline = scanlines[0];

			//switch (firstScanline.filtering)
			//{
			//case PNG::Scanline::Filtering::None:
			//	break;

			//case PNG::Scanline::Filtering::Sub:
			//	for (size_t j = 0; j < firstScanline.scanline.size(); ++j)
			//	{
			//		const PixelRGB& leftPixel = j == 0 ? PixelRGB{ 0, 0, 0 } : firstScanline.scanline[j - 1];
			//		PixelRGB& currentPixel = firstScanline.scanline[j];

			//		currentPixel = currentPixel + leftPixel;
			//	}
			//	break;

			//default:
			//	break;
			//}
			//firstScanline.filtering = PNG::Scanline::Filtering::None;


			for (size_t i = 0; i < scanlines.size(); ++i)
			{
				const PNG::Scanline& previousScanline = i == 0 ? PNG::Scanline(size.x) : scanlines[i - 1];
				PNG::Scanline& currentScanline = scanlines[i];

				switch (currentScanline.filtering)
				{
				case PNG::Scanline::Filtering::None:
					break;

				case PNG::Scanline::Filtering::Sub:
					for (size_t j = 0; j < currentScanline.scanline.size(); j += channels)
					{
						for (size_t k = 0; k < channels; ++k)
						{
							const auto& leftPixel = j == 0 ? 0 : currentScanline.scanline[j - channels + k];
							auto& currentPixel = currentScanline.scanline[j + k];

							currentPixel += leftPixel;
						}
					}
					break;

				case PNG::Scanline::Filtering::Up:
					for (size_t j = 0; j < currentScanline.scanline.size(); j += channels)
					{
						for (size_t k = 0; k < channels; ++k)
						{
							const auto& upPixel = previousScanline.scanline[j + k];
							auto& currentPixel = currentScanline.scanline[j + k];

							currentPixel += upPixel;
						}
					}
					break;

				case PNG::Scanline::Filtering::Average:
					for (size_t j = 0; j < currentScanline.scanline.size(); j += channels)
					{
						for (size_t k = 0; k < channels; ++k)
						{
							const auto& leftPixel = j == 0 ? 0 : currentScanline.scanline[j - channels + k];
							const auto& upPixel = previousScanline.scanline[j + k];
							auto& currentPixel = currentScanline.scanline[j + k];

							currentPixel += _Average(leftPixel, upPixel);
						}
					}
					break;

				case PNG::Scanline::Filtering::Paeth:
					for (size_t j = 0; j < currentScanline.scanline.size(); j += channels)
					{
						for (size_t k = 0; k < channels; ++k)
						{
							const auto& leftPixel = j == 0 ? 0 : currentScanline.scanline[j - channels + k];
							const auto& upPixel = previousScanline.scanline[j + k];
							const auto& upLeftPixel = j == 0 ? 0 : previousScanline.scanline[j - channels + k];
							auto& currentPixel = currentScanline.scanline[j + k];

							currentPixel += _Paeth(leftPixel, upPixel, upLeftPixel);
						}
					}
					break;

				default:
					break;
				}

				currentScanline.filtering = PNG::Scanline::Filtering::None;
			}
		}
	

	private:
		static CRC _crc;
	};

	CRC PNG::_crc;
}