export module Ray;
#include "Core.h"
import Vector3;

export namespace NSL
{
	struct NSL_API Ray
	{
		Ray(Vector3 origin = Vector3::Zero, Vector3 direction = Vector3::Zero) NSL_NOEXCEPT
			: origin(origin)
			, direction(direction)
		{
		}

		Vector3 origin;
		Vector3 direction;
	};
}