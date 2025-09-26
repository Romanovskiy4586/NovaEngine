export module PNGLoader;
#include "Core.h"
import std;
import PNG;
import PixelRGB;
import PixelRGBA;
import NSL.Functions;
import NSL.BinaryStream;
import Mathematics.Functions;
import Logger;

export namespace NSL
{
	class NSL_API PNGLoader
	{
	public:
		PNG Load(const std::string& filePath) const NSL_NOEXCEPT
		{
			PNG result;
			result.width = 1;
			result.height = 1;
			result.channels = PNG::Channels::RED;
			result.colorSpace = PNG::ColorSpace::Linear;

			NSL::BinaryStream binaryStream = NSL::ReadBinaryFile(filePath);
			if (!IsPNGValid(binaryStream))
			{
				return result;
			}

			PNGHeaderChunk headerChunk = ReadIHDRChunk(binaryStream);

			if (headerChunk.Interlace != 0)
			{
				LogError("Fail to load PNG. Interlacing (type of data storage) of that PNG is not supported");
				return result;
			}

			if (binaryStream.Search("IDAT") == binaryStream.NullPointerPosition)
			{
				LogError("Fail to load PNG. IDAT chunk are not present");
				return result;
			}

			std::vector<unsigned char> pngData = ExtractPNGData(binaryStream);

			bool isSRGB = binaryStream.Search("sRGB") != binaryStream.NullPointerPosition;

			if (isSRGB)
			{
				binaryStream.MovePointerTo(binaryStream.Search("gAMA") + 4);
				unsigned int gamma = binaryStream.ReadUint32(BinaryStream::BytesReadOrder::RightToLeft);
				if (gamma != 45455U)
				{
					LogWarning("PNG gamma value is not the standart 1/2.2");
				}
				result.gamma = gamma / 100000.0f;
			}
			else
			{
				result.gamma = 0.0f;
				LogWarning("Loading PNG " + filePath + ". PNG is not in sRGB space");
			}

			if (headerChunk.ColorType == 0x02)
			{
				result.channels = /*isSRGB ? PNG::Channels::SRGB :*/ PNG::Channels::RGB;
				RawPNGRGB rawPng(pngData, headerChunk.Width, headerChunk.Height);
				pngData = rawPng.GetData();
				//for (auto& i : pngData)
				//{
				//	i = static_cast<unsigned char>(ConvertSRGBToLinear((float)i / 256.0f) * 256.0f);
				//}
				FlipImage(pngData, headerChunk, 3);
			}
			else if (headerChunk.ColorType == 0x06)
			{
				result.channels = /*isSRGB ? PNG::Channels::SRGBA :*/ PNG::Channels::RGBA;
				RawPNGRGBA rawPngRgba(pngData, headerChunk.Width, headerChunk.Height);
				pngData = rawPngRgba.GetData();
				//for (size_t i = 0; i < pngData.size() / 4; ++i)
				//{
				//	pngData[0 + 4 * i] = static_cast<unsigned char>(ConvertSRGBToLinear((float)pngData[0 + 4 * i] / 256.0f) * 256.0f);
				//	pngData[1 + 4 * i] = static_cast<unsigned char>(ConvertSRGBToLinear((float)pngData[1 + 4 * i] / 256.0f) * 256.0f);
				//	pngData[2 + 4 * i] = static_cast<unsigned char>(ConvertSRGBToLinear((float)pngData[2 + 4 * i] / 256.0f) * 256.0f);
				//}
				FlipImage(pngData, headerChunk, 4);
			}
			else
			{
				LogError("Current PNG Color type are not supported");
				return result;
			}

			result.width = headerChunk.Width;
			result.height = headerChunk.Height;
			result.data = pngData;

			return result;
		}

	private:
		struct PNGHeaderChunk
		{
			unsigned int DataLength;
			std::string Type;
			unsigned int Width;
			unsigned int Height;
			unsigned char BitDepth;
			unsigned char ColorType;
			unsigned char Compression;
			unsigned char Filter;
			unsigned char Interlace;
		};
		struct PNGDataChunk
		{
			unsigned int DataLength;
			std::string Type;
			std::string Data;
			unsigned int Crc;
		};
		struct PNGPaletteChunk
		{
			unsigned int DataLength;
			std::string Type;
			std::string Data;
			unsigned int Crc;
		};
		enum class ScanlineFiltering
		{
			None, Sub, Up, Average, Paeth
		};
		struct PNGScanLineRGB
		{
			ScanlineFiltering filtering;
			std::vector<PixelRGB> scanline;
		};
		struct PNGScanLineRGBA
		{
			ScanlineFiltering filtering;
			std::vector<PixelRGBA> scanline;
		};

