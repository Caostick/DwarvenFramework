#pragma once

template<typename type>
struct TMat4 {
	TMat4();
	TMat4(type m11, type m12, type m13, type m14,
		type m21, type m22, type m23, type m24,
		type m31, type m32, type m33, type m34,
		type m41, type m42, type m43, type m44);

	template<typename type2>
	TMat4(const TMat4<type2>& matrix);

	template<typename type2>
	TMat4(const TMat3<type2>& matrix);

	template<typename type2>
	TMat4(type2 m11, type2 m12, type2 m13, type2 m14,
		type2 m21, type2 m22, type2 m23, type2 m24,
		type2 m31, type2 m32, type2 m33, type2 m34,
		type2 m41, type2 m42, type2 m43, type2 m44);

	static auto Empty() -> TMat4<type>;
	static auto Identity() -> TMat4<type>;
	static auto Transposed(const TMat4<type>& mat) -> TMat4<type>;
	static auto InverseTransform(const TMat4<type>& mat) -> TMat4<type>;
	static auto Translation(const TVec3<type>& position) -> TMat4<type>;
	static auto Translation(type x, type y, type z) -> TMat4<type>;
	static auto RotationX(const type angle) -> TMat4<type>;
	static auto RotationY(const type angle) -> TMat4<type>;
	static auto RotationZ(const type angle) -> TMat4<type>;
	static auto Rotation(const type angle, const TVec3<type>& axis) -> TMat4<type>;
	static auto Scaling(const TVec3<type>& scale) -> TMat4<type>;
	static auto Scaling(type x, type y, type z) -> TMat4<type>;
	static auto Scaling(type scale) -> TMat4<type>;
	static auto LookAt(const TVec3<type>& eye, const TVec3<type>& at, const TVec3<type>& up) -> TMat4<type>;
	static auto Ortho(type width, type height, type zNear, type zFar) -> TMat4<type>;
	static auto Ortho(type xMin, type xMax, type yMin, type yMax, type zMin, type zMax) -> TMat4<type>;
	static auto Perspective(type fov, type aspect, type zNear, type zFar) -> TMat4<type>;
	static auto Perspective(type left, type right, type bottom, type top, type zNear, type zFar) -> TMat4<type>;
	static auto Pick(type x, type y, type width, type height) -> TMat4<type>;
	static auto Inversed(const TMat4<type>& mat) -> TMat4<type>;

	type M[16];
};

template<typename type>
auto operator + (const TMat4<type>& left, const TMat4<type>& right)->TMat4<type>;

template<typename type>
auto operator - (const TMat4<type>& left, const TMat4<type>& right)->TMat4<type>;

template<typename type>
auto operator * (const TMat4<type>& left, const TMat4<type>& right)->TMat4<type>;

template<typename type>
auto operator * (const TMat3<type>& left, const TMat4<type>& right)->TMat4<type>;

template<typename type>
auto operator * (const TMat4<type>& left, const TMat3<type>& right)->TMat4<type>;

template<typename type>
auto operator * (const TMat4<type>& left, type right)->TMat4<type>;

template<typename type>
auto operator * (type left, const TMat4<type>& right)->TMat4<type>;

template<typename type>
auto operator * (const TVec3<type>& vec, const TMat4<type>& mat)->TVec3<type>;

template<typename type>
auto operator * (const TVec4<type>& vec, const TMat4<type>& mat)->TVec4<type>;

#include <DwarvenCore/Math/Mat4.inl>