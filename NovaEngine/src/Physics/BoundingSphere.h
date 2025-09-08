#ifndef PHYSICS__BOUNDINGSPHERE_H
#define PHYSICS__BOUNDINGSPHERE_H

namespace Physics
{
	class BoundingSphere final
	{
	public:
		BoundingSphere();
		BoundingSphere(const Mathematics::Vector3& position, const float& radius);

		const float& GetRadius() const NSL_NOEXCEPT;
		const Mathematics::Vector3& GetPosition() const NSL_NOEXCEPT;

		void SetRadius(const float& value) NSL_NOEXCEPT;
		void SetPosition(const Mathematics::Vector3& value) NSL_NOEXCEPT;

	private:
		Mathematics::Vector3 _position;
		float _radius;
	};
}

#endif // !PHYSICS__BOUNDINGSPHERE_H