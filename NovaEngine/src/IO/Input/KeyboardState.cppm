export module KeyboardState;
#include "Core.h"
import std;
import KeyboardKeys;

export namespace IO
{
	enum class NSL_API KeyState : unsigned char
	{
		Released = 0,
		Pressed = 1,
		Down = 2
	};

	class NSL_API KeyboardState
	{
	public:
		KeyboardState() NSL_NOEXCEPT
			: _keys()
		{
			_keys.fill(KeyState::Released);
		}

	public:
		void SetButtonState(KeyboardKey key, KeyState action) NSL_NOEXCEPT
		{
			if (_keys[static_cast<int>(key)] == KeyState::Down && action == KeyState::Pressed) return;

			_keys[static_cast<int>(key)] = action;
		}
		bool IsKeyPressed(KeyboardKey key) const NSL_NOEXCEPT
		{
			return _keys[static_cast<int>(key)] == KeyState::Pressed;
		}
		bool IsKeyDown(KeyboardKey key) const NSL_NOEXCEPT
		{
			return _keys[static_cast<int>(key)] == KeyState::Down || _keys[static_cast<int>(key)] == KeyState::Pressed;
		}
		bool IsKeyReleased(KeyboardKey key) const NSL_NOEXCEPT
		{
			return _keys[static_cast<int>(key)] == KeyState::Released;
		}
		void UpdateKeyboardState() NSL_NOEXCEPT
		{
			for (size_t i = 0; i < s_keysCount; ++i)
			{
				if (_keys.at(i) == KeyState::Pressed)
				{
					_keys.at(i) = KeyState::Down;
				}
			}
		}

	private:
		static const size_t s_keysCount = 512;
		std::array<KeyState, s_keysCount> _keys;
	};
}