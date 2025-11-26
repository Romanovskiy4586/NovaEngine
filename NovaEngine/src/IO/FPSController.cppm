export module FPSController;
import NSL;
#include "Core.h"
import std;

using namespace std::chrono;
using namespace std::this_thread;

export namespace IO
{
	class FPSController
	{
	public:
		FPSController(int fpsLock = 0, bool lockFPS = false) NSL_NOEXCEPT
			: lockFPS(lockFPS)
			, _fpsLock(fpsLock)
			, _frameDurationNeeded(0ll)
			, _startPoint()
			, _frameDuration(0ll)
			, _sleepTimeTrashHold(17000000ll)
			, _sleepTime(1000000ll)
		{
			SetFPSLock(fpsLock);
		}

	public:
		void SetFPSLock(int fpsLock) NSL_NOEXCEPT
		{
			if (fpsLock <= 0)
			{
				_fpsLock = 0;
				_frameDurationNeeded = 0ll;
				return;
			}
			_fpsLock = fpsLock;
			_frameDurationNeeded = 1000000000ll / fpsLock; // Converts FPS into nanoseconds per frame
		}
		int GetFPSLock() const NSL_NOEXCEPT
		{
			return _fpsLock;
		}
		void StartFrame() NSL_NOEXCEPT
		{
			_startPoint = steady_clock::now();
		}
		double EndFrame() NSL_NOEXCEPT
		{
			_frameDuration = (steady_clock::now() - _startPoint).count();
			if (!_frameDurationNeeded || !lockFPS) return static_cast<double>(_frameDuration) / 1000000.0;

			// Sleep until we've got too close to needed frame duration time
			while (_frameDurationNeeded - _frameDuration > _sleepTimeTrashHold)
			{
				sleep_for(nanoseconds(_sleepTime));
				_frameDuration = (steady_clock::now() - _startPoint).count();
			}

			// Then we start Spin-lock to get to needed frame duration percicely
			while (_frameDuration < _frameDurationNeeded)
			{
				yield();
				_frameDuration = (steady_clock::now() - _startPoint).count();
			}

			return static_cast<double>(_frameDuration) / 1000000.0; // Returns delta time in milliseconds
		}

	public:
		bool lockFPS;

	private:
		int _fpsLock;
		long long _frameDurationNeeded;
		std::chrono::steady_clock::time_point _startPoint;
		long long _frameDuration;
		long long _sleepTimeTrashHold;
		long long _sleepTime;
	};
}