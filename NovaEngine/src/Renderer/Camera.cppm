export module Camera;
#include "Core.h"
import std;
import NSL;

#define SETTER(Type, SetterName, Property, AdditionalLogic)\
void Set##SetterName(Type value) NSL_NOEXCEPT { Property = value; AdditionalLogic }

export namespace Renderer
{
	class NSL_API Camera final
	{
	public:
		enum class NSL_API ProjectionType : int { Perspective, Orthographic };

	public:
		Camera(int width = 1, int height = 1, ProjectionType projectionType = ProjectionType::Perspective) NSL_NOEXCEPT
			: _width(width)
			, _height(height)
			, _position(0.0f, 0.0f, 0.0f)
			, _frontVector(-NSL::Vector3::UnitZ)
			, _upVector(NSL::Vector3::UnitY)
			, _rightVector(NSL::Vector3::UnitX)
			, _viewMatrix(NSL::Matrix4x4::Unit)
			, _pitch(0.0f)
			, _yaw(-90.0f)
			, _mouseSensitivity(0.1f)
			, _projectionType(projectionType)
			, _perspectiveMatrix(NSL::Matrix4x4::Unit)
			, _orthographicMatrix(NSL::Matrix4x4::Unit)
			, _projectionViewMatrix(NSL::Matrix4x4::Unit)
			, _inverseProjectionViewMatrix(NSL::Matrix4x4::Unit)
			, _fov(60.0f)
			, _depthNear(0.001f)
			, _depthFar(1000.0f)
			, _aspectRatio(1.0f)
			, _fovClampingValue(20.0f, 120.0f)
		{
			_UpdateFrontVector();
			_UpdateAspectRatio();
			_UpdateViewMatrix();
			_UpdatePerspectiveMatrix();
			_UpdateOrthographicMatrix();
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}
		Camera(ProjectionType projectionType) NSL_NOEXCEPT
			: Camera(1, 1, projectionType)
		{
		}

		int GetWidth() const { return _width; }
		int GetHeight() const { return _height; }
		const NSL::Vector3& GetPosition() const { return _position; }
		const NSL::Vector3& GetFrontVector() const { return _frontVector; }
		const NSL::Vector3& GetUpVector() const { return _upVector; }
		const NSL::Vector3& GetRightVector() const { return _rightVector; }
		const NSL::Matrix4x4& GetViewMatrix() const { return _viewMatrix; }
		float GetPitch() const { return _pitch; }
		float GetYaw() const { return _yaw; }
		float GetMouseSensitivity() const { return _mouseSensitivity; }
		ProjectionType GetProjectionType() const { return _projectionType; }
		const NSL::Matrix4x4& GetProjectionMatrix() const { return _projectionType == ProjectionType::Perspective ? _perspectiveMatrix : _orthographicMatrix; }
		const NSL::Matrix4x4& GetProjectionViewMatrix() const { return _projectionViewMatrix; }
		const NSL::Matrix4x4& GetInverseProjectionViewMatrix() const { return _inverseProjectionViewMatrix; }
		float GetFOV() const { return _fov; }
		const NSL::Vector2& GetFOVClampingValue() const { return _fovClampingValue; }
		float GetDepthNear() const { return _depthNear; }
		float GetDepthFar() const { return _depthFar; }

		void SetWidth(int value)
		{
			_width = value;
			_UpdateAspectRatio();
			_projectionType == ProjectionType::Perspective ? _UpdatePerspectiveMatrix() : _UpdateOrthographicMatrix();
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}
		void SetHeight(int value)
		{
			_height = value;
			_UpdateAspectRatio();
			_projectionType == ProjectionType::Perspective ? _UpdatePerspectiveMatrix() : _UpdateOrthographicMatrix(); 
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}
		void SetPosition(const NSL::Vector3& value)
		{
			_position = value;
			_UpdateViewMatrix();
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}
		void SetMouseSensitivity(float value)
		{
			_mouseSensitivity = value;
		}
		void SetProjectionType(ProjectionType value)
		{
			_projectionType = value; 
			_projectionType == ProjectionType::Perspective ? _UpdatePerspectiveMatrix() : _UpdateOrthographicMatrix();
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}
		void SetFOV(float value)
		{
			_fov = value; 
			_ClampFOV();
			_projectionType == ProjectionType::Perspective ? _UpdatePerspectiveMatrix() : _UpdateOrthographicMatrix();
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}
		void SetFOVClampingValue(const NSL::Vector2& value)
		{
			_fovClampingValue = value; 
			if (_fovClampingValue.x > _fovClampingValue.y) 
			{
				LogWarning("FOV clamping value setting warning: min value bigger than max value");
			}
			_ClampFOV();
			_projectionType == ProjectionType::Perspective ? _UpdatePerspectiveMatrix() : _UpdateOrthographicMatrix();
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}
		void SetDepthNear(float value)
		{
			_depthNear = value;
			_projectionType == ProjectionType::Perspective ? _UpdatePerspectiveMatrix() : _UpdateOrthographicMatrix();
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}
		void SetDepthFar(float value)
		{
			_depthFar = value;
			_projectionType == ProjectionType::Perspective ? _UpdatePerspectiveMatrix() : _UpdateOrthographicMatrix();
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}

