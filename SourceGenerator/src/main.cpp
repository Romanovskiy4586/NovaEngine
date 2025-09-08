#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

static std::string _ToHex(unsigned char c)
{
	static const char hex[] = "0123456789ABCDEF"; // Hex symbols table
	std::string out = "0x00";		// Init out with '0'
	out[2] = hex[(c >> 4) & 0x0F];	// Look from table and fill with symbol
	out[3] = hex[c & 0x0F];			// Look from table and fill with symbol
	return out;
}
static std::string _ConvertFileContentIntoBinaryChars(const std::string& fileContent, const std::string& tabs)
{
	std::stringstream binaryString;
	int counter = 8;

	for (const auto& i : fileContent)
	{
		if (counter == 8)
		{
			counter = 0;
			binaryString << "\n" << tabs;
		}

		binaryString << _ToHex(i) << ", ";
		++counter;
	}

	std::string result = binaryString.str();
	result.erase(result.size() - 2, result.size());
	binaryString << '\n';

	return result;
}

int main(int argc, char* argv[])
{
	// Arguments into vector
	std::vector<std::string> arguments(argc);
	for (int i = 0; i < arguments.size(); ++i)
	{
		arguments[i] = argv[i];
	}

	// Read arguments
	std::string resourceFolderPath = arguments[1];
	std::string outputFolderPath = arguments[2];
	std::string namespaceName = arguments[3];
	bool includePCH = std::find(arguments.begin(), arguments.end(), "pch") != arguments.end();

	std::string tabs;
	std::ifstream inStream;
	std::ofstream outStream;
	std::stringstream sstream;
	std::string fileContent;
	std::string structName;
	std::stringstream moduleAgregatorContent;
	std::string emptyString;
	std::string message;

	moduleAgregatorContent << "export module ResourcesDB;\n\n";
	for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(resourceFolderPath))
	{
		if (dirEntry.is_regular_file())
		{
			// Read file
			inStream.open(dirEntry.path().string(), std::ios_base::binary);
			sstream << inStream.rdbuf();
			inStream.close();
			fileContent = sstream.str();
			sstream.str(emptyString);

			// Create struct name
			structName = dirEntry.path().string();
			structName = structName.erase(0, resourceFolderPath.size());
			for (auto& i : structName)
			{
				if (i == '\\' || i == '/' || i == '.' || i == ':' || i == '-' || i == ' ')
				{
					i = '_';
				}
			}

			// Start .cppm file
			outStream.open(outputFolderPath + structName + ".cppm");
			if (includePCH)
			{
				outStream << "#include \"pch.h\"\n";
			}
			outStream << "export module " << structName << ";\n\n";
			outStream << "export namespace " << namespaceName << '\n';
			outStream << "{\n";
			tabs += '\t';

			// Constructor
			outStream << tabs << "struct " << structName << '\n';
			outStream << tabs << "{\n";
			tabs += '\t';
			outStream << tabs << structName << "()\n";
			outStream << tabs << "{\n";
			tabs += '\t';
			outStream << tabs << "size = " << fileContent.size() << ";\n";
			outStream << tabs << "data = new unsigned char[]\n";
			outStream << tabs << "{";
			tabs += '\t';
			outStream << _ConvertFileContentIntoBinaryChars(fileContent, tabs);
			tabs.pop_back();
			outStream << '\n' << tabs << "};\n";
			tabs.pop_back();
			outStream << tabs << "};\n\n";

			// Destructor
			outStream << tabs << '~' << structName << "()\n";
			outStream << tabs << "{\n";
			tabs += '\t';
			outStream << tabs << "delete[] data;\n";
			tabs.pop_back();
			outStream << tabs << "};\n\n";

			// Members
			outStream << tabs << "unsigned long long size;\n";
			outStream << tabs << "const unsigned char* data;\n";
			tabs.pop_back();
			outStream << tabs << "};\n";

			// End .cppm file
			tabs.pop_back();
			outStream << tabs << "}";
			outStream.close();

			// Print message
			message = dirEntry.path().string() + " processed";
			std::cout << message << std::endl;

			moduleAgregatorContent << "export import " << structName << ";\n";
		}
	}

	// Output module-agregator
	outStream.open(outputFolderPath + "ResourcesDB.cppm");
	if (includePCH)
	{
		outStream << "#include \"pch.h\"\n";
	}
	outStream << moduleAgregatorContent.str();
	outStream.close();

	return 0;
}