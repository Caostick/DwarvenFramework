#pragma once

#include <DwarvenCore/Math/Math.h>

namespace df {
	class Camera {
	public:
		Camera();

		void SetViewport(float width, float height);
		auto GetViewportWidth() const -> float;
		auto GetViewportHeight() const -> float;

		void SetFOV(float fov);
		auto GetFOV() const -> float;

		void SetZNear(float zNear);
		auto GetZNear() const -> float;

		void SetZFar(float zFar);
		auto GetZFar() const -> float;

		auto GetFrustum() const -> Frustum;

		void SetPosition(const Vec3& position);
		auto GetPosition() const -> const Vec3&;

		void SetOrientation(const Mat3& orientation);
		void SetOrientation(const Quat& orientation);
		void SetOrientation(float pitch, float yaw, float roll);
		auto GetOrientation() const -> const Mat3&;
		auto GetRightVector() const->Vec3;
		auto GetUpVector() const->Vec3;
		auto GetForwardVector() const -> Vec3;
		
		void SetTransform(const Vec3& position, const Mat3& orientation);
		void SetTransform(const Vec3& position, const Quat& orientation);
		void SetTransform(const Vec3& position, float pitch, float yaw, float roll);
		void SetTransform(const Transform& transform);
		auto GetTransform() const -> Transform;

		auto GetMatView() const -> Mat4;
		auto GetMatProjection() const -> Mat4;

		void MakeRay(float x, float y, Vec3& rayPos, Vec3& rayDir) const;
		auto WorldToScreenSpace(const Vec3& position) const->Vec3;

	private:
		float m_ViewportWidth;
		float m_ViewportHeight;
		float m_FOV;
		float m_ZNear;
		float m_ZFar;

		Vec3 m_Position;
		Mat3 m_Orientation;
	};
}