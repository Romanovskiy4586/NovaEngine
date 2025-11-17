export module JSON;
#include "Core.h"
import std;
import NSL.Functions;
import NSL.BinaryStream;
import Logger;

namespace NSL
{
	class JSONArray;
	class JSONObject;
}

enum class JSONPropertyType { Property, Array, Object };

export namespace NSL
{
	class NSL_API JSONArray
	{
	public:
		const JSONObject& Child(size_t index) const NSL_NOEXCEPT
		{
			return _children[index];
		}
		const JSONArray& Array(size_t index) const NSL_NOEXCEPT
		{
			return _arrays[index];
		}
		std::string String(size_t index) const NSL_NOEXCEPT
		{
			const std::string& value = _properties[index];
			return std::string(value.begin() + 1, value.end() - 1);
		}
		long long Int(size_t index) const NSL_NOEXCEPT
		{
			return std::atoll(_properties[index].c_str());
		}
		double Real(size_t index) const NSL_NOEXCEPT
		{
			return std::atof(_properties[index].c_str());
		}
		bool Bool(size_t index) const NSL_NOEXCEPT
		{
			return _properties[index] == "true";
		}

		void AddProperty(const std::string& value) NSL_NOEXCEPT
		{
			_properties.push_back(value);
			_order.push_back(JSONPropertyType::Property);
		}
		void AddArray(JSONArray&& value) NSL_NOEXCEPT
		{
			_arrays.emplace_back(std::move(value));
			_order.push_back(JSONPropertyType::Array);
		}
		void AddChild(JSONObject&& value) NSL_NOEXCEPT
		{
			_children.emplace_back(std::move(value));
			_order.push_back(JSONPropertyType::Object);
		}

		const std::vector<std::string>& Properties() const NSL_NOEXCEPT
		{
			return _properties;
		}
		const std::vector<JSONArray>& Arrays() const NSL_NOEXCEPT
		{
			return _arrays;
		}
		const std::vector<JSONObject>& Children() const NSL_NOEXCEPT
		{
			return _children;
		}
		const std::vector<JSONPropertyType>& Order() const NSL_NOEXCEPT
		{
			return _order;
		}

	private:
		std::vector<std::string> _properties;
		std::vector<JSONArray> _arrays;
		std::vector<JSONObject> _children;
		std::vector<JSONPropertyType> _order;
	};

	class NSL_API JSONObject
	{
	public:
		const JSONObject& Child(const std::string& key) const NSL_NOEXCEPT
		{
			NSL_ASSERT(_children.contains(key), "JSON object is not contains child with name: " + key)
			return _children.at(key);
		}
		const JSONArray& Array(const std::string& key) const NSL_NOEXCEPT
		{
			NSL_ASSERT(_arrays.contains(key), "JSON object is not contains array with name: " + key)
			return _arrays.at(key);
		}
		std::string String(const std::string& key) const NSL_NOEXCEPT
		{
			NSL_ASSERT(_properties.contains(key), "JSON object is not contains property with name: " + key)
			const std::string& value = _properties.at(key);
			return std::string(value.begin() + 1, value.end() - 1);
		}
		long long Int(const std::string& key) const NSL_NOEXCEPT
		{
			NSL_ASSERT(_properties.contains(key), "JSON object is not contains property with name: " + key)
			return atoll(_properties.at(key).c_str());
		}
		double Real(const std::string& key) const NSL_NOEXCEPT
		{
			NSL_ASSERT(_properties.contains(key), "JSON object is not contains property with name: " + key)
			return atof(_properties.at(key).c_str());
		}
		bool Bool(const std::string& key) const NSL_NOEXCEPT
		{
			NSL_ASSERT(_properties.contains(key), "JSON object is not contains property with name: " + key)
			return _properties.at(key) == "true";
		}

