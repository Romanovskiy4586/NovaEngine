export module WAVE;
#include "Core.h"
import std;
import NSL.Functions;
import Logger;
import NSL.BinaryStream;

export namespace NSL
{
	struct NSL_API WAVE final
	{
	public:
		struct NSL_API Header
		{
			std::string chunkID; // Should contain the word "RIFF"
			int chunkSize;
			std::string format; // Should contain 4 bytes
		};
		struct NSL_API Format
		{
			std::string subChunkID;
			int subChunkSize;
			short audioFormat;
			short numChannels;
			int sampleRate;
			int byteRate;
			short blockAlign;
			short bitsPerSample;
		};
		struct NSL_API Data
		{
			std::string subChunkID; // Should contain the word "data"
			int subChunkSize;
			std::string data;
		};

	public:
		static WAVE Load(const std::string& wavePath) NSL_NOEXCEPT
		{
			// Read WAVE into stream
			NSL::BinaryStream stream = NSL::ReadBinaryFile(wavePath);

			// Init data
			std::string fileName = NSL::GetFileNameFromFilePath(wavePath, '\\');
			Header header;
			Format format;
			Data data;

			// Fill Header
			header.chunkID = stream.ReadBytes(4); // RIFF
			header.chunkSize = stream.ReadInt32();
			header.format = stream.ReadBytes(4); // WAVE

			// Fill Format
			format.subChunkID = stream.ReadBytes(4); // fmt
			format.subChunkSize = stream.ReadInt32();
			format.audioFormat = stream.ReadInt16();
			format.numChannels = stream.ReadInt16();
			format.sampleRate = stream.ReadInt32();
			format.byteRate = stream.ReadInt32();
			format.blockAlign = stream.ReadInt16();
			format.bitsPerSample = stream.ReadInt16();

			// Fill Data
			data.subChunkID = stream.ReadBytes(4); // data
			std::string buffer("");
			while (data.subChunkID != "data")
			{
				buffer = stream.ReadBytes(2);
				data.subChunkID[0] = data.subChunkID[2];
				data.subChunkID[1] = data.subChunkID[3];
				data.subChunkID[2] = buffer[0];
				data.subChunkID[3] = buffer[1];
				if (stream.Length() < 2)
				{
					LogError("Failed to load WAVE file. \"data\" chunk wasn't there");
					return WAVE(header, format, data);
				}
			}
			data.subChunkSize = stream.ReadInt32();
			data.data = stream.ReadBytes(stream.Length());

			LogInfo("Loaded WAVE from: " + fileName);
			return WAVE(header, format, data);
		}

	public:
		Header header;
		Format format;
		Data data;
	};
}