		static int Paeth(int left, int above, int upperLeft) NSL_NOEXCEPT
		{
			int estimated = left + above - upperLeft;

			int pa = std::abs(estimated - left);
			int pb = std::abs(estimated - above);
			int pc = std::abs(estimated - upperLeft);

			if ((pa <= pb) && (pa <= pc)) return left;
			else if (pb <= pc) return above;
			else return upperLeft;
		}
		static int Average(int left, int up) NSL_NOEXCEPT
		{
			return (int)std::floor((left + up) / 2.0);
		}

		class RawPNGRGB
		{
		public:
			RawPNGRGB(const std::vector<unsigned char>& rawData, int width, int height)
				: _scanlines()
				, _width(width)
				, _height(height)
			{
				Init(rawData);
				Unfilter();
			}

		public:
			std::vector<unsigned char> GetData() NSL_NOEXCEPT
			{
				std::vector<unsigned char> result;

				for (auto& scanline : _scanlines)
				{
					for (auto& pixel : scanline.scanline)
					{
						result.push_back(pixel.R);
						result.push_back(pixel.G);
						result.push_back(pixel.B);
					}
				}

				return result;
			}

		private:
			void Init(const std::vector<unsigned char>& rawData)
			{
				_scanlines.clear();
				_scanlines.resize(_height);

				const int rawScanlineSize = _width * 3 + 1;
				std::vector<unsigned char> currentDataChunk(rawScanlineSize, 0x00);

				for (size_t i = 0; i < _height; ++i)
				{
					PNGScanLineRGB& currentScanline = _scanlines[i];

					for (size_t j = 0; j < rawScanlineSize; ++j)
					{
						currentDataChunk[j] = rawData[j + (rawScanlineSize * i)];
					}

					currentScanline.filtering = static_cast<ScanlineFiltering>(currentDataChunk[0]);

					for (size_t j = 0; j < _width; ++j)
					{
						currentScanline.scanline.push_back({});
						PixelRGB& currentPixel = currentScanline.scanline[j];
						currentPixel.R = currentDataChunk[1 + (j * 3)];
						currentPixel.G = currentDataChunk[2 + (j * 3)];
						currentPixel.B = currentDataChunk[3 + (j * 3)];
					}
				}
			}
			void Unfilter() NSL_NOEXCEPT
			{
				PNGScanLineRGB& firstScanline = _scanlines[0];

				switch (firstScanline.filtering)
				{
				case ScanlineFiltering::None:
					break;

				case ScanlineFiltering::Sub:
					for (size_t j = 0; j < firstScanline.scanline.size(); ++j)
					{
						const PixelRGB& leftPixel = j == 0 ? PixelRGB{0, 0, 0} : firstScanline.scanline[j - 1];
						PixelRGB& currentPixel = firstScanline.scanline[j];

						currentPixel = currentPixel + leftPixel;
					}
					break;

				default:
					break;
				}
				firstScanline.filtering = ScanlineFiltering::None;


				for (size_t i = 1; i < _scanlines.size(); ++i)
				{
					PNGScanLineRGB& previousScanline = _scanlines[i - 1];
					PNGScanLineRGB& currentScanline = _scanlines[i];

					switch (currentScanline.filtering)
					{
					case ScanlineFiltering::None:
						break;

					case ScanlineFiltering::Sub:
						for (size_t j = 0; j < currentScanline.scanline.size(); ++j)
						{
							const PixelRGB& leftPixel = j == 0 ? PixelRGB(0, 0, 0) : currentScanline.scanline[j - 1];
							PixelRGB& currentPixel = currentScanline.scanline[j];

							currentPixel = currentPixel + leftPixel;
						}
						break;

					case ScanlineFiltering::Up:
						for (size_t j = 0; j < currentScanline.scanline.size(); ++j)
						{
							const PixelRGB& upPixel = previousScanline.scanline[j];
							PixelRGB& currentPixel = currentScanline.scanline[j];

							currentPixel = currentPixel + upPixel;
						}
						break;

					case ScanlineFiltering::Average:
						for (size_t j = 0; j < currentScanline.scanline.size(); ++j)
						{
							const PixelRGB& leftPixel = j == 0 ? PixelRGB(0, 0, 0) : currentScanline.scanline[j - 1];
							const PixelRGB& abovePixel = previousScanline.scanline[j];

							PixelRGB resultPixel;
							resultPixel.R = Average(leftPixel.R, abovePixel.R);
							resultPixel.G = Average(leftPixel.G, abovePixel.G);
							resultPixel.B = Average(leftPixel.B, abovePixel.B);

							currentScanline.scanline[j] = currentScanline.scanline[j] + resultPixel;
						}
						break;

					case ScanlineFiltering::Paeth:
						for (size_t j = 0; j < currentScanline.scanline.size(); ++j)
						{
							const PixelRGB& leftPixel = j == 0 ? PixelRGB(0, 0, 0) : currentScanline.scanline[j - 1];
							const PixelRGB& abovePixel = previousScanline.scanline[j];
							const PixelRGB& upperLeftPixel = j == 0 ? PixelRGB(0, 0, 0) : previousScanline.scanline[j - 1];

							PixelRGB resultPixel;
							resultPixel.R = Paeth(leftPixel.R, abovePixel.R, upperLeftPixel.R);
							resultPixel.G = Paeth(leftPixel.G, abovePixel.G, upperLeftPixel.G);
							resultPixel.B = Paeth(leftPixel.B, abovePixel.B, upperLeftPixel.B);

							currentScanline.scanline[j] = currentScanline.scanline[j] + resultPixel;
						}
						break;

					default:
						break;
					}

					currentScanline.filtering = ScanlineFiltering::None;
				}
			}