		void SetWidthAndHeight(int width, int height) NSL_NOEXCEPT
		{
			_width = width;
			_height = height;
			_UpdateAspectRatio();
			_projectionType == ProjectionType::Perspective ? _UpdatePerspectiveMatrix() : _UpdateOrthographicMatrix();
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}
		void AddToFOV(float valueInDegrees) NSL_NOEXCEPT
		{
			_fov += valueInDegrees;
			_ClampFOV();
			_projectionType == ProjectionType::Perspective ? _UpdatePerspectiveMatrix() : _UpdateOrthographicMatrix();
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}
		void AddToPosition(const NSL::Vector3& value) NSL_NOEXCEPT
		{
			_position += value;
			_UpdateViewMatrix();
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}
		void InputMouseDelta(float x, float y) NSL_NOEXCEPT
		{
			_yaw -= x * _mouseSensitivity;
			_pitch += y * _mouseSensitivity;

			_ClampYawIn360();
			_ClampPitch();

			_UpdateFrontVector();
			_UpdateViewMatrix();
			_UpdateProjectionViewMatrixAndSameInverseMatrix();
		}

	private:
		void _UpdateOrthographicMatrix() NSL_NOEXCEPT
		{
			float fovInRadians = NSL::DegreesToRadians(_fov);
			_orthographicMatrix = NSL::CreateOrthographic(-fovInRadians * _aspectRatio, fovInRadians * _aspectRatio, fovInRadians, -fovInRadians, _depthNear, _depthFar);
		}
		void _UpdatePerspectiveMatrix() NSL_NOEXCEPT
		{
			_perspectiveMatrix = NSL::CreatePerspective(NSL::DegreesToRadians(_fov), _aspectRatio, _depthNear, _depthFar);
		}
		void _UpdateAspectRatio() NSL_NOEXCEPT
		{
			_aspectRatio = (float)_width / _height;
		}
		void _ClampFOV() NSL_NOEXCEPT
		{
			_fov = NSL::Clamp(_fov, _fovClampingValue.x, _fovClampingValue.y);
		}
		void _ClampPitch() NSL_NOEXCEPT
		{
			_pitch = NSL::Clamp(_pitch, -89.0f, 89.0f);
		}
		void _ClampYawIn360() NSL_NOEXCEPT
		{
			if (_yaw > 360.0f || _yaw < -360.0f)
			{
				_yaw = 0.0f;
			}
		}

		void _NormalizeDirectionVectors() NSL_NOEXCEPT
		{
			_frontVector = NSL::Normalize(_frontVector);
			_rightVector = NSL::Normalize(NSL::Cross(_frontVector, NSL::Vector3::UnitY));
			_upVector = NSL::Normalize(NSL::Cross(_rightVector, _frontVector));
		}
		void _UpdateViewMatrix() NSL_NOEXCEPT
		{
			_viewMatrix = NSL::CreateView(_position, _frontVector + _position, _upVector);
		}
		void _UpdateFrontVector() NSL_NOEXCEPT
		{
			float pitchInRadians = NSL::DegreesToRadians(_pitch);
			float yawInRadians = NSL::DegreesToRadians(_yaw);

			_frontVector.x = cos(pitchInRadians) * cos(yawInRadians);
			_frontVector.y = sin(pitchInRadians);
			_frontVector.z = cos(pitchInRadians) * sin(yawInRadians);

			_NormalizeDirectionVectors();
		}
		void _UpdateProjectionViewMatrixAndSameInverseMatrix() NSL_NOEXCEPT
		{
			_projectionViewMatrix = _projectionType == ProjectionType::Perspective ? _perspectiveMatrix * _viewMatrix : _orthographicMatrix * _viewMatrix;
			_inverseProjectionViewMatrix = NSL::Inverse(_projectionViewMatrix);
		}

	private:
		int _width;
		int _height;
		NSL::Vector3 _position;
		NSL::Vector3 _frontVector;
		NSL::Vector3 _upVector;
		NSL::Vector3 _rightVector;
		NSL::Matrix4x4 _viewMatrix;
		float _pitch;
		float _yaw;
		float _mouseSensitivity;

		ProjectionType _projectionType;
		NSL::Matrix4x4 _perspectiveMatrix;
		NSL::Matrix4x4 _orthographicMatrix;

		NSL::Matrix4x4 _projectionViewMatrix;
		NSL::Matrix4x4 _inverseProjectionViewMatrix;

		float _fov;
		float _depthNear;
		float _depthFar;
		float _aspectRatio;

		NSL::Vector2 _fovClampingValue;
	};
}