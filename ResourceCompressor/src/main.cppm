export module main;
import std;

export int main(int argc, char* argv[])
{
	// Arguments into vector
	std::vector<std::string> arguments(argc);
	for (int i = 0; i < arguments.size(); ++i)
	{
		arguments[i] = argv[i];
	}

	// Read arguments
	std::string resourceFolderPath = arguments[1];
	std::string outputFilePath = arguments[2];

	std::stringstream sstream;
	std::string fileContent;
	std::string fileName;
	std::string emptyString;
	std::ifstream inStream;
	std::ofstream outStream(outputFilePath + ".ncr");

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

			// Get file name
			fileName = dirEntry.path().string();
			fileName = fileName.erase(0, resourceFolderPath.size());
			for (auto& i : fileName)
			{
				if (i == '\\' || i == '/' || i == '.' || i == ':' || i == '-' || i == ' ')
				{
					i = '_';
				}
			}

			outStream << fileName << fileContent.size() << fileContent;
		}
	}

	outStream.close();

	return 0;
}