		private:
			std::vector<PNGScanLineRGB> _scanlines;
			int _width;
			int _height;
		};
		class RawPNGRGBA
		{
		public:
			RawPNGRGBA(const std::vector<unsigned char>& rawData, int width, int height)
				: _scanlines()
				, _width(width)
				, _height(height)
			{
				Init(rawData);
				Unfilter();
			}

		public:
			std::vector<unsigned char> GetData() NSL_NOEXCEPT
			{
				std::vector<unsigned char> result;

				for (auto& scanline : _scanlines)
				{
					for (auto& pixel : scanline.scanline)
					{
						result.push_back(pixel.R);
						result.push_back(pixel.G);
						result.push_back(pixel.B);
						result.push_back(pixel.A);
					}
				}

				return result;
			}

		private:
			void Init(const std::vector<unsigned char>& rawData)
			{
				_scanlines.clear();
				_scanlines.resize(_height);

				const int rawScanlineSize = _width * 4 + 1;
				std::vector<unsigned char> currentDataChunk(rawScanlineSize, 0x00);

				for (size_t i = 0; i < _height; ++i)
				{
					PNGScanLineRGBA& currentScanline = _scanlines[i];

					for (size_t j = 0; j < rawScanlineSize; ++j)
					{
						currentDataChunk[j] = rawData[j + (rawScanlineSize * i)];
					}

					currentScanline.filtering = static_cast<ScanlineFiltering>(currentDataChunk[0]);

					for (size_t j = 0; j < _width; ++j)
					{
						currentScanline.scanline.push_back({});
						PixelRGBA& currentPixel = currentScanline.scanline[j];
						currentPixel.R = currentDataChunk[1 + (j * 4)];
						currentPixel.G = currentDataChunk[2 + (j * 4)];
						currentPixel.B = currentDataChunk[3 + (j * 4)];
						currentPixel.A = currentDataChunk[4 + (j * 4)];
					}
				}
			}
			void Unfilter() NSL_NOEXCEPT
			{
				PNGScanLineRGBA& firstScanline = _scanlines[0];

				switch (firstScanline.filtering)
				{
				case ScanlineFiltering::None:
					break;

				case ScanlineFiltering::Sub:
					for (size_t j = 0; j < firstScanline.scanline.size(); ++j)
					{
						const PixelRGBA& leftPixel = j == 0 ? PixelRGBA(0, 0, 0, 0) : firstScanline.scanline[j - 1];
						PixelRGBA& currentPixel = firstScanline.scanline[j];

						currentPixel = currentPixel + leftPixel;
					}
					break;

				default:
					break;
				}
				firstScanline.filtering = ScanlineFiltering::None;

				for (size_t i = 1; i < _scanlines.size(); ++i)
				{
					PNGScanLineRGBA& previousScanline = _scanlines[i - 1];
					PNGScanLineRGBA& currentScanline = _scanlines[i];

					switch (currentScanline.filtering)
					{
					case ScanlineFiltering::None:
						break;

					case ScanlineFiltering::Sub:
						for (size_t j = 0; j < currentScanline.scanline.size(); ++j)
						{
							const PixelRGBA& leftPixel = j == 0 ? PixelRGBA(0, 0, 0, 0) : currentScanline.scanline[j - 1];
							PixelRGBA& currentPixel = currentScanline.scanline[j];

							currentPixel += leftPixel;
						}
						break;

					case ScanlineFiltering::Up:
						for (size_t j = 0; j < currentScanline.scanline.size(); ++j)
						{
							PixelRGBA& abovePixel = previousScanline.scanline[j];
							PixelRGBA& currentPixel = currentScanline.scanline[j];

							currentPixel += abovePixel;
						}
						break;

					case ScanlineFiltering::Average:
						for (size_t j = 0; j < currentScanline.scanline.size(); ++j)
						{
							const PixelRGBA& leftPixel = j == 0 ? PixelRGBA(0, 0, 0, 0) : currentScanline.scanline[j - 1];
							PixelRGBA& abovePixel = previousScanline.scanline[j];

							PixelRGBA resultPixel;
							resultPixel.R = Average(leftPixel.R, abovePixel.R);
							resultPixel.G = Average(leftPixel.G, abovePixel.G);
							resultPixel.B = Average(leftPixel.B, abovePixel.B);
							resultPixel.A = Average(leftPixel.A, abovePixel.A);

							currentScanline.scanline[j] = currentScanline.scanline[j] + resultPixel;
						}
						break;

					case ScanlineFiltering::Paeth:
						for (size_t j = 0; j < currentScanline.scanline.size(); ++j)
						{
							const PixelRGBA& leftPixel = j == 0 ? PixelRGBA(0, 0, 0, 0) : currentScanline.scanline[j - 1];
							PixelRGBA& abovePixel = previousScanline.scanline[j];
							const PixelRGBA& upperLeftPixel = j == 0 ? PixelRGBA(0, 0, 0, 0) : previousScanline.scanline[j - 1];

							PixelRGBA resultPixel;
							resultPixel.R = Paeth(leftPixel.R, abovePixel.R, upperLeftPixel.R);
							resultPixel.G = Paeth(leftPixel.G, abovePixel.G, upperLeftPixel.G);
							resultPixel.B = Paeth(leftPixel.B, abovePixel.B, upperLeftPixel.B);
							resultPixel.A = Paeth(leftPixel.A, abovePixel.A, upperLeftPixel.A);

							currentScanline.scanline[j] = currentScanline.scanline[j] + resultPixel;
						}
						break;

					default:
						break;
					}

					currentScanline.filtering = ScanlineFiltering::None;
				}
			}

