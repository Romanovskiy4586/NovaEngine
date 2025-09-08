export module StalkerWorld;
#include "Core.h"
import std;
import NovaEngine;
import Tile;
import GameMap;
import NPC;

export namespace Stalker
{
	class StalkerWorld final
	{
	public:
		StalkerWorld()
			: _gameMap(0, 0)
		{
		}

	public:
		void RebuildMap(int width, int height)
		{
			_gameMap.Rebuild(width, height);

			_emptyTilesPositions.clear();
			_wallTilesPositions.clear();
			_lowtierStalkersPositions.clear();

			NSL::Vector2ui pos;
			for (int x = 0; x < width; ++x)
			{
				for (int y = 0; y < height; ++y)
				{
					pos.x = static_cast<unsigned int>(x);
					pos.y = static_cast<unsigned int>(y);
					_emptyTilesPositions.insert(pos);
				}
			}
		}
		void SetTileType(int x, int y, Stalker::Tile::Type type)
		{
			NSL::Vector2ui position(x, y);

			switch (_gameMap.GetTile(x, y).type)
			{
			case Stalker::Tile::Type::Empty:
				if (_emptyTilesPositions.contains(position)) _emptyTilesPositions.erase(position);
				break;

			case Stalker::Tile::Type::Wall:
				if (_wallTilesPositions.contains(position)) _wallTilesPositions.erase(position);
				break;

			default:
				break;
			}

			switch (type)
			{
			case Stalker::Tile::Type::Empty:
				_emptyTilesPositions.insert(position);
				break;
			case Stalker::Tile::Type::Wall:
				_wallTilesPositions.insert(position);
				break;
			default:
				break;
			}

			Stalker::Tile tile;
			tile.type = type;
			_gameMap.SetTile(x, y, tile);
		}
		const Tile& GetTile(unsigned int x, unsigned int y) const
		{
			return _gameMap.GetTile(x, y);
		}
		const GameMap& GetMap() const
		{
			return _gameMap;
		}

	public:
		std::vector<NSL::Vector2> GetEmptyTilesPositions() const
		{
			std::vector<NSL::Vector2> positions(_emptyTilesPositions.size());
			auto it = _emptyTilesPositions.begin();
			for (auto& pos : positions)
			{
				pos.x = static_cast<float>((*it).x);
				pos.y = static_cast<float>((*it).y);
				++it;
			}
			return positions;
		}
		std::vector<NSL::Vector2> GetWallTilesPositions() const
		{
			std::vector<NSL::Vector2> positions(_wallTilesPositions.size());
			auto it = _wallTilesPositions.begin();
			for (auto& pos : positions)
			{
				pos.x = static_cast<float>((*it).x);
				pos.y = static_cast<float>((*it).y);
				++it;
			}
			return positions;
		}
		std::vector<NSL::Vector2> GetLowtierStalkersPositions() const
		{
			std::vector<NSL::Vector2> positions(_lowtierStalkersPositions.size());
			auto it = _lowtierStalkersPositions.begin();
			for (auto& pos : positions)
			{
				pos.x = static_cast<float>((*it).x);
				pos.y = static_cast<float>((*it).y);
				++it;
			}
			return positions;
		}
		std::vector<NSL::Vector2> GetMidlowtierStalkersPositions() const
		{
			std::vector<NSL::Vector2> positions(_midlowtierStalkersPositions.size());
			auto it = _midlowtierStalkersPositions.begin();
			for (auto& pos : positions)
			{
				pos.x = static_cast<float>((*it).x);
				pos.y = static_cast<float>((*it).y);
				++it;
			}
			return positions;
		}
		std::vector<NSL::Vector2> GetMidtierStalkersPositions() const
		{
			std::vector<NSL::Vector2> positions(_midtierStalkersPositions.size());
			auto it = _midtierStalkersPositions.begin();
			for (auto& pos : positions)
			{
				pos.x = static_cast<float>((*it).x);
				pos.y = static_cast<float>((*it).y);
				++it;
			}
			return positions;
		}
		std::vector<NSL::Vector2> GetMidhightierStalkersPositions() const
		{
			std::vector<NSL::Vector2> positions(_midhightierStalkersPositions.size());
			auto it = _midhightierStalkersPositions.begin();
			for (auto& pos : positions)
			{
				pos.x = static_cast<float>((*it).x);
				pos.y = static_cast<float>((*it).y);
				++it;
			}
			return positions;
		}
		std::vector<NSL::Vector2> GetHightierStalkersPositions() const
		{
			std::vector<NSL::Vector2> positions(_hightierStalkersPositions.size());
			auto it = _hightierStalkersPositions.begin();
			for (auto& pos : positions)
			{
				pos.x = static_cast<float>((*it).x);
				pos.y = static_cast<float>((*it).y);
				++it;
			}
			return positions;
		}

	public:
		void SpawnNPC(NPC::Type type, const NSL::Vector2ui& position)
		{
			_npcs.push_back(NPC(type, position));

			switch (type)
			{
			case Stalker::NPC::Type::LowtierStalker:
				_lowtierStalkersPositions.insert(position);
				break;
			case Stalker::NPC::Type::MidlowtierStalker:
				_midlowtierStalkersPositions.insert(position);
				break;
			case Stalker::NPC::Type::MidtierStalker:
				_midtierStalkersPositions.insert(position);
				break;
			case Stalker::NPC::Type::MidhightierStalker:
				_midhightierStalkersPositions.insert(position);
				break;
			case Stalker::NPC::Type::HightierStalker:
				_hightierStalkersPositions.insert(position);
				break;
			default:
				break;
			}
		}
		std::vector<NSL::Vector2ui> Astar1(const NSL::Vector2ui start, const NSL::Vector2ui destination)
		{
			return _gameMap.Astar1(start, destination);
		}
		std::vector<NSL::Vector2ui> Astar2(const NSL::Vector2ui start, const NSL::Vector2ui destination)
		{
			return _gameMap.Astar2(start, destination);
		}
		std::vector<NSL::Vector2ui> Astar3(const NSL::Vector2ui start, const NSL::Vector2ui destination)
		{
			return _gameMap.Astar3(start, destination);
		}

	private:
		GameMap _gameMap;
		std::vector<NPC> _npcs;

		std::unordered_set<NSL::Vector2ui, NSL::STDHasher<NSL::Vector2ui>> _emptyTilesPositions;
		std::unordered_set<NSL::Vector2ui, NSL::STDHasher<NSL::Vector2ui>> _wallTilesPositions;
		std::unordered_set<NSL::Vector2ui, NSL::STDHasher<NSL::Vector2ui>> _lowtierStalkersPositions;
		std::unordered_set<NSL::Vector2ui, NSL::STDHasher<NSL::Vector2ui>> _midlowtierStalkersPositions;
		std::unordered_set<NSL::Vector2ui, NSL::STDHasher<NSL::Vector2ui>> _midtierStalkersPositions;
		std::unordered_set<NSL::Vector2ui, NSL::STDHasher<NSL::Vector2ui>> _midhightierStalkersPositions;
		std::unordered_set<NSL::Vector2ui, NSL::STDHasher<NSL::Vector2ui>> _hightierStalkersPositions;
	};
}