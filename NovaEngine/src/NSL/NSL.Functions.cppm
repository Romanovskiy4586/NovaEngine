module;
#include "zlib.h"
export module NSL.Functions;
#include "Core.h"
#define BUFFER_SIZE 1024
import std;
import Logger;

std::string _ReadFile(const std::string& filePath, std::ios_base::openmode mode) NSL_NOEXCEPT
{
    std::ifstream inFileStream;
    std::stringstream sstream;
    std::string result;

    inFileStream.open(filePath, mode);
    if (inFileStream.good())
    {
        sstream << inFileStream.rdbuf();
        inFileStream.close();
        result = sstream.str();
    }

    return result;
}
void _WriteFile(const std::string& filePath, const std::string& textToWrite, std::ios_base::openmode mode) NSL_NOEXCEPT
{
    std::ofstream outFileStream(filePath, mode);
    outFileStream << textToWrite;
    outFileStream.close();
}

export namespace NSL
{
    void WriteFile(const std::string& filePath, const std::string& textToWrite) NSL_NOEXCEPT
    {
        _WriteFile(filePath, textToWrite, std::ios::out);
    }
    void WriteBinaryFile(const std::string& filePath, const std::string& textToWrite) NSL_NOEXCEPT
    {
        _WriteFile(filePath, textToWrite, std::ios::binary);
    }
    bool IsValidFilePath(const std::string& filePath) NSL_NOEXCEPT
    {
        return std::filesystem::is_regular_file(filePath);
    }
    bool IsValidFolderPath(const std::string& folderPath) NSL_NOEXCEPT
    {
        return std::filesystem::is_directory(folderPath);
    }
    std::string ReadFile(const std::string& filePath) NSL_NOEXCEPT
    {
        return _ReadFile(filePath, std::ios::in);
    }
    std::string ReadBinaryFile(const std::string& filePath) NSL_NOEXCEPT
    {
        return _ReadFile(filePath, std::ios::binary);
    }
    std::string Replace(const std::string& text, const std::string& replacable, const std::string& replaceWith) NSL_NOEXCEPT
    {
        if (replacable.empty())
        {
            LogWarning("Attempt to make a replace in string, but replace string is empty");
            return text;
        }

        std::stringstream result;
        std::stringstream buffer;
        auto it = text.begin();

        while (it != text.end())
        {
            // If character is not match first character of replacable - continue
            if (*it != replacable[0])
            {
                result << *it++;
                continue;
            }

            // If it does - read num of symbols size of replacable sequence
            // If readed string matches replacable string - write in result replaceWith sequence
            for (size_t j = 0; j < replacable.size(); ++j)
            {
                buffer << *(it + j);
            }
            if (buffer.str() == replacable)
            {
                result << replaceWith;
                it += replacable.size();
                buffer.str(std::string());
                continue;
            }

            // If sequence do not match replacable sequence - just write that symbol in result
            buffer.str(std::string());
            result << *it++;
        }

        return result.str();
    }
    std::string GetNameOfLastFolderInFolderPath(const std::string& folderPath, char foldersDelimiter) NSL_NOEXCEPT
    {
        size_t indexOfLastSlash = folderPath.find_last_of(foldersDelimiter, folderPath.size());
        size_t indexOfFirstCharacterOfFolderName = indexOfLastSlash + 1;
        size_t lengthOfFolderName = folderPath.size() - indexOfLastSlash - 1;
        return folderPath.substr(indexOfFirstCharacterOfFolderName, lengthOfFolderName);
    }
    std::string GetFileNameFromFilePath(const std::string& filePath) NSL_NOEXCEPT
    {
        std::string result;
        std::string delimiter = "/";

        std::string pathCopy = Replace(filePath, "\\", delimiter);

        size_t indexOfLastSlash = filePath.find_last_of(delimiter, filePath.size());
        size_t indexOfFirstCharacterOfFolderName = indexOfLastSlash + 1;
        size_t lengthOfFolderName = filePath.size() - indexOfLastSlash - 1;
        result = filePath.substr(indexOfFirstCharacterOfFolderName, lengthOfFolderName);

        size_t indexOfDot = result.find('.');
        result = result.substr(0, indexOfDot);

        return result;
    }
    std::string GetFileExtention(const std::string& filePath) NSL_NOEXCEPT
    {
        std::string result;

        size_t indexOfDot = filePath.rfind('.');
        size_t extentionSize = filePath.size() - indexOfDot;
        result = filePath.substr(indexOfDot + 1, extentionSize);

        return result;
    }
    std::vector<std::string> DivideString(const std::string& text, char delimiter) NSL_NOEXCEPT
    {
        // Init needed variables
        std::string textCopy = text;
        std::vector<std::string> raws;
        size_t delimiterIndex = 0;

        // First delimiter check. If there is no delimiter = returns -1
        delimiterIndex = textCopy.find(delimiter);

        // If no delimiter - skips while loop. Else - enter/continue
        while (delimiterIndex != -1)
        {
            // Push into vector substring from text from zero to index of a delimiter
            raws.push_back(textCopy.substr(0, delimiterIndex));
            // Erase that raw
            textCopy.erase(0, delimiterIndex + 1);
            // Find next delimiter
            delimiterIndex = textCopy.find(delimiter);
        }
        raws.push_back(textCopy.substr(0, delimiterIndex));

        return raws;
    }
    std::vector<std::string> DivideStringIntoRows(const std::string& text) NSL_NOEXCEPT
    {
        return DivideString(text, '\n');
    }
    std::string DiscardCharacter(const std::string& text, char character) NSL_NOEXCEPT
    {
        std::stringstream result;
        for (const char& i : text)
        {
            if (i != character)
            {
                result << i;
            }
        }
        return result.str();
    }
    std::string DiscardAllSpaces(const std::string& text) NSL_NOEXCEPT
    {
        return DiscardCharacter(text, ' ');
    }
    std::string DiscardAllTabulations(const std::string& text) NSL_NOEXCEPT
    {
        return DiscardCharacter(text, '\t');
    }
    void CutFront(std::string& text, size_t cuttingIndex) NSL_NOEXCEPT
    {
        text.erase(0, cuttingIndex);
    }
    void CutBack(std::string& text, size_t cuttingIndex) NSL_NOEXCEPT
    {
        text.erase(cuttingIndex, text.size());
    }
    bool IsASCIINumber(char character) NSL_NOEXCEPT
    {
        return 0x30 <= character && character <= 0x39;
    }
    std::vector<unsigned char> CompressData(const std::vector<unsigned char>& uncompressedData, int compressionLevel = 9) NSL_NOEXCEPT
    {
        z_stream zstream;
        std::memset(&zstream, 0, sizeof(zstream));

        if (deflateInit(&zstream, compressionLevel) != Z_OK)
        {
            LogError("deflateInit failed while compressing");
            return std::vector<unsigned char>();
        }

        zstream.next_in = (Bytef*)(uncompressedData.data());
        zstream.avail_in = static_cast<uInt>(uncompressedData.size());

        int returnMacroIndex;
        char outbuffer[BUFFER_SIZE];
        std::vector<unsigned char> outData;

        do
        {
            zstream.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zstream.avail_out = sizeof(outbuffer);

            returnMacroIndex = deflate(&zstream, Z_FINISH);

            if (outData.size() < zstream.total_out)
            {
                size_t oldSize = outData.size();
                outData.resize(zstream.total_out);
                for (size_t i = 0; i < outData.size() - oldSize; ++i)
                {
                    outData[oldSize + i] = outbuffer[i];
                }
            }
        } while (returnMacroIndex == Z_OK);

        if (deflateEnd(&zstream) != Z_OK)
        {
            LogError("Fail when called deflateEnd");
        }

        if (returnMacroIndex != Z_STREAM_END)
        {
            std::ostringstream oss;
            oss << "Error during zlib compression: (" << returnMacroIndex << ") " << zstream.msg;
            LogError(oss.str());
        }

        return outData;
    }
    std::vector<unsigned char> DecompressData(const std::vector<unsigned char>& compressedData) NSL_NOEXCEPT
    {
        z_stream zstream;
        std::memset(&zstream, 0, sizeof(zstream));

        if (inflateInit(&zstream) != Z_OK)
        {
            LogError("inflateInit failed while decompressing");
            return std::vector<unsigned char>();
        }

        zstream.next_in = (Bytef*)compressedData.data();
        zstream.avail_in = static_cast<uInt>(compressedData.size());

        int returnMacroIndex;
        char outbuffer[BUFFER_SIZE];
        std::vector<unsigned char> outData;

        do
        {
            zstream.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zstream.avail_out = sizeof(outbuffer);

            returnMacroIndex = inflate(&zstream, 0);

            if (outData.size() < zstream.total_out)
            {
                size_t oldSize = outData.size();
                outData.resize(zstream.total_out);
                for (size_t i = 0; i < outData.size() - oldSize; ++i)
                {
                    outData[oldSize + i] = outbuffer[i];
                }
            }
        } while (returnMacroIndex == Z_OK);

        if (inflateEnd(&zstream) != Z_OK)
        {
            LogError("Fail when called inflateEnd");
        }

        if (returnMacroIndex != Z_STREAM_END)
        {
            std::ostringstream oss;
            oss << "Error during zlib decompression: (" << returnMacroIndex << ") " << zstream.msg;
            LogError(oss.str());
        }

        return outData;
    }
    std::string CompressData(const std::string& uncompressedData, int compressionLevel) NSL_NOEXCEPT
    {
        z_stream zstream;
        std::memset(&zstream, 0, sizeof(zstream));

        if (deflateInit(&zstream, compressionLevel) != Z_OK)
        {
            LogError("deflateInit failed while compressing");
            return std::string();
        }

        zstream.next_in = (Bytef*)uncompressedData.data();
        zstream.avail_in = static_cast<uInt>(uncompressedData.size());

        int returnMacroIndex;
        char outbuffer[BUFFER_SIZE];
        std::string outstring;

        do
        {
            zstream.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zstream.avail_out = sizeof(outbuffer);

            returnMacroIndex = deflate(&zstream, Z_FINISH);

            if (outstring.size() < zstream.total_out)
            {
                outstring.append(outbuffer, zstream.total_out - outstring.size());
            }
        } while (returnMacroIndex == Z_OK);

        if (deflateEnd(&zstream) != Z_OK)
        {
            LogError("Fail when called deflateEnd");
        }

        if (returnMacroIndex != Z_STREAM_END)
        {
            std::ostringstream oss;
            oss << "Error during zlib compression: (" << returnMacroIndex << ") " << zstream.msg;
            LogError(oss.str());
        }

        return outstring;
    }
    std::string DecompressData(const std::string& compressedData) NSL_NOEXCEPT
    {
        z_stream zstream;
        std::memset(&zstream, 0, sizeof(zstream));

        if (inflateInit(&zstream) != Z_OK)
        {
            LogError("inflateInit failed while decompressing");
            return std::string();
        }

        zstream.next_in = (Bytef*)compressedData.data();
        zstream.avail_in = static_cast<uInt>(compressedData.size());

        int returnMacroIndex;
        char outbuffer[BUFFER_SIZE];
        std::string outstring;

        do
        {
            zstream.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zstream.avail_out = sizeof(outbuffer);

            returnMacroIndex = inflate(&zstream, 0);

            if (outstring.size() < zstream.total_out)
            {
                outstring.append(outbuffer, zstream.total_out - outstring.size());
            }
        } while (returnMacroIndex == Z_OK);

        if (inflateEnd(&zstream) != Z_OK)
        {
            LogError("Fail when called inflateEnd");
        }

        if (returnMacroIndex != Z_STREAM_END)
        {
            std::ostringstream oss;
            oss << "Error during zlib decompression: (" << returnMacroIndex << ") " << zstream.msg;
            LogError(oss.str());
        }

        return outstring;
    }
    void CompressFile(const std::string& uncompressedInputFilePath, const std::string& compressedOutputFilePath, int compressionLevel = 9) NSL_NOEXCEPT
    {
        std::string inputFileString = ReadBinaryFile(uncompressedInputFilePath);
        WriteBinaryFile(compressedOutputFilePath, CompressData(inputFileString, compressionLevel));
    }
    void DecompressFile(const std::string& compressedInputFilePath, const std::string& uncompressedOutputFilePath) NSL_NOEXCEPT
    {
        std::string inputFileString = ReadBinaryFile(compressedInputFilePath);
        WriteBinaryFile(uncompressedOutputFilePath, DecompressData(inputFileString));
    }
}