		private:
			std::vector<PNGScanLineRGBA> _scanlines;
			int _width;
			int _height;
		};

		bool IsPNGValid(NSL::BinaryStream& binaryStream) const NSL_NOEXCEPT
		{
			if (binaryStream.Empty())
			{
				LogWarning("Fail to load PNG. Invalid path to PNG, or corrupt PNG");
				return false;
			}

			if (binaryStream.ReadBytes(8) != "\211PNG\r\n\032\n")
			{
				LogError("Fail to load PNG. First 8 signature bytes has invalid values");
				return false;
			}

			return true;
		}
		PNGHeaderChunk ReadIHDRChunk(NSL::BinaryStream& binaryStream) const NSL_NOEXCEPT
		{
			return PNGHeaderChunk
			{
				binaryStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft),
				binaryStream.ReadBytes(4),
				binaryStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft),
				binaryStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft),
				binaryStream.ReadUint8(),
				binaryStream.ReadUint8(),
				binaryStream.ReadUint8(),
				binaryStream.ReadUint8(),
				binaryStream.ReadUint8()
			};
		}
		std::vector<unsigned char> ExtractPNGData(NSL::BinaryStream& binaryStream) const NSL_NOEXCEPT
		{
			PNGDataChunk chunk;
			std::vector<unsigned char> result;

			do
			{
				binaryStream.MovePointerTo(binaryStream.SearchFromCurrentPosition("IDAT") - 4);

				chunk.DataLength = binaryStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);
				chunk.Type = binaryStream.ReadBytes(4);
				chunk.Data = binaryStream.ReadBytes(chunk.DataLength);
				chunk.Crc = binaryStream.ReadUint32(NSL::BinaryStream::BytesReadOrder::RightToLeft);

				result.resize(result.size() + chunk.Data.size());
				for (size_t i = 0; i < chunk.Data.size(); i++)
				{
					result[result.size() - chunk.Data.size() + i] = chunk.Data[i];
				}
			} while (binaryStream.SearchFromCurrentPosition("IDAT") != binaryStream.NullPointerPosition);

			return NSL::DecompressData(result);
		}
		void FlipImage(std::vector<unsigned char>& image, PNGHeaderChunk headerChunk, int channelsNum) const NSL_NOEXCEPT
		{
			//const int pixelSize = 3;
			size_t halfHeight = headerChunk.Height / 2;
			size_t buffersSize = headerChunk.Width * channelsNum;
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
	};
}