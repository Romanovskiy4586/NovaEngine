module;
#include "Core.h"

export module World;
import std;

import NovaEngine;
export import TileGrid;
export import Bug;

typedef std::unordered_set<NSL::Vector2i, NSL::STDHasher<NSL::Vector2i>> HashedPositions;

export namespace Bugs
{
	class World
	{
	public:
		World(int width = 1, int height = 1, double delay = 0.0f) NSL_NOEXCEPT
			: _map(width, height)
			, _bugs()
			, _movedBugsIndices()
			, _delay(delay)
			, _deltaSum(0.0)
		{
		}

		const TileGrid& GetMap() const { return _map; }
		TileGrid& GetMap() { return _map; }
		double GetDeltaSum() const NSL_NOEXCEPT { return _deltaSum; }
		const std::vector<Bug>& GetBugs() const NSL_NOEXCEPT { return _bugs; }
		const std::vector<Bug>& GetBugsPreviousState() const NSL_NOEXCEPT { return _bugsPrevoiusState; }
		const std::vector<size_t>& GetMovedBugsIndices() const NSL_NOEXCEPT { return _movedBugsIndices; }

		void SetDelayBetweenUpdates(double delay) NSL_NOEXCEPT
		{
			_delay = delay;
		}
		void SpawnBug(const NSL::Vector2i position)
		{
			NSL_ASSERT(position.x >= 0, "Cannot Spawn bug at that coordinates. X is less than zero")
			NSL_ASSERT(position.y >= 0, "Cannot Spawn bug at that coordinates. Y is less than zero")
			NSL_ASSERT(position.x < _map.GetWidth(), "Cannot Spawn bug at that coordinates. X is out of bounds of the map. \nMap width = " + std::to_string(_map.GetWidth()) + "\nX = " + std::to_string(position.x))
			NSL_ASSERT(position.y < _map.GetHeight(), "Cannot Spawn bug at that coordinates. Y is out of bounds of the map. \nMap height = " + std::to_string(_map.GetHeight()) + "\nY = " + std::to_string(position.y))

			_bugs.push_back(Bug(position, _map));
			_hashedPositions.insert(position);
		}
		void Update(double delta)
		{
			_deltaSum += delta;
			if (_deltaSum < _delay) return;
			delta > _delay ? _deltaSum = delta : _deltaSum -= _delay;

			_movedBugsIndices.clear();
			_bugsPrevoiusState = _bugs;

			for (size_t i = 0; i < _bugs.size(); ++i)
			{
				Bug& bug = _bugs[i];
				Bug& bugPreviousState = _bugsPrevoiusState[i];

				bug.Update(_hashedPositions);

				if (bug.IsMoved())
				{
					_hashedPositions.erase(bugPreviousState.GetPosition());
					_hashedPositions.insert(bug.GetPosition());

					_movedBugsIndices.push_back(i);
				}
			}
		}

	private:
		TileGrid _map;
		std::vector<Bug> _bugs;
		std::vector<Bug> _bugsPrevoiusState;
		std::vector<size_t> _movedBugsIndices;
		HashedPositions _hashedPositions;
		double _delay;
		double _deltaSum;
	};
}