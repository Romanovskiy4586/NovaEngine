export module XML;

export import XMLNode;
import NSL;
#include "Core.h"

export namespace NSL
{
	struct NSL_API XML
	{
	public:
        static XML Load(const char* xmlPath) NSL_NOEXCEPT
        {
            NSL::BinaryStream xmlStream = NSL::ReadFile(xmlPath);
            XML xml;
            std::stringstream sstr;

            if ("<?xml version=\"1.0\" encoding=\"utf-8\"?>" != xmlStream.PeekBytes(38)) return xml;
            xmlStream.SkipBytes(38);
            while (xmlStream.PeekInt8() != '<') xmlStream.SkipBytes(1);

            xml.rootNode = ReadNode(xmlStream, sstr);

            return xml;
        }

    private:
        static XMLNode ReadNode(NSL::BinaryStream& xmlStream, std::stringstream& sstr) NSL_NOEXCEPT
        {
            XMLNode node;

            node.Name() = ReadNodeName(xmlStream, sstr);
            for (auto& i : ReadAttributes(xmlStream, sstr)) node.AddAttribute(i.first, i.second);

            bool shortNodeClose = xmlStream.PeekInt8() == '/';
            while (xmlStream.PeekInt8() == '/' || xmlStream.PeekInt8() == '>') xmlStream.SkipBytes(1); // Skip / and >

            node.Element() = ReadElement(xmlStream, sstr);

            // If short node close - node cannot have children, so node can be returned
            if (shortNodeClose) return node;
            // Else - check if node has children


            // Read text in current angle brackets
            size_t bitesWasReadedBeforeAngleBracketsTextCheck = xmlStream.BytesWasReaded(); // Save pointer position
            std::string textInCurrentAndleBrackets = ReadNodeName(xmlStream, sstr);


            // While text in angle brackets are not equal to an end of a current node - it is a new node, so we read it, and repeat until all children will be readed
            std::string textReresentsNodeClosing = '/' + node.Name();
            while (textInCurrentAndleBrackets != textReresentsNodeClosing)
            {
                xmlStream.MovePointerTo(bitesWasReadedBeforeAngleBracketsTextCheck); // Return binarystream pointer to begin of a new child node
                node.AddChild(ReadNode(xmlStream, sstr));

                while (xmlStream.PeekInt8() != '<') xmlStream.SkipBytes(1);
                bitesWasReadedBeforeAngleBracketsTextCheck = xmlStream.BytesWasReaded();
                textInCurrentAndleBrackets = ReadNodeName(xmlStream, sstr);
            }

            return node;
        }

        static std::string ReadNodeName(NSL::BinaryStream& xmlStream, std::stringstream& sstr) NSL_NOEXCEPT
        {
            sstr.str(std::string());
            xmlStream.SkipBytes(1); // Skip <
            while (xmlStream.PeekInt8() != '>' && xmlStream.PeekInt8() != ' ' && xmlStream.PeekBytes(2) != "/>") sstr << xmlStream.ReadInt8();
            while (xmlStream.PeekInt8() == ' ') xmlStream.SkipBytes(1); // Skip spaces
            return sstr.str();
        }
        static std::map<std::string, std::string> ReadAttributes(NSL::BinaryStream& xmlStream, std::stringstream& sstr) NSL_NOEXCEPT
        {
            sstr.str(std::string());
            std::map<std::string, std::string> attributes;

            while (xmlStream.PeekBytes(2) != "/>" && xmlStream.PeekInt8() != '>')
            {
                std::string attributeName;
                std::string attributeValue;

                // Read attribute name
                while (xmlStream.PeekInt8() != '=' && xmlStream.PeekInt8() != ' ') sstr << xmlStream.ReadInt8();
                attributeName = sstr.str();
                sstr.str(std::string());

                // Skip all junk between attribute name and value
                while (xmlStream.PeekInt8() != '\"' && xmlStream.PeekInt8() != '\'') xmlStream.SkipBytes(1); // Skip all before first " or '
                xmlStream.SkipBytes(1); // Skip first " or '

                // Read attribute value
                while (xmlStream.PeekInt8() != '\"' && xmlStream.PeekInt8() != '\'') sstr << xmlStream.ReadInt8();
                xmlStream.SkipBytes(1); // Skip last " or ' symbol
                attributeValue = sstr.str();
                sstr.str(std::string());

                while (xmlStream.PeekInt8() == ' ' || xmlStream.PeekInt8() == '\t' || xmlStream.PeekInt8() == '\n') xmlStream.SkipBytes(1);

                attributes[attributeName] = attributeValue;
            }

            return attributes;
        }
        static std::string ReadElement(NSL::BinaryStream& xmlStream, std::stringstream& sstr) NSL_NOEXCEPT
        {
            sstr.str(std::string());
            while (xmlStream.PeekInt8() != '<') if (xmlStream.PeekInt8() != '\t' && xmlStream.PeekInt8() != '\n') sstr << xmlStream.ReadInt8(); else xmlStream.SkipBytes(1);
            return sstr.str();
        }

	public:
		XMLNode rootNode;
	};
}