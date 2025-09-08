#include "pch.h"
#include "BoundingSphere.h"

using namespace Mathematics;

namespace Physics
{
	BoundingSphere::BoundingSphere()
		: _position(), _radius(0.0f)
	{

	}
	BoundingSphere::BoundingSphere(const Vector3& position, const float& radius)
		: _position(position), _radius(radius)
	{

	}

	const float& BoundingSphere::GetRadius() const NSL_NOEXCEPT
	{
		return _radius;
	}
	const Vector3& BoundingSphere::GetPosition() const NSL_NOEXCEPT
	{
		return _position;
	}

	void BoundingSphere::SetRadius(const float& value) NSL_NOEXCEPT
	{
		_radius = value;
	}
	void BoundingSphere::SetPosition(const Vector3& value) NSL_NOEXCEPT
	{
		_position = value;
	}
}