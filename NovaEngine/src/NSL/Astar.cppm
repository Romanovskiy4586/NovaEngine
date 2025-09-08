export module Astar;
#include "Core.h"
import std;
import Astar2DGrid;
import Mathematics;
import STDLess;
import STDHasher;

const float DIAGONAL_COST = 1.4f;
const float STRAIGHT_COST = 1.0f;

export namespace NSL
{
	class NSL_API Astar
	{
	public:
		Astar(const NSL::Vector2ui& size) NSL_NOEXCEPT
			: _grid(size)
			, _openList()
			, _sortedOpenList()
			, _closeList()
		{
		}

	public:
		void Resize(const NSL::Vector2ui& size)
		{
			_grid.Resize(size);
		}
		void Resize(unsigned int width, unsigned int height)
		{
			Resize(NSL::Vector2ui(width, height));
		}
		
		std::vector<NSL::Vector2ui> FindPath1(const NSL::Vector2ui& start, const NSL::Vector2ui& end)
		{
			_Clear();

			// It will be result path
			std::vector<NSL::Vector2ui> path;

			// Initialize astar by pushing start node in open list
			_openList.push_back(&_grid.GetNode(start));

			while (!_openList.empty())
			{
				Astar2DGrid::Node* currentNode = _FindNodeWithMinFcostInOpenList();
				_openList.erase(std::find(_openList.begin(), _openList.end(), currentNode));
				_closeList.insert(currentNode);

				// If current node is destination - track path from it to start and return vector with positions
				if (currentNode->position == end)
				{
					while (currentNode->parentNode)
					{
						path.push_back(currentNode->position);
						currentNode = currentNode->parentNode;
					}
					std::reverse(path.begin(), path.end());
					return path;
				}

				NSL::Vector2ui& position = currentNode->position;
				NSL::Vector2ui neighbourPosition;

				for (int x = -1; x <= 1; ++x)
				{
					for (int y = -1; y <= 1; ++y)
					{
						// Continue if it over the down or left bounds
						if (position.x == 0 && x == -1 ||
							position.y == 0 && y == -1) continue;

						neighbourPosition.x = position.x + x;
						neighbourPosition.y = position.y + y;

						// Continue if it is over the up or right bounds
						if (neighbourPosition.x >= _grid.GetWidth() ||
							neighbourPosition.y >= _grid.GetHeight()) continue;

						// Continue if node cost multiplier is less than zero, means that tile is unvisitable
						Astar2DGrid::Node* neighbourNode = &_grid.GetNode(neighbourPosition);
						if (neighbourNode->costMultiplier < 0.0f) continue;

						// Continue if this node is in close list
						if (_closeList.contains(neighbourNode)) continue;

						// Continue if this node is diagonal but neighbours straight nodes is unwalkable
						if (std::abs(x) == 1 && std::abs(y) == 1 &&
							_grid.GetNode(position.x + x, position.y).costMultiplier < 0.0f &&
							_grid.GetNode(position.x, position.y + y).costMultiplier < 0.0f)
						{
							continue;
						}

						// All checks passed. We sure that this node is valid to pathfinding
						// Precalculate neighbour node
						Astar2DGrid::Node precalculatedNode = *neighbourNode;
						precalculatedNode.gcost = currentNode->gcost;
						precalculatedNode.gcost +=
							std::abs(x) == 1 && std::abs(y) == 1
							? static_cast<int>(DIAGONAL_COST * precalculatedNode.costMultiplier)
							: static_cast<int>(STRAIGHT_COST * precalculatedNode.costMultiplier);
						precalculatedNode.hcost = _CalculateHcost(precalculatedNode.position, end);
						precalculatedNode.fcost = precalculatedNode.gcost + precalculatedNode.hcost;
						precalculatedNode.parentNode = currentNode;
						
						// If precalculated node fcost is better than it were calculated earlier
						// - current calculated path is more efficient, so we update this node
						if (std::find(_openList.begin(), _openList.end(), neighbourNode) != _openList.end() &&
							neighbourNode->fcost > precalculatedNode.fcost)
						{
							*neighbourNode = precalculatedNode;
						}

						// Push neighbour node in open list only if it is not there already
						if (std::find(_openList.begin(), _openList.end(), neighbourNode) == _openList.end())
						{
							*neighbourNode = precalculatedNode;
							_openList.push_back(neighbourNode);
						}
					}
				}
			}

			// Path was not founded. Returned empty vector
			return path;
		}
		std::vector<NSL::Vector2ui> FindPath2(const NSL::Vector2ui& start, const NSL::Vector2ui& end)
		{
			_Clear();

			// It will be result path
			std::vector<NSL::Vector2ui> path;

			// Initialize astar by pushing start node in open list
			_sortedOpenList.insert(_grid.GetNode(start));

			while (!_sortedOpenList.empty())
			{
				const Astar2DGrid::Node currentNode = _GetLowestFcostNodeInOpenList();
				_sortedOpenList.erase(currentNode);
				_newCloseList.insert(currentNode);

				// If current node is destination - track path from it to start and return vector with positions
				if (currentNode.position == end)
				{
					path.push_back(currentNode.position);
					Astar2DGrid::Node* parentNode = currentNode.parentNode;
					while (parentNode)
					{
						path.push_back(parentNode->position);
						parentNode = parentNode->parentNode;
					}
					std::reverse(path.begin(), path.end());
					return path;
				}

				const NSL::Vector2ui& position = currentNode.position;
				NSL::Vector2ui neighbourPosition;

				for (int x = -1; x <= 1; ++x)
				{
					for (int y = -1; y <= 1; ++y)
					{
						// Continue if it over the down or left bounds
						if (position.x == 0 && x == -1 ||
							position.y == 0 && y == -1) continue;

						neighbourPosition.x = position.x + x;
						neighbourPosition.y = position.y + y;

						// Continue if it is over the up or right bounds
						if (neighbourPosition.x >= _grid.GetWidth() ||
							neighbourPosition.y >= _grid.GetHeight()) continue;

						// Continue if node cost multiplier is less than zero, means that tile is unvisitable
						Astar2DGrid::Node& neighbourNode = _grid.GetNode(neighbourPosition);
						if (neighbourNode.costMultiplier < 0.0f) continue;

						// Continue if this node is in close list
						if (_newCloseList.contains(neighbourNode)) continue;

						// Continue if this node is diagonal but neighbours straight nodes is unwalkable
						if (std::abs(x) == 1 && std::abs(y) == 1 &&
							_grid.GetNode(position.x + x, position.y).costMultiplier < 0.0f &&
							_grid.GetNode(position.x, position.y + y).costMultiplier < 0.0f)
						{
							continue;
						}

						// All checks passed. We sure that this node is valid to pathfinding
						// Precalculate neighbour node
						Astar2DGrid::Node precalculatedNode = neighbourNode;
						precalculatedNode.gcost = currentNode.gcost;
						precalculatedNode.gcost +=
							std::abs(x) == 1 && std::abs(y) == 1
							? static_cast<int>(DIAGONAL_COST * precalculatedNode.costMultiplier)
							: static_cast<int>(STRAIGHT_COST * precalculatedNode.costMultiplier);
						precalculatedNode.hcost = _CalculateHcost(precalculatedNode.position, end);
						precalculatedNode.fcost = precalculatedNode.gcost + precalculatedNode.hcost;
						precalculatedNode.parentNode = &_grid.GetNode(currentNode.position);

						// If precalculated node fcost is better than it were calculated earlier
						// - current calculated path is more efficient, so we update this node
						if (_sortedOpenList.contains(neighbourNode) && neighbourNode.fcost > precalculatedNode.fcost)
						{
							neighbourNode = precalculatedNode;
						}

						// Push neighbour node in open list only if it is not there already
						if (!_sortedOpenList.contains(neighbourNode))
						{
							neighbourNode = precalculatedNode;
							_sortedOpenList.insert(neighbourNode);
						}
					}
				}
			}

			// Path was not founded. Returned empty vector
			return path;
		}
		std::vector<NSL::Vector2ui> FindPath3(const NSL::Vector2ui& start, const NSL::Vector2ui& end)
		{
			_Clear();

			// It will be result path
			std::vector<NSL::Vector2ui> path;

			// Initialize astar by pushing start node in open list
			_sortedOpenListPointers.insert(&_grid.GetNode(start));

			while (!_sortedOpenListPointers.empty())
			{
				Astar2DGrid::Node* currentNode = _FastGetLowestFcostNodeInOpenList();
				_sortedOpenListPointers.erase(currentNode);
				_closeList.insert(currentNode);

				// If current node is destination - track path from it to start and return vector with positions
				if (currentNode->position == end)
				{
					while (currentNode->parentNode)
					{
						path.push_back(currentNode->position);
						currentNode = currentNode->parentNode;
					}
					std::reverse(path.begin(), path.end());
					return path;
				}

				NSL::Vector2ui& position = currentNode->position;
				NSL::Vector2ui neighbourPosition;

				for (int x = -1; x <= 1; ++x)
				{
					for (int y = -1; y <= 1; ++y)
					{
						// Continue if it over the down or left bounds
						if (position.x == 0 && x == -1 ||
							position.y == 0 && y == -1) continue;

						neighbourPosition.x = position.x + x;
						neighbourPosition.y = position.y + y;

						// Continue if it is over the up or right bounds
						if (neighbourPosition.x >= _grid.GetWidth() ||
							neighbourPosition.y >= _grid.GetHeight()) continue;

						// Continue if node cost multiplier is less than zero, means that tile is unvisitable
						Astar2DGrid::Node* neighbourNode = &_grid.GetNode(neighbourPosition);
						if (neighbourNode->costMultiplier < 0.0f)
						{
							_closeList.insert(neighbourNode);
							continue;
						}

						// Continue if this node is in close list
						if (_closeList.contains(neighbourNode)) continue;

						// Continue if this node is diagonal but neighbours straight nodes is unwalkable
						if (std::abs(x) == 1 && std::abs(y) == 1 &&
							_grid.GetNode(position.x + x, position.y).costMultiplier < 0.0f &&
							_grid.GetNode(position.x, position.y + y).costMultiplier < 0.0f)
						{
							continue;
						}

						// All checks passed. We sure that this node is valid to pathfinding
						// Precalculate neighbour node
						Astar2DGrid::Node precalculatedNode = *neighbourNode;
						precalculatedNode.gcost = currentNode->gcost;
						precalculatedNode.gcost +=
							std::abs(x) == 1 && std::abs(y) == 1
							? static_cast<int>(DIAGONAL_COST * precalculatedNode.costMultiplier)
							: static_cast<int>(STRAIGHT_COST * precalculatedNode.costMultiplier);
						precalculatedNode.hcost = _CalculateHcost(precalculatedNode.position, end);
						precalculatedNode.fcost = precalculatedNode.gcost + precalculatedNode.hcost;
						precalculatedNode.parentNode = currentNode;

						// If precalculated node fcost is better than it were calculated earlier
						// - current calculated path is more efficient, so we update this node
						if (_sortedOpenListPointers.contains(neighbourNode) &&
							neighbourNode->fcost > precalculatedNode.fcost)
						{
							*neighbourNode = precalculatedNode;
							_sortedOpenListPointers.erase(neighbourNode);
							_sortedOpenListPointers.insert(neighbourNode);
						}

						// Push neighbour node in open list only if it is not there already
						if (!_sortedOpenListPointers.contains(neighbourNode))
						{
							*neighbourNode = precalculatedNode;
							_sortedOpenListPointers.insert(neighbourNode);
						}
					}
				}
			}

			// Path was not founded. Returned empty vector
			return path;
		}
		void SetNodeCostMultiplier(unsigned int x, unsigned int y, double costMultiplier) NSL_NOEXCEPT
		{
			_grid.GetNode(x, y).costMultiplier = costMultiplier;
		}
		void SetNodeCostMultiplier(const NSL::Vector2ui& position, float costMultiplier) NSL_NOEXCEPT
		{
			_grid.GetNode(position).costMultiplier = costMultiplier;
		}

