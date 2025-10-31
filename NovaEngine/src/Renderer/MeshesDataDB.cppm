export module MeshesDataDB;
#include "Core.h"
import std;
import Mesh;

export namespace Renderer
{
	struct NSL_API MeshesDataDB final
	{
		const Mesh screenspaceSprite = Mesh
		(
			{ -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f },
			2,
			{ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },
			{},
			{ 0, 1, 2, 0, 2, 3 },
			{ 2, 2 }
		);
		const Mesh sprite = Mesh
		(
			{ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },
			2,
			{ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },
			{},
			{ 0, 1, 2, 0, 2, 3 },
			{ 2, 2 }
		);
		const Mesh spriteCentered = Mesh
		(
			{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f },
			2,
			{ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },
			{},
			{ 0, 1, 2, 0, 2, 3 },
			{ 2, 2 }
		);
		const Mesh glyph = Mesh
		(
			{ 0.0f, 0.0f, 0.6f, 0.0f, 0.6f, 1.4f, 0.0f, 1.4f },
			2,
			{ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },
			{},
			{ 0, 1, 2, 0, 2, 3 },
			{ 2, 2 }
		);
	};
}