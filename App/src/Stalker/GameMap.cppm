export module GameMap;
#include "Core.h"
import std;
import NovaEngine;
import Tile;

export namespace Stalker
{
	class GameMap final
	{
	public:
		GameMap(const NSL::Vector2ui size)
			: _size(size)
			, _grid(size.x * size.y)
			, _astar(size)
		{
			NSL::JSON json = NSL::JSON::Load("StalkerConfig.json");
			_tiletypeToCostMultiplierMap[Tile::Type::Empty] = json.root.Real("emptyTileCost");
			_tiletypeToCostMultiplierMap[Tile::Type::Wall] =  json.root.Real("wallTileCost");
		}
		GameMap(unsigned int width, unsigned int height)
			: GameMap(NSL::Vector2ui(width, height))
		{
		}

	public:
		void Rebuild(const NSL::Vector2ui& size)
		{
			_size.x = NSL::Max<unsigned int>(size.x, 1);
			_size.y = NSL::Max<unsigned int>(size.y, 1);
			_grid = std::vector<Tile>(size.x * size.y);
			_astar.Resize(size);
		}
		void Rebuild(unsigned int width, unsigned int height)
		{
			Rebuild(NSL::Vector2ui(width, height));
		}

		const Tile& GetTile(unsigned int x, unsigned int y) const
		{
			const size_t index = static_cast<size_t>(y) * _size.x + x;
			return _grid[index];
		}
		void SetTile(unsigned int x, unsigned int y, const Tile& tile)
		{
			const size_t index = static_cast<size_t>(y) * _size.x + x;
			_grid[index] = tile;
			_astar.SetNodeCostMultiplier(x, y, _tiletypeToCostMultiplierMap.at(tile.type));
		}

		int GetWidth() const
		{
			return _size.x;
		}
		int GetHeight() const
		{
			return _size.y;
		}

		std::vector<NSL::Vector2ui> Astar1(const NSL::Vector2ui start, const NSL::Vector2ui destination)
		{
			return _astar.FindPath1(start, destination);
		}
		std::vector<NSL::Vector2ui> Astar2(const NSL::Vector2ui start, const NSL::Vector2ui destination)
		{
			return _astar.FindPath2(start, destination);
		}
		std::vector<NSL::Vector2ui> Astar3(const NSL::Vector2ui start, const NSL::Vector2ui destination)
		{
			return _astar.FindPath3(start, destination);
		}

	private:
		NSL::Vector2ui _size;
		std::vector<Tile> _grid;
		NSL::Astar _astar;
		std::unordered_map<Tile::Type, double> _tiletypeToCostMultiplierMap;
	};
}