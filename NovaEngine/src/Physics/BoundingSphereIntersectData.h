#ifndef PHYSICS__BOUNDINDSPHEREINTERSECTDATA_H
#define PHYSICS__BOUNDINDSPHEREINTERSECTDATA_H

namespace Physics
{
	struct BoundingSphereIntersectData final
	{
	public:
		BoundingSphereIntersectData() = delete;
		BoundingSphereIntersectData(const bool& doesIntersect, const float& distance);
		BoundingSphereIntersectData(const BoundingSphereIntersectData& other) = delete;
		BoundingSphereIntersectData(BoundingSphereIntersectData&& other) = delete;
		BoundingSphereIntersectData& operator=(const BoundingSphereIntersectData& other) = delete;
		BoundingSphereIntersectData& operator=(BoundingSphereIntersectData&& other) = delete;
		~BoundingSphereIntersectData() = default;

		void Print() const NSL_NOEXCEPT;

		bool doesIntersect;
		float distance;
	};
}

#endif // !PHYSICS__BOUNDINDSPHEREINTERSECTDATA_H