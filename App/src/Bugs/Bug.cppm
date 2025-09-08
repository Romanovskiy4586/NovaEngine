export module Bug;

import NovaEngine;
import TileGrid;

#include "Core.h"
#include <unordered_set>

using HashedPositions = std::unordered_set<NSL::Vector2i, NSL::STDHasher<NSL::Vector2i>>;

export namespace Bugs
{
	class Bug
	{
	public: // Classes
		enum State : char
		{
			Waiting =		0,
			MovingUp =		1,
			MovingDown =	2,
			MovingLeft =	3,
			MovingRight =	4
		};

	public: // Constructors
		Bug(const NSL::Vector2i& position, TileGrid& map) NSL_NOEXCEPT
			: _position(position)
			, _counter(NSL::Random(0, s_waitingCounterMax))
			, _state(State::Waiting)
			, _map(map)
			, _moved(false)
		{
		}
		Bug& operator=(const Bug& other)
		{
			if (this != &other)
			{
				_position = other._position;
				_counter = other._counter;
				_state = other._state;
				_map = other._map;
				_moved = other._moved;
			}
			return *this;
		}

	public: // Getters/Setters
		const NSL::Vector2i& GetPosition() const NSL_NOEXCEPT
		{
			return _position;
		}
		int GetX() const NSL_NOEXCEPT
		{
			return _position.x;
		}
		int GetY() const NSL_NOEXCEPT
		{
			return _position.y;
		}

		void SetPosition(const NSL::Vector2i& position) NSL_NOEXCEPT
		{
			_position = position;
		}
		void SetX(int x) NSL_NOEXCEPT
		{
			_position.x = x;
		}
		void SetY(int y) NSL_NOEXCEPT
		{
			_position.y = y;
		}

	public: // Behaviour
		void Update(const HashedPositions& hashedPositions) NSL_NOEXCEPT
		{
			_moved = false;

			// Куда то шёл и пришёл - постой подумай
			if (State::MovingUp <= _state && _state <= State::MovingRight && !_counter)
			{
				_state = State::Waiting;
				_counter = NSL::Random(s_waitingCounterMin, s_waitingCounterMax);
			}

			// Постоял подумал - реши куда идти и как далеко
			if (_state == State::Waiting && !_counter)
			{
				_state = static_cast<State>(NSL::Random(static_cast<int>(State::MovingUp), static_cast<int>(State::MovingRight)));
				_counter = NSL::Random(s_movingCounterMin, s_movingCounterMax);
			}

			// Действуй исходя из своего состояния
			switch (_state)
			{
			case State::Waiting:
				break;

			case State::MovingUp:
				if (_CanGoUp(hashedPositions))
				{
					++_position.y;
					_moved = true;
				}
				else
				{
					_counter = 1;
				}
				break;

			case State::MovingDown:
				if (_CanGoDown(hashedPositions))
				{
					--_position.y;
					_moved = true;
				}
				else
				{
					_counter = 1;
				}
				break;

			case State::MovingLeft:
				if (_CanGoLeft(hashedPositions))
				{
					--_position.x;
					_moved = true;
				}
				else
				{
					_counter = 1;
				}
				break;

			case State::MovingRight:
				if (_CanGoRight(hashedPositions))
				{
					++_position.x;
					_moved = true;
				}
				else
				{
					_counter = 1;
				}
				break;

			default:
				break;
			}

			--_counter;
		}
		inline bool IsMoved() const NSL_NOEXCEPT
		{
			return _moved;
		}

	private: // Private helpers
		bool _CanGoUp(const HashedPositions& hashedPositions) NSL_NOEXCEPT
		{
			int& xCurrent = _position.x;
			int& yCurrent = _position.y;

			int xNext = xCurrent;
			int yNext = yCurrent + 1;

			if (yNext >= _map.GetHeight()) return false;

			if (hashedPositions.contains(NSL::Vector2i(xNext, yNext)))
			{
				return false;
			}

			return _map[xNext][yNext].type == TileGrid::Tile::Type::Empty;
		}
		bool _CanGoDown(const HashedPositions& hashedPositions) NSL_NOEXCEPT
		{
			int& xCurrent = _position.x;
			int& yCurrent = _position.y;

			int xNext = xCurrent;
			int yNext = yCurrent - 1;

			if (yNext < 0) return false;

			if (hashedPositions.contains(NSL::Vector2i(xNext, yNext)))
			{
				return false;
			}

			return _map[xNext][yNext].type == TileGrid::Tile::Type::Empty;
		}
		bool _CanGoLeft(const HashedPositions& hashedPositions) NSL_NOEXCEPT
		{
			int& xCurrent = _position.x;
			int& yCurrent = _position.y;

			int xNext = xCurrent - 1;
			int yNext = yCurrent;

			if (xNext < 0) return false;

			if (hashedPositions.contains(NSL::Vector2i(xNext, yNext)))
			{
				return false;
			}

			return _map[xNext][yNext].type == TileGrid::Tile::Type::Empty;
		}
		bool _CanGoRight(const HashedPositions& hashedPositions) NSL_NOEXCEPT
		{
			int& xCurrent = _position.x;
			int& yCurrent = _position.y;

			int xNext = xCurrent + 1;
			int yNext = yCurrent;

			if (xNext >= _map.GetWidth()) return false;


			if (hashedPositions.contains(NSL::Vector2i(xNext, yNext)))
			{
				return false;
			}

			return _map[xNext][yNext].type == TileGrid::Tile::Type::Empty;
		}

	private: // Private fields
		NSL::Vector2i _position;

		int _counter;
		State _state;
		TileGrid& _map;
		bool _moved;

		static const int s_waitingCounterMin = 5;
		static const int s_waitingCounterMax = 10;

		static const int s_movingCounterMin = 1;
		static const int s_movingCounterMax = 3;
	};
}