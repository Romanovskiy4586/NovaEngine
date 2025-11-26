export module MouseState;
#include "Core.h"
import std;
import MouseButtons;
import NSL;

export namespace IO
{
	enum class NSL_API ButtonState
	{
		Released = 0,
		Pressed = 1,
		Down = 2
	};

	class NSL_API MouseState
	{
	public:
		NSL::Vector2 GetDelta() const NSL_NOEXCEPT
		{
			return _positionDelta;
		}
		NSL::Vector2 GetPosition() const NSL_NOEXCEPT
		{
			return _position;
		}
		float GetScrollDelta() const NSL_NOEXCEPT
		{
			return _scrollDelta;
		}

		void SetPosition(float x, float y) NSL_NOEXCEPT
		{
			_positionDelta.x = _position.x - x;
			_positionDelta.y = _position.y - y;

			_position.x = x;
			_position.y = y;
		}
		void SetPosition(const NSL::Vector2& position) NSL_NOEXCEPT
		{
			SetPosition(position.x, position.y);
		}
		void SetButtonState(MouseButton button, ButtonState action) NSL_NOEXCEPT
		{
			if (_buttons[static_cast<int>(button)] == ButtonState::Down && action == ButtonState::Pressed) return;
			_buttons[static_cast<int>(button)] = action;
		}
		void SetScrollDelta(float delta)
		{
			_scrollDelta = delta;
		}

		bool IsButtonPressed(MouseButton button) const NSL_NOEXCEPT
		{
			return _buttons[static_cast<int>(button)] == ButtonState::Pressed;
		}
		bool IsButtonDown(MouseButton button) const NSL_NOEXCEPT
		{
			return _buttons[static_cast<int>(button)] == ButtonState::Down || _buttons[static_cast<int>(button)] == ButtonState::Pressed;
		}
		bool IsButtonReleased(MouseButton button) const NSL_NOEXCEPT
		{
			return _buttons[static_cast<int>(button)] == ButtonState::Released;
		}
		void UpdateMouseState() NSL_NOEXCEPT
		{
			for (int i = 0; i < s_keysCount; ++i)
			{
				if (_buttons.at(i) == ButtonState::Pressed)
				{
					_buttons.at(i) = ButtonState::Down;
				}
			}
			_positionDelta.x = 0.0f;
			_positionDelta.y = 0.0f;
			_scrollDelta = 0.0f;
		}

	private:
		static const size_t s_keysCount = 3;
		std::array<ButtonState, s_keysCount> _buttons;
		NSL::Vector2 _position;
		NSL::Vector2 _positionDelta;
		float _scrollDelta;
	};
}