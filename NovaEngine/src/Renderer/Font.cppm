export module Font;
#include "Core.h"
import std;
import NSL;

#define MACRO(param, index) const NSL::JSONArray& array##index = parameters.root.Array({param}); \
_charToTileIndex[param] = NSL::Vector2(static_cast<float>(array##index.Real(0)), static_cast<float>(array##index.Real(1)))


export namespace Renderer
{
	class NSL_API Font
	{
	public:
		Font(const NSL::JSON& parameters) NSL_NOEXCEPT
		{
			_Init(parameters);
		}
		Font(const std::string& jsonParameters) NSL_NOEXCEPT
		{
			NSL::JSON parameters = NSL::JSON::Parse(jsonParameters);
			_Init(parameters);
		}

		const NSL::Vector2& GetAtlasSize() const NSL_NOEXCEPT
		{
			return _atlasSize;
		}
		const NSL::Vector2& GetGlyphSize() const NSL_NOEXCEPT
		{
			return _glyphSize;
		}
		const NSL::Vector2& CharToTileIndex(char c) const NSL_NOEXCEPT
		{
			NSL_ASSERT(_charToTileIndex.contains(c), "Font do not contains char \"" + std::string{c} + "\"")
			return _charToTileIndex.at(c);
		}

	private:
		void _Init(const NSL::JSON& parameters) NSL_NOEXCEPT
		{
			_atlasSize.x = static_cast<float>(parameters.root.Array("AtlasSize").Real(0));
			_atlasSize.y = static_cast<float>(parameters.root.Array("AtlasSize").Real(1));
			_glyphSize.x = static_cast<float>(parameters.root.Array("GlyphSize").Real(0));
			_glyphSize.y = static_cast<float>(parameters.root.Array("GlyphSize").Real(1));

			MACRO('A', 0);
			MACRO('B', 1);
			MACRO('C', 2);
			MACRO('D', 3);
			MACRO('E', 4);
			MACRO('F', 5);
			MACRO('G', 6);
			MACRO('H', 7);
			MACRO('I', 8);
			MACRO('J', 9);
			MACRO('K', 10);
			MACRO('L', 11);
			MACRO('M', 12);
			MACRO('N', 13);
			MACRO('O', 14);
			MACRO('P', 15);
			MACRO('Q', 16);
			MACRO('R', 17);
			MACRO('S', 18);
			MACRO('T', 19);
			MACRO('U', 20);
			MACRO('V', 21);
			MACRO('W', 22);
			MACRO('X', 23);
			MACRO('Y', 24);
			MACRO('Z', 25);

			MACRO('a', 26);
			MACRO('b', 27);
			MACRO('c', 28);
			MACRO('d', 29);
			MACRO('e', 30);
			MACRO('f', 31);
			MACRO('g', 32);
			MACRO('h', 33);
			MACRO('i', 34);
			MACRO('j', 35);
			MACRO('k', 36);
			MACRO('l', 37);
			MACRO('m', 38);
			MACRO('n', 39);
			MACRO('o', 40);
			MACRO('p', 41);
			MACRO('q', 42);
			MACRO('r', 43);
			MACRO('s', 44);
			MACRO('t', 45);
			MACRO('u', 46);
			MACRO('v', 47);
			MACRO('w', 48);
			MACRO('x', 49);
			MACRO('y', 50);
			MACRO('z', 51);

			MACRO('0', 52);
			MACRO('1', 53);
			MACRO('2', 54);
			MACRO('3', 55);
			MACRO('4', 56);
			MACRO('5', 57);
			MACRO('6', 58);
			MACRO('7', 59);
			MACRO('8', 60);
			MACRO('9', 61);
			MACRO(' ', 62);

			MACRO('@', 63);
			MACRO('.', 64);
			MACRO(',', 65);
			MACRO(':', 66);
			MACRO(';', 67);
			MACRO('~', 68);
			MACRO('!', 69);
			MACRO('?', 70);

			MACRO('\'', 71);
			MACRO('(', 72);
			MACRO(')', 73);
			MACRO('{', 74);
			MACRO('}', 75);
		}

	private:
		NSL::Vector2 _atlasSize;
		NSL::Vector2 _glyphSize;
		std::unordered_map<char, NSL::Vector2> _charToTileIndex;
	};
}