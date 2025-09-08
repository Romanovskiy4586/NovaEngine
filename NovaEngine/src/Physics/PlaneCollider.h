#ifndef PHYSICS__PLANECOLLIDER_H
#define PHYSICS__PLANECOLLIDER_H

namespace Physics
{
	class PlaneCollider final
	{
	public:
		PlaneCollider() = default;
		PlaneCollider(const PlaneCollider& other) = delete;
		PlaneCollider(PlaneCollider&& other) = delete;
		PlaneCollider& operator=(const PlaneCollider& other) = delete;
		PlaneCollider& operator=(PlaneCollider&& other) = delete;
		~PlaneCollider() = default;
	private:

	};
}

#endif // !PHYSICS__PLANECOLLIDER_H