		void AddStringAsProperty(const std::string& key, const std::string& value) NSL_NOEXCEPT
		{
			_properties[key] = value;
			_order.push_back(std::pair(JSONPropertyType::Property, key));
		}
		void AddProperty(const std::string& key, const std::string& value) NSL_NOEXCEPT
		{
			_properties[key] = '\"' + value + '\"';
			_order.push_back(std::pair(JSONPropertyType::Property, key));
		}
		void AddProperty(const std::string& key, const char* value) NSL_NOEXCEPT
		{
			AddProperty(key, std::string(value));
		}
		void AddProperty(const std::string& key, long long value) NSL_NOEXCEPT
		{
			_properties[key] = std::to_string(value);
			_order.push_back(std::pair(JSONPropertyType::Property, key));
		}
		void AddProperty(const std::string& key, int value) NSL_NOEXCEPT
		{
			AddProperty(key, static_cast<long long>(value));
		}
		void AddProperty(const std::string& key, double value) NSL_NOEXCEPT
		{
			_properties[key] = std::to_string(value);
			_order.push_back(std::pair(JSONPropertyType::Property, key));
		}
		void AddProperty(const std::string& key, float value) NSL_NOEXCEPT
		{
			AddProperty(key, static_cast<double>(value));
		}
		void AddProperty(const std::string& key, bool value) NSL_NOEXCEPT
		{
			_properties[key] = value ? "true" : "false";
			_order.push_back(std::pair(JSONPropertyType::Property, key));
		}
		void AddArray(const std::string& key, JSONArray&& value) NSL_NOEXCEPT
		{
			_arrays.try_emplace(key, std::move(value));
			_order.push_back(std::pair(JSONPropertyType::Array, key));
		}
		void AddChild(const std::string& key, JSONObject&& value) NSL_NOEXCEPT
		{
			_children.try_emplace(key, std::move(value));
			_order.push_back(std::pair(JSONPropertyType::Object, key));
		}

		const std::map<std::string, std::string>& Properties() const NSL_NOEXCEPT
		{
			return _properties;
		}
		const std::map<std::string, JSONArray>& Arrays() const NSL_NOEXCEPT
		{
			return _arrays;
		}
		const std::map<std::string, JSONObject>& Children() const NSL_NOEXCEPT
		{
			return _children;
		}
		const std::vector<std::pair<JSONPropertyType, std::string>>& Order() const NSL_NOEXCEPT
		{
			return _order;
		}

	private:
		std::map<std::string, std::string> _properties;
		std::map<std::string, JSONArray> _arrays;
		std::map<std::string, JSONObject> _children;
		std::vector<std::pair<JSONPropertyType, std::string>> _order;
	};

	class NSL_API JSON
	{
	public:
		static JSON Load(const std::string& jsonPath) NSL_NOEXCEPT
		{
			JSON json;
			NSL::BinaryStream stream = NSL::ReadFile(jsonPath);

			// Read root
			json.root = ParseObject(stream);

			return json;
		}
		static std::string ParseObjectIntoString(const JSONObject& object, std::string& tabulations) NSL_NOEXCEPT
		{
			std::stringstream str;
			tabulations += '\t';
			str << "{\n";

			const auto& order = object.Order();
			const auto& properties = object.Properties();
			const auto& arrays = object.Arrays();
			const auto& children = object.Children();

			bool isNotFirstProperty = false;
			for (const auto& current : order)
			{
				if (isNotFirstProperty)
				{
					str << ",\n";
				}
				else
				{
					isNotFirstProperty = true;
				}

				if (current.first == JSONPropertyType::Property)
				{
					str << tabulations << '\"' << current.second << "\": " << properties.at(current.second);
				}
				if (current.first == JSONPropertyType::Array)
				{
					str << tabulations << '\"' << current.second << "\": " << ParseArrayIntoString(arrays.at(current.second), tabulations);
				}
				if (current.first == JSONPropertyType::Object)
				{
					str << tabulations << '\"' << current.second << "\": " << ParseObjectIntoString(children.at(current.second), tabulations);
				}
			}

			str << "\n";
			tabulations.pop_back();
			str << tabulations << "}";

			return str.str();
		}
		void Save(const std::string& jsonPath) NSL_NOEXCEPT
		{
			std::string tabulations;
			NSL::WriteFile(jsonPath, ParseObjectIntoString(root, tabulations));
		}

