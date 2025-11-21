export module FBXLoader;
#include "Core.h"
import std;
import FBX;
import RawFBX;
import NSL.BinaryStream;
import NSL.Functions;
import Logger;

export namespace NSL
{
	class FBXLoader
	{
	public:
		FBX Load(const std::string& filePath) const NSL_NOEXCEPT
		{
			RawFBX rawFBX = LoadRawFBX(filePath);
			FBX fbx = MakeFBXFromRawFBX(rawFBX);
			return fbx;
		}

	private:
		enum class ArrayType { Integers = 105, Doubles = 100 };
		enum class LayerElement { Normal, UV };

		RawFBX LoadRawFBX(const std::string& filePath) const NSL_NOEXCEPT
		{
			NSL::BinaryStream fbxStream = NSL::ReadBinaryFile(filePath);
			NSL::BinaryStream nodeStream;

			RawFBX rawFBX;

			// Vertices
			nodeStream = GetUncompressedDataFromFBXNode(fbxStream, "Vertices", ArrayType::Doubles);
			size_t verticesCount = nodeStream.Length() / 8;
			rawFBX.Vertices.resize(verticesCount);
			for (size_t i = 0; i < verticesCount; ++i)
			{
				rawFBX.Vertices[i] = nodeStream.ReadDouble();
			}

			// Indices
			nodeStream = GetUncompressedDataFromFBXNode(fbxStream, "PolygonVertexIndex", ArrayType::Integers);
			size_t indicesCount = nodeStream.Length() / 4;
			rawFBX.Indices.resize(indicesCount);
			for (size_t i = 0; i < indicesCount; ++i)
			{
				rawFBX.Indices[i] = nodeStream.ReadUint32();
			}

			// UVs
			nodeStream = GetUncompressedDataFromFBXNode(fbxStream, "UV", ArrayType::Doubles);
			size_t uvsCount = nodeStream.Length() / 8;
			rawFBX.UVs.resize(uvsCount);
			for (size_t i = 0; i < uvsCount; ++i)
			{
				rawFBX.UVs[i] = nodeStream.ReadDouble();
			}

			// UV Indices
			nodeStream = GetUncompressedDataFromFBXNode(fbxStream, "UVIndex", ArrayType::Integers);
			size_t uvIndicesCount = nodeStream.Length() / 4;
			rawFBX.UVsIndices.resize(uvIndicesCount);
			for (size_t i = 0; i < uvIndicesCount; ++i)
			{
				rawFBX.UVsIndices[i] = nodeStream.ReadUint32();
			}

			// Normals
			nodeStream = GetUncompressedDataFromFBXNode(fbxStream, "Normals", ArrayType::Doubles);
			size_t normalsCount = nodeStream.Length() / 8;
			rawFBX.Normals.resize(normalsCount);
			for (size_t i = 0; i < normalsCount; ++i)
			{
				rawFBX.Normals[i] = nodeStream.ReadDouble();
			}

			// Normals Indices
			nodeStream = GetUncompressedDataFromFBXNode(fbxStream, "NormalsIndex", ArrayType::Integers);
			size_t normalsIndicesCount = nodeStream.Length() / 4;
			rawFBX.NormalsIndices.resize(normalsIndicesCount);
			for (size_t i = 0; i < normalsIndicesCount; ++i)
			{
				rawFBX.NormalsIndices[i] = nodeStream.ReadUint32();
			}

			FillMappingAndReferenceType(fbxStream, rawFBX, LayerElement::Normal);
			FillMappingAndReferenceType(fbxStream, rawFBX, LayerElement::UV);

			return rawFBX;
		}
		FBX MakeFBXFromRawFBX(const RawFBX& rawFBX) const NSL_NOEXCEPT
		{
			FBX fbx;

			// Indices
			//NSL_ASSERT(static_cast<int>(rawFBX.Indices[3]) >= 0, "");
			NSL_ASSERT(static_cast<int>(rawFBX.Indices[2]) < 0, "Third index of a FBX less than zero. It means that FBX mesh consists not from triandles");

			fbx.indices.resize(rawFBX.Indices.size());
			for (size_t i = 0; i < fbx.indices.size() / 3; ++i)
			{
				fbx.indices[0 + i * 3] = rawFBX.Indices[0 + i * 3];
				fbx.indices[1 + i * 3] = rawFBX.Indices[1 + i * 3];
				fbx.indices[2 + i * 3] = 0xffffffff - rawFBX.Indices[2 + i * 3];
			}

			// Vertices
			fbx.vertices.resize(fbx.indices.size() * 3);
			for (size_t i = 0; i < fbx.indices.size(); ++i)
			{
				double& currentVertexX = fbx.vertices[0 + i * 3];
				double& currentVertexY = fbx.vertices[1 + i * 3];
				double& currentVertexZ = fbx.vertices[2 + i * 3];

				const double& fromTableVertexX = rawFBX.Vertices[0 + fbx.indices[i] * 3];
				const double& fromTableVertexY = rawFBX.Vertices[1 + fbx.indices[i] * 3];
				const double& fromTableVertexZ = rawFBX.Vertices[2 + fbx.indices[i] * 3];

				currentVertexX = fromTableVertexX;
				currentVertexY = fromTableVertexY;
				currentVertexZ = fromTableVertexZ;

				fbx.indices[i] = static_cast<unsigned int>(i);
			}

			// Normals
			fbx.normals.resize(fbx.vertices.size());
			switch (rawFBX.NormalsReference)
			{
			case RawFBX::ReferenceType::Direct:
				for (size_t i = 0; i < fbx.normals.size(); ++i)
				{
					fbx.normals[i] = rawFBX.Normals[i];
				}
				break;

			case RawFBX::ReferenceType::IndexToDirect:
				for (size_t i = 0; i < fbx.normals.size() / 3; ++i)
				{
					double& currentNormalX = fbx.normals[0 + i * 3];
					double& currentNormalY = fbx.normals[1 + i * 3];
					double& currentNormalZ = fbx.normals[2 + i * 3];

					const double& fromTableNormalX = rawFBX.Normals[0 + rawFBX.NormalsIndices[i] * 3];
					const double& fromTableNormalY = rawFBX.Normals[1 + rawFBX.NormalsIndices[i] * 3];
					const double& fromTableNormalZ = rawFBX.Normals[2 + rawFBX.NormalsIndices[i] * 3];

					currentNormalX = fromTableNormalX;
					currentNormalY = fromTableNormalY;
					currentNormalZ = fromTableNormalZ;
				}
				break;

			default:
				LogError("Fail to load FBX. Unknown normals reference type");
				break;
			}

			// UVs
			fbx.uvs.resize(fbx.vertices.size() / 3 * 2);
			switch (rawFBX.UVsReference)
			{
			case RawFBX::ReferenceType::Direct:
				for (size_t i = 0; i < fbx.uvs.size(); ++i)
				{
					fbx.uvs[i] = rawFBX.UVs[i];
				}
				break;

			case RawFBX::ReferenceType::IndexToDirect:
				for (size_t i = 0; i < fbx.uvs.size() / 2; ++i)
				{
					double& currentUvX = fbx.uvs[0 + i * 2];
					double& currentUvY = fbx.uvs[1 + i * 2];

					const double& fromTableUvX = rawFBX.UVs[0 + rawFBX.UVsIndices[i] * 2];
					const double& fromTableUvY = rawFBX.UVs[1 + rawFBX.UVsIndices[i] * 2];

					currentUvX = fromTableUvX;
					currentUvY = fromTableUvY;
				}
				break;

			default:
				LogError("Fail to load FBX. Unknown UVs reference type");
				break;
			}

			return fbx;
		}
		void FillMappingAndReferenceType(NSL::BinaryStream& fbxStream, RawFBX& rawFBX, LayerElement layerElement) const NSL_NOEXCEPT
		{
			size_t layerIndex;
			RawFBX::MappingType* mapping;
			RawFBX::ReferenceType* reference;
			switch (layerElement)
			{
			case FBXLoader::LayerElement::Normal:
				layerIndex = fbxStream.Search("LayerElementNormal");
				mapping = &rawFBX.NormalsMapping;
				reference = &rawFBX.NormalsReference;
				break;

			case FBXLoader::LayerElement::UV:
				layerIndex = fbxStream.Search("LayerElementUV");
				mapping = &rawFBX.UVsMapping;
				reference = &rawFBX.UVsReference;
				break;

			default:
				LogError("Fail to load FBX. LayerElement chunk can not be found");
				return;
			}

			// Mapping Information Type
			fbxStream.MovePointerTo(layerIndex);
			fbxStream.MovePointerTo(fbxStream.SearchFromCurrentPosition("MappingInformationTypeS"));
			fbxStream.SkipBytes(23);
			unsigned int mappingSize = fbxStream.ReadUint32();
			std::string mappingType = fbxStream.ReadBytes(mappingSize);

			if (mappingType == "ByPolygon") *mapping = RawFBX::MappingType::ByPolygon;
			else if (mappingType == "ByPolygonVertex") *mapping = RawFBX::MappingType::ByPolygonVertex;
			else if (mappingType == "ByVertex" || mappingType == "ByVertice") *mapping = RawFBX::MappingType::ByVertex;
			else if (mappingType == "ByEdge") *mapping = RawFBX::MappingType::ByEdge;
			else if (mappingType == "AllSame") *mapping = RawFBX::MappingType::AllSame;
			else LogError("Fail to load FBX. Unknown FBX Mapping Type");

			// Reference Information Type
			fbxStream.MovePointerTo(fbxStream.SearchFromCurrentPosition("ReferenceInformationTypeS"));
			fbxStream.SkipBytes(25);
			unsigned int referenceSize = fbxStream.ReadUint32();
			std::string referenceType = fbxStream.ReadBytes(referenceSize);

			if (referenceType == "Direct") *reference = RawFBX::ReferenceType::Direct;
			else if (referenceType == "IndexToDirect" || referenceType == "Index") *reference = RawFBX::ReferenceType::IndexToDirect;
			else LogError("Fail to load FBX. Unknown FBX Reference Type");
		}
		std::string GetUncompressedDataFromFBXNode(NSL::BinaryStream& fbxStream, const std::string& nodeName, ArrayType arrayType) const NSL_NOEXCEPT
		{
			fbxStream.MovePointerTo(fbxStream.Search(nodeName + static_cast<char>(arrayType)));
			fbxStream.SkipBytes(nodeName.length() + 5);
			unsigned int isCompressed = fbxStream.ReadUint32();
			unsigned int dataLength = fbxStream.ReadUint32();

			return isCompressed ? NSL::DecompressData(fbxStream.ReadBytes(dataLength)) : fbxStream.ReadBytes(dataLength);
		}
		void ParseBinaryFBXToHumanReadableTxtFile(const std::string& filePath) const NSL_NOEXCEPT
		{
			NSL::BinaryStream bstream = NSL::ReadFile(filePath);
			std::ofstream outStream(NSL::GetFileNameFromFilePath(filePath) + ".txt");

			bstream.SkipBytes(23); // Пропускаем заголовок
			auto version = bstream.ReadUint32();
			if (version > 7400)
			{
				LogError("Engine do not support FBX files with versions more than 7400(7.4). Version of this FBX was " + std::to_string(version));
				return;
			}

			unsigned int endOffset;
			unsigned int propertiesCount;
			unsigned int propertyListLength;
			unsigned char nameLength;
			std::string name;

			char propertyType;
			unsigned int arrayLength;
			unsigned int coding;
			unsigned int compressedLength;
			unsigned int length;
			std::string arrayData;
			std::string peekBuf;
			std::string endOfNodeTerminators(13, '\0');

			std::string spacing(4, ' ');
			size_t spacingsCount = 0;

			while (true)
			{
				endOffset = bstream.ReadUint32();
				propertiesCount = bstream.ReadUint32();
				propertyListLength = bstream.ReadUint32();
				nameLength = bstream.ReadUint8();
				name = bstream.ReadBytes(nameLength);

				for (size_t i = 0; i < spacingsCount; ++i)
				{
					outStream << spacing;
				}
				outStream << name << ": ";

				for (size_t i = 0; i < propertiesCount; ++i)
				{
					propertyType = bstream.ReadInt8();
					switch (propertyType)
					{
					case 'Y':
						outStream << std::to_string(bstream.ReadInt16()) << ' ';
						break;

					case 'C':
						outStream << std::to_string(bstream.ReadInt8()) << ' ';
						break;

					case 'I':
						outStream << std::to_string(bstream.ReadInt32()) << ' ';
						break;

					case 'F':
						outStream << std::to_string(bstream.ReadFloat()) << ' ';
						break;

					case 'D':
						outStream << std::to_string(bstream.ReadDouble()) << ' ';
						break;

					case 'L':
						outStream << std::to_string(bstream.ReadInt64()) << ' ';
						break;

					case 'f':
						arrayLength = bstream.ReadUint32();
						coding = bstream.ReadUint32();
						compressedLength = bstream.ReadUint32();
						if (coding)
						{
							bstream.SkipBytes(compressedLength);
							outStream << "COMPRESSED f[" << compressedLength << "] ";
						}
						else
						{
							bstream.SkipBytes(arrayLength);
							outStream << "f[" << arrayLength << "] ";
						}
						break;

					case 'd':
						arrayLength = bstream.ReadUint32();
						coding = bstream.ReadUint32();
						compressedLength = bstream.ReadUint32();
						if (coding)
						{
							bstream.SkipBytes(compressedLength);
							outStream << "COMPRESSED d[" << compressedLength << "] ";
						}
						else
						{
							bstream.SkipBytes(arrayLength);
							outStream << "d[" << arrayLength << "] ";
						}
						break;

					case 'l':
						arrayLength = bstream.ReadUint32();
						coding = bstream.ReadUint32();
						compressedLength = bstream.ReadUint32();
						if (coding)
						{
							bstream.SkipBytes(compressedLength);
							outStream << "COMPRESSED l[" << compressedLength << "] ";
						}
						else
						{
							bstream.SkipBytes(arrayLength);
							outStream << "l[" << arrayLength << "] ";
						}
						break;

					case 'i':
						arrayLength = bstream.ReadUint32();
						coding = bstream.ReadUint32();
						compressedLength = bstream.ReadUint32();
						if (coding)
						{
							bstream.SkipBytes(compressedLength);
							outStream << "COMPRESSED i[" << compressedLength << "] ";
						}
						else
						{
							bstream.SkipBytes(compressedLength);
							outStream << "i[" << arrayLength << "] ";
						}
						break;

					case 'b':
						arrayLength = bstream.ReadUint32();
						coding = bstream.ReadUint32();
						compressedLength = bstream.ReadUint32();
						if (coding)
						{
							bstream.SkipBytes(compressedLength);
							outStream << "COMPRESSED b[" << compressedLength << "] ";
						}
						else
						{
							bstream.SkipBytes(arrayLength);
							outStream << "b[" << arrayLength << "] ";
						}
						break;

					case 'S':
						length = bstream.ReadUint32();
						arrayData = bstream.ReadBytes(length);
						arrayData.insert(arrayData.size(), 1, '\0');
						outStream << arrayData;
						break;

					case 'R':
						length = bstream.ReadUint32();
						arrayData = bstream.ReadBytes(length);
						arrayData.insert(arrayData.size(), 1, '\0');
						outStream << "RAW[" << arrayData.size() << "] ";
						break;

					default:
						outStream << "<<<!!!ERROR!!!>>>";
						outStream.close();
						return;
					}
				}

				if (bstream.BytesWasReaded() != endOffset)
				{
					++spacingsCount;
				}

				peekBuf = bstream.PeekBytes(13);
				while (peekBuf == endOfNodeTerminators)
				{
					bstream.SkipBytes(13);
					peekBuf = bstream.PeekBytes(13);
					spacingsCount = 0;
				}

				outStream << '\n';
			}

			outStream.close();
		}
	};
}