export module Model;
#include "Core.h"
import std;
import Mesh;
import Texture2D;
import Transform;
import GLContext;

export namespace Renderer
{
	struct NSL_API Model final
	{
		Model(Mesh& mesh, Texture2D& albedo, Texture2D& normal, Texture2D& metallic,
			Texture2D& ao, Texture2D& roughness) NSL_NOEXCEPT
			: mesh(mesh)
			, albedo(albedo)
			, normal(normal)
			, metallic(metallic)
			, ao(ao)
			, roughness(roughness)
			, transform()
			, context()
		{
		}

		Mesh& mesh;
		Texture2D& albedo;
		Texture2D& normal;
		Texture2D& metallic;
		Texture2D& ao;
		Texture2D& roughness;
		Transform transform;
		GLContext context;
	};
}