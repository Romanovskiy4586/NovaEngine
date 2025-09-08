#include "pch.h"
#include "PhysicsHandler.h"

using namespace Mathematics;

namespace Physics
{
	BoundingSphereIntersectData PhysicsHandler::IntersectSpheres(const BoundingSphere& left, const BoundingSphere& right) const NSL_NOEXCEPT
	{
		float collisionDistance = left.GetRadius() + right.GetRadius();
		Vector3 rightSphereRelativePosition = right.GetPosition() - left.GetPosition();

		float sqrtDistance =
			rightSphereRelativePosition.X * rightSphereRelativePosition.X +
			rightSphereRelativePosition.Y * rightSphereRelativePosition.Y +
			rightSphereRelativePosition.Z * rightSphereRelativePosition.Z;

		float distance = sqrtf(sqrtDistance) - collisionDistance;

		return { distance < 0.0f, distance };
	}
	void PhysicsHandler::Simulation() const NSL_NOEXCEPT
	{
	}
	void PhysicsHandler::CollisionDetection() const NSL_NOEXCEPT
	{
	}
	void PhysicsHandler::CollisionRespond() const NSL_NOEXCEPT
	{
	}
}