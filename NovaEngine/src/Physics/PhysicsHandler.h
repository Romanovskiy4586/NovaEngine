#ifndef PHYSICS__PHYSICSCORE_H
#define PHYSICS__PHYSICSCORE_H

#include "BoundingSphere.h"
#include "BoundingSphereIntersectData.h"
#include <vector>

namespace Physics
{
	class PhysicsHandler final
	{
	public:
		PhysicsHandler() = default;
		PhysicsHandler(const PhysicsHandler& other) = delete;
		PhysicsHandler(PhysicsHandler&& other) = delete;
		PhysicsHandler& operator=(const PhysicsHandler& other) = delete;
		PhysicsHandler& operator=(PhysicsHandler&& other) = delete;
		~PhysicsHandler() = default;

		void Simulation() const NSL_NOEXCEPT;
		void CollisionDetection() const NSL_NOEXCEPT;
		void CollisionRespond() const NSL_NOEXCEPT;

		BoundingSphereIntersectData IntersectSpheres(const BoundingSphere& left, const BoundingSphere& right) const NSL_NOEXCEPT;

	private:
		std::vector<BoundingSphere> _boundingSpheres;
	};
}

#endif // !NOVA__PHYSICS__PHYSICSCORE_H