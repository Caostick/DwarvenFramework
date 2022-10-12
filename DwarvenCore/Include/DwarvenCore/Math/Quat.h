#pragma once

template<typename type>
struct TQuat {
	TQuat();
	TQuat(type x, type y, type z, type w);

	template<typename type2>
	TQuat(const TQuat<type2>& other);

	type X;
	type Y;
	type Z;
	type W;
};

template<typename type>
void QuaternionToEulerAngles(const TQuat<type>& quat, type& pitch, type& yaw, type& roll);

template<typename type>
auto QuaternionFromEulerAngles(type pitch, type yaw, type roll)->TQuat<type>;

template<typename type>
auto QuaternionFromAxisRotation(TQuat<type>* quat, const TVec3<type>& vec, type angle)->TQuat<type>;

template<typename type>
auto QuaternionFromMatrix(const TMat3<type>& mat)->TQuat<type>;

template<typename type>
auto SLerp(const TQuat<type>& quatA, const TQuat<type>& quatB, type factor)->TQuat<type>;

template<typename type>
auto Normalized(const TQuat<type>& quat)->TQuat<type>;

template<typename type>
TQuat<type> operator * (const TQuat<type>& left, type right);

template<typename type>
const TQuat<type>& operator *= (TQuat<type>& left, type right);

#include <DwarvenCore/Math/Quat.inl>