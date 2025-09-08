export module Astar2DGrid;
#include "Core.h"
import std;
import Mathematics;

export namespace NSL
{
	class NSL_API Astar2DGrid
	{
	public:
		struct NSL_API Node
		{
			Node() NSL_NOEXCEPT
				: position(NSL::Vector2ui::Zero) 
				, gcost(0)
				, hcost(0)
				, fcost(0)
				, costMultiplier(1.0f)
				, parentNode(nullptr)
			{
			}

			NSL::Vector2ui position;
			int gcost; // How far away tile is from origin
			int hcost; // How far away tile is from destination (heuristic)
			int fcost; // fcost = gcost + hcost
			double costMultiplier;
			Node* parentNode;

			//bool operator>(const Node& other) { return fcost > other.fcost; }
			//bool operator>=(const Node& other) { return fcost >= other.fcost; }
			//bool operator<(const Node& other) { return fcost < other.fcost; }
			//bool operator<=(const Node& other) { return fcost <= other.fcost; }
			//bool operator!=(const Node& other) { return fcost != other.fcost; }
			bool operator==(const Node& other) const { return position == other.position; }
		};

	public:
		Astar2DGrid(const NSL::Vector2ui size) NSL_NOEXCEPT
			: _size(size)
			, _grid(size.x * size.y)
		{
			Rebuild();
		}
		Astar2DGrid(unsigned int width, unsigned int height) NSL_NOEXCEPT
			: Astar2DGrid(NSL::Vector2ui(width, height))
		{
		}

	public:
		Node& GetNode(unsigned int x, unsigned int y) NSL_NOEXCEPT
		{
			const size_t index = static_cast<size_t>(y) * _size.x + x;
			return _grid[index];
		}
		Node& GetNode(const NSL::Vector2ui position) NSL_NOEXCEPT
		{
			const size_t index = static_cast<size_t>(position.y) * _size.x + position.x;
			return _grid[index];
		}
		const Node& GetNode(unsigned int x, unsigned int y) const NSL_NOEXCEPT
		{
			const size_t index = static_cast<size_t>(y) * _size.x + x;
			return _grid[index];
		}
		const Node& GetNode(const NSL::Vector2ui position) const NSL_NOEXCEPT
		{
			const size_t index = static_cast<size_t>(position.y) * _size.x + position.x;
			return _grid[index];
		}

		void Resize(const NSL::Vector2ui& size)
		{
			_size.x = size.x;
			_size.y = size.y;
			Rebuild();
		}
		void Resize(unsigned int width, unsigned int height)
		{
			Resize(NSL::Vector2ui(width, height));
		}
		void Rebuild() NSL_NOEXCEPT
		{
			_grid = std::vector<Node>(_size.x * _size.y);

			for (unsigned int x = 0; x < _size.x; ++x)
			{
				for (unsigned int y = 0; y < _size.y; ++y)
				{
					NSL::Vector2ui& position = GetNode(x, y).position;
					position.x = x;
					position.y = y;
				}
			}
		}

		unsigned int GetWidth() const NSL_NOEXCEPT
		{
			return _size.x;
		}
		unsigned int GetHeight() const NSL_NOEXCEPT
		{
			return _size.y;
		}
		const NSL::Vector2ui& GetSize() const NSL_NOEXCEPT
		{
			return _size;
		}

	private:
		NSL::Vector2ui _size;
		std::vector<Node> _grid;
	};
}