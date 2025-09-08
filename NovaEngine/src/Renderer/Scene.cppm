export module Scene;
#include "Core.h"
import std;
import Camera;
import Sprite;

export namespace Renderer
{
	struct NSL_API Scene final
	{
		struct NSL_API RenderParameters
		{
			enum class NSL_API SortingByDistance
			{
				None, NearToFar, FarToNear
			};

			RenderParameters() NSL_NOEXCEPT
				: sortingByDistance(SortingByDistance::None)
				, sharpness(0.0f)
			{
			}

			SortingByDistance sortingByDistance;
			float sharpness;
		};

		Scene() NSL_NOEXCEPT
			: camera(Camera::ProjectionType::Perspective)
			, sprites()
		{
		}

		Camera camera;
		std::vector<std::string> sprites;
		std::vector<std::string> models;
		RenderParameters renderParameters;
	};
}