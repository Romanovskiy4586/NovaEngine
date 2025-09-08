export module STDHasher;
#include "Core.h"
import std;
import Vector2;
import Vector2i;
import Vector2ui;
import Astar2DGrid;

export namespace NSL
{
	template <typename T>
	struct STDHasher
	{
		size_t operator()(const T& value) const NSL_NOEXCEPT
		{
			return std::hash<T>()(value);
		}
	};

	template <>
	struct STDHasher<std::string>
	{
		size_t operator()(const std::string& value) const NSL_NOEXCEPT
		{
			return std::hash<std::string>()(value);
		}
	};

	template <>
	struct STDHasher<Vector2>
	{
		size_t operator()(const Vector2& value) const NSL_NOEXCEPT
		{
			return std::hash<float>()(value.x) ^ std::hash<float>()(value.y) << 1;
		}
	};

	template <>
	struct STDHasher<Vector2i>
	{
		size_t operator()(const Vector2i& value) const NSL_NOEXCEPT
		{
			return std::hash<int>()(value.x) ^ std::hash<int>()(value.y) << 1;
		}
	};

	template <>
	struct STDHasher<Vector2ui>
	{
		size_t operator()(const Vector2ui& value) const NSL_NOEXCEPT
		{
			return std::hash<unsigned int>()(value.x) ^ std::hash<unsigned int>()(value.y) << 1;
		}
	};

	template <>
	struct STDHasher<Astar2DGrid::Node>
	{
		size_t operator()(const Astar2DGrid::Node& value) const NSL_NOEXCEPT
		{
			return std::hash<unsigned int>()(value.position.x) ^ std::hash<unsigned int>()(value.position.y) << 1;
		}
	};
}