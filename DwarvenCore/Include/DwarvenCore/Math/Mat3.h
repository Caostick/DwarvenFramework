#pragma once

template<typename type>
struct TMat3 {
	TMat3();
	TMat3(type m0, type m1, type m2, type m3, type m4, type m5, type m6, type m7, type m8);

	template<typename type2>
	TMat3(const TMat3<type2>& a_Matrix);

	void ToEulerAngles(type& pitch, type& yaw, type& roll) const;

	static auto Empty() -> TMat3<type>;
	static auto Identity() -> TMat3<type>;
	static auto Transposed(const TMat3<type>& mat) -> TMat3<type>;
	static auto RotationX(type angle) -> TMat3<type>;
	static auto RotationY(type angle) -> TMat3<type>;
	static auto RotationZ(type angle) -> TMat3<type>;
	static auto Rotation(type angle, const TVec3<type>& axis) -> TMat3<type>;
	static auto FromEulerAngles(type pitch, type yaw, type roll) -> TMat3<type>;
	static auto Scaling(const TVec3<type>& scale) -> TMat3<type>;
	static auto Scaling(type scale)->TMat3<type>;
	static auto Scaling(type sx, type sy, type sz) -> TMat3<type>;
	static auto FromQuaternion(const TQuat<type>& quat) -> TMat3<type>;
	static auto LookAt(const TVec3<type>& origin, const TVec3<type>& target, const TVec3<type>& up) -> TMat3<type>;
	static auto RightVector(const TMat3<type>& mat) -> TVec3<type>;
	static auto UpVector(const TMat3<type>& mat) -> TVec3<type>;
	static auto ForwardVector(const TMat3<type>& mat) -> TVec3<type>;

	type M[9];
};

template<typename type>
auto operator + (const TMat3<type>& left, const TMat3<type>& right)->TMat3<type>;

template<typename type>
auto operator - (const TMat3<type>& left, const TMat3<type>& right)->TMat3<type>;

template<typename type>
auto operator * (const TMat3<type>& left, const TMat3<type>& right)->TMat3<type>;

template<typename type>
auto operator * (const TMat3<type>& left, type right)->TMat3<type>;

template<typename type>
auto operator * (type left, const TMat3<type>& right)->TMat3<type>;

template<typename type>
auto operator * (const TVec3<type>& vec, const TMat3<type>& mat)->TVec3<type>;

#include <DwarvenCore/Math/Mat3.inl>