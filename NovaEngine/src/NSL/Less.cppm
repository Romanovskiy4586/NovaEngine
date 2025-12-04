export module Less;
#include "Core.h"
import Astar2DGrid;

export namespace NSL
{
	template <class T>
	class NSL_API Less
	{
	public:
		bool operator()(const T& left, const T& right) const NSL_NOEXCEPT
		{
			return left < right;
		}
	};

	template <>
	class NSL_API Less<Astar2DGrid::Node*>
	{
	public:
		bool operator()(const Astar2DGrid::Node* const& left, const Astar2DGrid::Node* const& right) const NSL_NOEXCEPT
		{
			if (left->fcost != right->fcost)			return left->fcost < right->fcost;
			if (left->hcost != right->hcost)			return left->hcost < right->hcost;
			if (left->position.x != right->position.x)	return left->position.x < right->position.x;

			return left->position.y < right->position.y;
		}
	};

	template <>
	class NSL_API Less<Astar2DGrid::Node>
	{
	public:
		bool operator()(const Astar2DGrid::Node& left, const Astar2DGrid::Node& right) const NSL_NOEXCEPT
		{
			if (left.fcost != right.fcost)				return left.fcost < right.fcost;
			if (left.hcost != right.hcost)				return left.hcost < right.hcost;
			//if (left.position != right.position)		return left.position < right.position;
			if (left.position.x != right.position.x)	return left.position.x < right.position.x;

			return left.position.y < right.position.y;
		}
	};
}