	private:
		static void SkipSpacings(NSL::BinaryStream& stream) NSL_NOEXCEPT
		{
			while (stream.PeekInt8() == ' ' || stream.PeekInt8() == '\t' || stream.PeekInt8() == '\n')
			{
				stream.SkipBytes(1);
			}
		}
		static std::string ParseKey(NSL::BinaryStream& stream) NSL_NOEXCEPT
		{
			std::stringstream string;

			stream.SkipBytes(1);
			while (/*stream.PeekInt8() != '\'' && */stream.PeekInt8() != '\"') string << stream.ReadInt8();
			stream.SkipBytes(1);

			return string.str();
		}
		static std::string ParseProperty(NSL::BinaryStream& stream) NSL_NOEXCEPT
		{
			std::stringstream value;

			if (/*stream.PeekInt8() == '\'' || */stream.PeekInt8() == '\"')
			{
				value << stream.ReadInt8();
				while (/*stream.PeekInt8() != '\'' && */stream.PeekInt8() != '\"')
				{
					value << stream.ReadInt8();
				}
				value << stream.ReadInt8();
			}
			else
			{
				while (stream.PeekInt8() != ' ' && stream.PeekInt8() != '\t' && stream.PeekInt8() != '\n' 
					&& stream.PeekInt8() != ',' && stream.PeekInt8() != ']')
				{
					value << stream.ReadInt8();
				}
			}

			return value.str();
		}
		static JSONArray ParseArray(NSL::BinaryStream& stream) NSL_NOEXCEPT
		{
			JSONArray resultArray;
			std::string value;

			while (stream.PeekInt8() != ']')
			{
				SkipSpacings(stream);
				stream.SkipBytes(1); // Skip '[' or ','
				SkipSpacings(stream);

				if (stream.PeekInt8() == '{')
				{
					resultArray.AddChild(ParseObject(stream));
				}
				else if (stream.PeekInt8() == '[')
				{
					resultArray.AddArray(ParseArray(stream));
				}
				else
				{
					resultArray.AddProperty(ParseProperty(stream));
				}

				SkipSpacings(stream);
			}
			stream.SkipBytes(1); // Skip ']'
			return resultArray;
		}
		static JSONObject ParseObject(NSL::BinaryStream& stream) NSL_NOEXCEPT
		{
			JSONObject resultObject;
			std::string key;
			std::string value;

			while (stream.PeekInt8() != '}')
			{
				SkipSpacings(stream);
				stream.SkipBytes(1); // Skip '{' or ','
				SkipSpacings(stream);

				// If object is empty - break
				if (stream.PeekInt8() == '}') break;

				key = ParseKey(stream);
				SkipSpacings(stream);
				stream.SkipBytes(1); // Skip ':'
				SkipSpacings(stream);

				if (stream.PeekInt8() == '{')
				{
					resultObject.AddChild(key, ParseObject(stream));
				}
				else if (stream.PeekInt8() == '[')
				{
					resultObject.AddArray(key, ParseArray(stream));
				}
				else
				{
					resultObject.AddStringAsProperty(key, ParseProperty(stream));
				}

				SkipSpacings(stream);
			}
			stream.SkipBytes(1); // Skip '}'
			return resultObject;
		}
		static std::string ParseArrayIntoString(const JSONArray& array, std::string& tabulations) NSL_NOEXCEPT
		{
			std::stringstream str;
			tabulations += '\t';
			str << "[\n";

			const auto& order = array.Order();
			auto properties = array.Properties().begin();
			auto arrays = array.Arrays().begin();
			auto children = array.Children().begin();

			bool isNotFirstProperty = false;
			for (const auto& current : order)
			{
				if (isNotFirstProperty)
				{
					str << ",\n";
				}
				else
				{
					isNotFirstProperty = true;
				}

				if (current == JSONPropertyType::Property)
				{
					str << tabulations << *properties++;
				}
				if (current == JSONPropertyType::Array)
				{
					str << tabulations << ParseArrayIntoString(*arrays++, tabulations);
				}
				if (current == JSONPropertyType::Object)
				{
					str << tabulations << ParseObjectIntoString(*children++, tabulations);
				}
			}

			str << "\n";
			tabulations.pop_back();
			str << tabulations << "]";

			return str.str();
		}

	public:
		JSONObject root;
	};
}