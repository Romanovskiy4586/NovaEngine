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
		enum Filtering : unsigned char
		{
			None = 0, Sub = 1, Up = 2, Average = 3, Paeth = 4
		};

		enum Channels : unsigned char
		{
			Red = 1, RG = 2, RGB = 3, RGBA = 4
		};
		
		struct Scanline
		{
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
		
		std::vector<Scanline> scanlines;
		NSL::Vector2i size;
		float gamma;
		Channels channels;
		bool srgb;

	public:
		struct IHDR
		{
			enum ColorType : unsigned char
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
			ColorType colorType = Grayscale;
			unsigned char compression = 0;
			unsigned char filter = 0;
			unsigned char interlace = 0;
		};
		struct PLTE
		{
			std::vector<NSL::Color3> colors;
		};
		struct gAMA
		{
			unsigned int gamma = 0;
		};
		struct cHRM
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
		struct sRGB
		{
			enum Mode : unsigned char
			{
				Perceptual = 0,
				RelativeColorimetric = 1,
				Saturation = 2, 
				AbsoluteColorimetric = 3
			};
			Mode mode = Perceptual;
		};
		struct Chunk
		{
			std::string type;
			std::string data;
			unsigned int crc = 0;
		};

	private:
		//static std::vector<unsigned char> ExtractPNGData(NSL::BinaryStream& binaryStream) NSL_NOEXCEPT
		//{
		//	Chunk chunk;
		//	std::vector<unsigned char> result;

		//	do
		//	{
		//		binaryStream.MovePointerTo(binaryStream.SearchFromCurrentPosition("IDAT") - 4);

		//		chunk.dataLength = binaryStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
		//		chunk.type = binaryStream.ReadBytes(4);
		//		chunk.data = binaryStream.ReadBytes(chunk.dataLength);
		//		chunk.crc = binaryStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);

		//		result.resize(result.size() + chunk.data.size());
		//		for (size_t i = 0; i < chunk.data.size(); i++)
		//		{
		//			result[result.size() - chunk.data.size() + i] = chunk.data[i];
		//		}
		//	} while (binaryStream.SearchFromCurrentPosition("IDAT") != binaryStream.NullPointerPosition);

		//	return NSL::DecompressData(result);
		//}
		static void FlipImage(std::vector<unsigned char>& image, NSL::Vector2i size, int channelsNum) NSL_NOEXCEPT
		{
			//const int pixelSize = 3;
			size_t halfHeight = size.y / 2;
			size_t buffersSize = size.x * channelsNum;
			std::vector<char> beginBuffer(buffersSize);
			std::vector<char> endBuffer(buffersSize);

			for (size_t i = 0; i < halfHeight; ++i)
			{
				// Filling buffers
				for (size_t j = 0; j < buffersSize; ++j)
				{
					beginBuffer[j] = image[j + (buffersSize * i)];
					endBuffer[j] = image[image.size() - buffersSize + j - (buffersSize * i)];
				}

				// Filling pngData from buffers
				for (size_t j = 0; j < buffersSize; ++j)
				{
					image[j + (buffersSize * i)] = endBuffer[j];
					image[image.size() - buffersSize + j - (buffersSize * i)] = beginBuffer[j];
				}
			}
		}
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
			std::vector<Chunk> chunks;
			std::string chunkContent;
			Chunk chunk;
			do
			{
				// As long as they are unprocessed chunks - read them
				auto dataLength = binaryStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
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
			IHDR ihdr;
			PLTE plte;
			gAMA gama;
			cHRM chrm;
			sRGB srgb;
			do
			{
				if (chunks.back().type == "IHDR")
				{
					dataStream = chunks.back().data;
					ihdr.width = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
					ihdr.height = dataStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
					ihdr.bitDepth = dataStream.ReadUint8();
					ihdr.colorType = (IHDR::ColorType)(dataStream.ReadUint8());
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
					srgb.mode = (sRGB::Mode)(dataStream.ReadUint8());
				}
				chunks.pop_back();

			} while (!chunks.empty());

			// PNG size and channels
			result.size = NSL::Vector2i(ihdr.width, ihdr.height);
			switch (ihdr.colorType)
			{
				case IHDR::Grayscale:		result.channels = PNG::Red;		break;
				case IHDR::RGB:				result.channels = PNG::RGB;		break;
				case IHDR::PaletteIndex:	result.channels = PNG::RGB;		break;
				case IHDR::GrayscaleAlpha:	result.channels = PNG::RG;		break;
				case IHDR::RGBA:			result.channels = PNG::RGBA;	break;
				default: LogError("Unknown PNG color type"); break;
			}

			// Decompress
			pixels = NSL::DecompressData(pixels);

			// If palette - pixels indexing from palette table
			if (ihdr.colorType == IHDR::PaletteIndex)
			{
				std::vector<unsigned char> indexedPixels;
				for (size_t y = 0; y < result.size.y; ++y)
				{
					for (size_t x = 1; x < result.size.x; ++x)
					{
						Color3 color = plte.colors[x + y * result.size.x];
						indexedPixels.push_back(color.r);
						indexedPixels.push_back(color.g);
						indexedPixels.push_back(color.b);
					}
				}
				pixels = indexedPixels;
			}

			

			//if (headerChunk.colorType == 0x02)
			//{
			//	result.channels = /*isSRGB ? PNG::Channels::SRGB :*/ PNG::Channels::RGB;
			//	RawPNGRGB rawPng(pngData, headerChunk.Width, headerChunk.Height);
			//	pngData = rawPng.GetData();
			//	//for (auto& i : pngData)
			//	//{
			//	//	i = static_cast<unsigned char>(ConvertSRGBToLinear((float)i / 256.0f) * 256.0f);
			//	//}
			//	FlipImage(pngData, headerChunk, 3);
			//}
			//else if (headerChunk.ColorType == 0x06)
			//{
			//	result.channels = /*isSRGB ? PNG::Channels::SRGBA :*/ PNG::Channels::RGBA;
			//	RawPNGRGBA rawPngRgba(pngData, headerChunk.width, headerChunk.height);
			//	pngData = rawPngRgba.GetData();
			//	//for (size_t i = 0; i < pngData.size() / 4; ++i)
			//	//{
			//	//	pngData[0 + 4 * i] = static_cast<unsigned char>(ConvertSRGBToLinear((float)pngData[0 + 4 * i] / 256.0f) * 256.0f);
			//	//	pngData[1 + 4 * i] = static_cast<unsigned char>(ConvertSRGBToLinear((float)pngData[1 + 4 * i] / 256.0f) * 256.0f);
			//	//	pngData[2 + 4 * i] = static_cast<unsigned char>(ConvertSRGBToLinear((float)pngData[2 + 4 * i] / 256.0f) * 256.0f);
			//	//}
			//	FlipImage(pngData, headerChunk, 4);
			//}
			//else
			//{
			//	LogError("Current PNG Color type are not supported");
			//	return result;
			//}

			//result.width = headerChunk.width;
			//result.height = headerChunk.height;
			//result.data = pngData;

			return result;
		}

	private:
		static CRC _crc;
	};

	CRC PNG::_crc;
}