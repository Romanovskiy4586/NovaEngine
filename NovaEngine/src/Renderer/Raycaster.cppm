export module Raycaster;
#include "Core.h"
import std;
import NSL;
import Camera;

export namespace Renderer
{
	class NSL_API Raycaster
	{
		NSL::Ray RaycastFromCursorPosition(int cursorPositionX, int cursorPositionY, const Camera& camera) NSL_NOEXCEPT
		{
			float normalizedX = (2.0f * cursorPositionX / camera.GetWidth()) - 1.0f;
			float normalizedY = 1.0f - (2.0f * cursorPositionY / camera.GetHeight());

			NSL::Matrix4x4 inverseProjectionView = Inverse(camera.GetProjectionMatrix() * camera.GetViewMatrix());

			NSL::Vector3 nearPoint = Unproject(normalizedX, normalizedY, 0.0f, inverseProjectionView);
			NSL::Vector3 farPoint = Unproject(normalizedX, normalizedY, 1.0f, inverseProjectionView);

			NSL::Vector3 rayOrigin = nearPoint;
			NSL::Vector3 rayDirection = Normalize(farPoint - nearPoint);

			return NSL::Ray(rayOrigin, rayDirection);
		}
	};
}