export module Monitor;
#include "Core.h"
import NSL;

export namespace IO
{
	class NSL_API Monitor
	{
	public:
		struct NSL_API Config
		{
			int width;
			int height;
			int resolutionWidth;
			int resolutionHeight;
			int refreshRate;
			float scale;
		};

	public:
		Monitor() NSL_NOEXCEPT
			: _initialized(false)
			, _width(0)
			, _height(0)
			, _resolutionWidth(0)
			, _resolutionHeight(0)
			, _refreshRate(0)
			, _scale(0)
		{
		}

		void Init(const Config& config) NSL_NOEXCEPT
		{
			if (_initialized) return;
			_initialized = true;

			_width = config.width;
			_height = config.height;
			_resolutionWidth = config.resolutionWidth;
			_resolutionHeight = config.resolutionHeight;
			_refreshRate = config.refreshRate;
			_scale = config.scale;
		}

	public:
		int Width() const NSL_NOEXCEPT
		{
			return _width;
		}
		int Height() const NSL_NOEXCEPT
		{
			return _height;
		}
		int ResolutionWidth() const NSL_NOEXCEPT
		{
			return _resolutionWidth;
		}
		int ResolutionHeight() const NSL_NOEXCEPT
		{
			return _resolutionHeight;
		}
		int RefreshRate() const NSL_NOEXCEPT
		{
			return _refreshRate;
		}
		float GetPPM() const NSL_NOEXCEPT
		{
			return static_cast<float>(_resolutionWidth) / static_cast<float>(_width);
		}
		float Scale() const NSL_NOEXCEPT
		{
			return _scale;
		}

	private:
		bool _initialized;
		int _width;
		int _height;
		int _resolutionWidth;
		int _resolutionHeight;
		int _refreshRate;
		float _scale;
	};
}