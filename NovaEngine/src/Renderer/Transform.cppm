export module Transform;
#include "Core.h"
import std;
import NSL;

export namespace Renderer
{
	class NSL_API Transform final
	{
	public:
		Transform() NSL_NOEXCEPT
			: _position(NSL::Vector3::Zero)
			, _rotation(NSL::Vector3::Zero)
			, _scale(NSL::Vector3::Unit)
			, _modelMatrix(NSL::Matrix4x4::Unit)
		{
			_RecalculateModelMatrix();
		}

		const NSL::Vector3& GetPosition() const NSL_NOEXCEPT
		{
			return _position;
		}
		const NSL::Vector3& GetRotation() const NSL_NOEXCEPT
		{
			return _rotation;
		}
		const NSL::Vector3& GetScale() const NSL_NOEXCEPT
		{
			return _scale;
		}
		const NSL::Matrix4x4& GetModelMatrix() const NSL_NOEXCEPT
		{
			return _modelMatrix;
		}

		void SetPosition(const NSL::Vector3& position) NSL_NOEXCEPT
		{
			_position = position;
			_modelMatrix.col3.x = 0.0f;
			_modelMatrix.col3.y = 0.0f;
			_modelMatrix.col3.z = 0.0f;
			_modelMatrix = NSL::Translate(_modelMatrix, _position);
		}
		void SetRotation(const NSL::Vector3& rotation) NSL_NOEXCEPT
		{
			_rotation = rotation;
			_RecalculateModelMatrix();
		}
		void SetScale(const NSL::Vector3& scale) NSL_NOEXCEPT
		{
			_scale = scale;
			_RecalculateModelMatrix();
		}

		void AddToPosition(const NSL::Vector3& position) NSL_NOEXCEPT
		{
			_position += position;
			_modelMatrix = NSL::Translate(_modelMatrix, _position);
		}
		void AddToRotation(const NSL::Vector3& rotation) NSL_NOEXCEPT
		{
			_rotation += rotation;
			_RecalculateModelMatrix();
		}
		void AddToScale(const NSL::Vector3& scale) NSL_NOEXCEPT
		{
			_scale += scale;
			_RecalculateModelMatrix();
		}

		void MultiplyPosition(const NSL::Vector3& position) NSL_NOEXCEPT
		{
			_position *= position;
			_RecalculateModelMatrix();
		}
		void MultiplyRotation(const NSL::Vector3& rotation) NSL_NOEXCEPT
		{
			_rotation *= rotation;
			_RecalculateModelMatrix();
		}
		void MultiplyScale(const NSL::Vector3& scale) NSL_NOEXCEPT
		{
			_scale *= scale;
			_RecalculateModelMatrix();
		}

	private:
		NSL::Vector3 _position;
		NSL::Vector3 _rotation;
		NSL::Vector3 _scale;
		NSL::Matrix4x4 _modelMatrix;

		void _RecalculateModelMatrix() NSL_NOEXCEPT
		{
			_modelMatrix = NSL::Matrix4x4::Unit;
			_modelMatrix = Translate(_modelMatrix, _position);
			_modelMatrix = Rotate(_modelMatrix, NSL::DegreesToRadians(_rotation.x), NSL::Vector3::UnitX);
			_modelMatrix = Rotate(_modelMatrix, NSL::DegreesToRadians(_rotation.y), NSL::Vector3::UnitY);
			_modelMatrix = Rotate(_modelMatrix, NSL::DegreesToRadians(_rotation.z), NSL::Vector3::UnitZ);
			_modelMatrix = Scale(_modelMatrix, _scale);
		}
	};
}