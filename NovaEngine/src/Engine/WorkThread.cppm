export module WorkThread;
#include "Core.h"
import std;
import NSL;

export namespace Engine
{
	class NSL_API WorkThread
	{
	public:
		WorkThread() NSL_NOEXCEPT
			: sleepTimeInMilliseconds(100)
			, tasks()
			, _shouldClose(false)
			, thrd([&]() {_Run(); })
		{
			_id = ++s_threadsCounter;
			LogInfo("WorkThread is started. ID: " + std::to_string(_id));
		}
		~WorkThread() NSL_NOEXCEPT
		{
			_shouldClose = true;
			LogInfo("Start to destroy WorkThread. ID: " + std::to_string(_id));
			thrd.join();
			LogInfo("WorkThread is destroyed, thread is joined. ID: " + std::to_string(_id));
			--s_threadsCounter;
		}

	public:
		int sleepTimeInMilliseconds;
		std::vector<std::function<void()>> tasks;

	private:
		void _Run() NSL_NOEXCEPT
		{
			while (!_shouldClose)
			{
				for (auto& task : tasks)
				{
					task();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeInMilliseconds));
			}
		}

	private:
		bool _shouldClose;
		std::thread thrd;
		int _id;

	private:
		static int s_threadsCounter;
	};

	int WorkThread::s_threadsCounter = 0;
}