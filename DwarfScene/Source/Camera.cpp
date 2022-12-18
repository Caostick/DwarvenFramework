#include <DwarfScene/Camera.h>

#include <DwarvenCore/Math/Constants.h>

df::Camera::Camera() 
	: m_ViewportWidth(1)
	, m_ViewportHeight(1)
	, m_FOV(Constants<float>::HalfPi)
	, m_ZNear(0.1f)
	, m_ZFar(1000.0f)
	, m_Position(0.0f, 0.0f, 0.0f)
	, m_Orientation(Mat3::Identity()) {

}

void df::Camera::SetViewport(float width, float height) {
	m_ViewportWidth = width;
	m_ViewportHeight = height;
}

auto df::Camera::GetViewportWidth() const -> float {
	return m_ViewportWidth;
}

auto df::Camera::GetViewportHeight() const -> float {
	return m_ViewportHeight;
}

void df::Camera::SetFOV(float fov) {
	m_FOV = fov;
}

auto df::Camera::GetFOV() const -> float {
	return m_FOV;
}

void df::Camera::SetZNear(float zNear) {
	m_ZNear = zNear;
}

auto df::Camera::GetZNear() const -> float {
	return m_ZNear;
}

void df::Camera::SetZFar(float zFar) {
	m_ZFar = zFar;
}

auto df::Camera::GetZFar() const -> float {
	return m_ZFar;
}

auto df::Camera::GetFrustum() const -> Frustum {
	return Frustum(GetMatView() * GetMatProjection());
}

void df::Camera::SetPosition(const Vec3& position) {
	m_Position = position;
}

auto df::Camera::GetPosition() const -> const Vec3& {
	return m_Position;
}

void df::Camera::SetOrientation(const Mat3& orientation) {
	m_Orientation = orientation;
}

void df::Camera::SetOrientation(const Quat& orientation) {
	m_Orientation = Mat3::FromQuaternion(orientation);
}

void df::Camera::SetOrientation(float pitch, float yaw, float roll) {
	const Mat3 rPitch = Mat3::RotationX(pitch);
	const Mat3 rYaw = Mat3::RotationY(yaw);
	const Mat3 rRoll = Mat3::RotationZ(roll);

	m_Orientation = rPitch * rYaw * rRoll;
}

auto df::Camera::GetOrientation() const -> const Mat3& {
	return m_Orientation;
}

auto df::Camera::GetRightVector() const->Vec3 {
	return Vec3(m_Orientation.M[0], m_Orientation.M[3], m_Orientation.M[6]);
}

auto df::Camera::GetUpVector() const->Vec3 {
	return Vec3(m_Orientation.M[1], m_Orientation.M[4], m_Orientation.M[7]);
}

auto df::Camera::GetForwardVector() const->Vec3 {
	return Vec3(m_Orientation.M[2], m_Orientation.M[5], m_Orientation.M[8]);
}

void df::Camera::SetTransform(const Vec3& position, const Mat3& orientation) {
	SetPosition(position);
	SetOrientation(orientation);
}

void df::Camera::SetTransform(const Vec3& position, const Quat& orientation) {
	SetPosition(position);
	SetOrientation(orientation);
}

void df::Camera::SetTransform(const Vec3& position, float pitch, float yaw, float roll) {
	SetPosition(position);
	SetOrientation(pitch, yaw, roll);
}

void df::Camera::SetTransform(const Transform& transform) {
	SetPosition(transform.GetPosition());
	SetOrientation(transform.GetOrientation());
}

auto df::Camera::GetTransform() const -> Transform {
	return Transform(m_Position, m_Orientation);
}

auto df::Camera::GetMatView() const -> Mat4 {
	return Mat4::Translation(-m_Position) * Mat4(Mat3::Transposed(m_Orientation));
}

auto df::Camera::GetMatProjection() const -> Mat4 {
	const float aspectRation = m_ViewportWidth / m_ViewportHeight;
	return Mat4::Perspective(m_FOV, aspectRation, m_ZNear, m_ZFar);
}

void df::Camera::MakeRay(float x, float y, Vec3& rayPos, Vec3& rayDir) const {
	const float screenWidth = float(m_ViewportWidth);
	const float screenHeight = float(m_ViewportHeight);
	const float invScreenWidth = 1.0f / screenWidth;
	const float invScreenHeight = 1.0f / screenHeight;
	const Mat4 invMatProjection = Mat4::Inversed(GetMatProjection());
	const Vec4 p0 = Vec4(x * invScreenWidth * 2.0f - 1.0f, y * invScreenHeight * 2.0f - 1.0f, 0.0f, 1.0f) * invMatProjection;
	const Vec4 p1 = Vec4(x * invScreenWidth * 2.0f - 1.0f, y * invScreenHeight * 2.0f - 1.0f, 1.0f, 1.0f) * invMatProjection;

	rayDir = Vec3::Normalized(Vec3(p0.X, p0.Y, p0.Z) * p0.W - Vec3(p1.X, p1.Y, p1.Z) * p1.W) * m_Orientation;
	rayPos = Vec3(p0.X, p0.Y, p0.Z) * m_Orientation + m_Position;
}

auto df::Camera::WorldToScreenSpace(const Vec3& position) const->Vec3 {
	const Vec3 viewSpacePos = (position - m_Position) * Mat3::Transposed(m_Orientation);
	const Vec4 vireSpacePos4 = Vec4(viewSpacePos, 1.0f);
	const Vec4 projectedPos = vireSpacePos4 * GetMatProjection();
	const Vec3 homogeneousPos = Vec3(projectedPos.X, projectedPos.Y, projectedPos.Z) / projectedPos.W;

	return Vec3((homogeneousPos.X * 0.5f + 0.5f) * m_ViewportWidth, (homogeneousPos.Y * 0.5f + 0.5f) * m_ViewportHeight, projectedPos.Z);
}