export module AssetsManager;
#include "Core.h"
import std;
import NSL;
import Model;
import Sprite;
import Mesh;
import Texture2D;

export namespace Renderer
{
	class NSL_API AssetsManager final
	{
	public:
		void BindIntoModel(const std::string& name, Mesh& mesh, Texture2D& albedo, Texture2D& normal,
			Texture2D& metallic, Texture2D& ao, Texture2D& roughness) NSL_NOEXCEPT
		{
			_models.try_emplace(name, mesh, albedo, normal, metallic, ao, roughness);
		}
		void BindIntoSprite(const std::string& name, Texture2D& texture) NSL_NOEXCEPT
		{
			_sprites.try_emplace(name, texture);
		}

		Model& GetModel(const std::string& name) NSL_NOEXCEPT
		{
			return _models.at(name);
		}
		const Model& GetModel(const std::string& name) const NSL_NOEXCEPT
		{
			return _models.at(name);
		}
		Sprite& GetSprite(const std::string& name) NSL_NOEXCEPT
		{
			return _sprites.at(name);
		}
		const Sprite& GetSprite(const std::string& name) const NSL_NOEXCEPT
		{
			return _sprites.at(name);
		}

	private:
		std::unordered_map<std::string, Model> _models;
		std::unordered_map<std::string, Sprite> _sprites;
	};
}