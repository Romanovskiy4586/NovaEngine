export module NPC;
#include "Core.h"
import std;
import NSL;

export namespace Stalker
{
	class NPC
	{
	public:
		enum class Type
		{
			LowtierStalker,
			MidlowtierStalker,
			MidtierStalker,
			MidhightierStalker,
			HightierStalker
		};

	public:
		NPC(Type type, const NSL::Vector2ui& position)
			: _type(type)
			, _position(position)
		{
		}

	private:
		Type _type;
		NSL::Vector2ui _position;
	};
}