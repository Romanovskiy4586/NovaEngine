export module Tile;
#include "Core.h"
import std;

export namespace Stalker
{
	struct Tile
	{
		enum class Type { Empty, Wall };

		Tile()
			: type(Type::Empty)
		{
		}

		Type type;
	};
}