module;
#include "Core.h"
#include <vector>

export module TileGrid;
//import std;
import NovaEngine;

export namespace Bugs
{
	class TileGrid
	{
	public:
		struct Tile
		{
			enum class Type
			{
				Empty, Wall
			};

			Tile()
				: type(Type::Empty)
			{
			}

			bool operator==(const Tile& other) const
			{
				return type == other.type;
			}

			Type type;
		};
		class Column
		{
		public:
			Column(int height = 1)
			{
				Rebuild(height);
			}

		public:
			Tile& operator[](int index)
			{
				return _tiles[index];
			}
			const Tile& operator[](int index) const
			{
				return _tiles[index];
			}
			void Rebuild(int height)
			{
				_tiles = std::vector<Tile>(NSL::Max(height, 1), Tile());
			}
			int GetSize() const
			{
				return static_cast<int>(_tiles.size());
			}


		private:
			std::vector<Tile> _tiles;
		};

	public:
		TileGrid(int width = 1, int height = 1)
		{
			Resize(width, height);
		}

	public:
		Column& operator[](int index)
		{
			return _columns[index];
		}
		const Column& operator[](int index) const
		{
			return _columns[index];
		}
		void Resize(int width, int height)
		{
			_width = width;
			_height = height;
			_columns = std::vector<Column>(NSL::Max(width, 1), Column(height));
		}
		int GetWidth() const
		{
			return _width;
		}
		int GetHeight() const
		{
			return _height;
		}

	private:
		int _width;
		int _height;
		std::vector<Column> _columns;
	};
}