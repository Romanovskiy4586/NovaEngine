#include "pch.h"
#include "BoundingSphereIntersectData.h"

namespace Physics
{
	BoundingSphereIntersectData::BoundingSphereIntersectData(const bool& doesIntersect, const float& distance)
		: doesIntersect(doesIntersect), distance(distance)
	{

	}
	void BoundingSphereIntersectData::Print() const NSL_NOEXCEPT
	{
		LogInfo("doesIntersect = " + std::to_string(doesIntersect) + "\ndistance = " + std::to_string(distance) + "\n");
	}
}