	private:
		Astar2DGrid::Node* _FindNodeWithMinFcostInOpenList() NSL_NOEXCEPT
		{
			Astar2DGrid::Node* resultNode = _openList[0];

			for (size_t i = 1; i < _openList.size(); ++i)
			{
				Astar2DGrid::Node* currentNode = _openList[i];

				if (currentNode->fcost < resultNode->fcost)
				{
					resultNode = currentNode;
				}
				else if (currentNode->fcost == resultNode->fcost && currentNode->hcost < resultNode->fcost)
				{
					resultNode = currentNode;
				}
			}

			return resultNode;
		}
		const Astar2DGrid::Node& _GetLowestFcostNodeInOpenList() NSL_NOEXCEPT
		{
			//auto& ref = *_sortedOpenList.begin();
			return *_sortedOpenList.begin();
		}
		Astar2DGrid::Node* _FastGetLowestFcostNodeInOpenList() NSL_NOEXCEPT
		{
			//auto& ref = *_sortedOpenList.begin();
			return *_sortedOpenListPointers.begin();
		}
		int _CalculateHcost(const NSL::Vector2ui& start, const NSL::Vector2ui& end) NSL_NOEXCEPT
		{
			int hcost = 0;
			NSL::Vector2ui currentPosition = start;
			bool movedHorizontal;
			bool movedVertical;

			while (currentPosition != end)
			{
				movedHorizontal = false;
				movedVertical = false;

				if (currentPosition.x > end.x)
				{
					--currentPosition.x;
					movedHorizontal = true;
				}
				if (currentPosition.x < end.x)
				{
					++currentPosition.x;
					movedHorizontal = true;
				}

				if (currentPosition.y > end.y)
				{
					--currentPosition.y;
					movedVertical = true;
				}
				if (currentPosition.y < end.y)
				{
					++currentPosition.y;
					movedVertical = true;
				}

				if (movedHorizontal && movedVertical)
				{
					hcost += DIAGONAL_COST;
				}
				else
				{
					hcost += STRAIGHT_COST;
				}
			}

			return hcost;
		}
		void _Clear() NSL_NOEXCEPT
		{
			_openList.clear();
			_sortedOpenList.clear();
			_closeList.clear();
			_newCloseList.clear();
		}

	private:
		Astar2DGrid _grid;
		std::vector<Astar2DGrid::Node*> _openList;
		std::set<Astar2DGrid::Node, NSL::STDLess<Astar2DGrid::Node>> _sortedOpenList;
		std::set<Astar2DGrid::Node*, NSL::STDLess<Astar2DGrid::Node*>> _sortedOpenListPointers;
		std::unordered_set<Astar2DGrid::Node*> _closeList;
		std::unordered_set<Astar2DGrid::Node, NSL::STDHasher<Astar2DGrid::Node>> _newCloseList;
		
		const int DIAGONAL_COST = 14;
		const int STRAIGHT_COST = 10;
	};
}