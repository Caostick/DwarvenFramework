#pragma once

template<typename type>
struct TPlane {
	TPlane();
	TPlane(type a, type b, type c, type d);
	TPlane(const TVec3<type>& normal, const TVec3<type>& point);
	TPlane(const TVec3<type>& n, type d);

	TVec3<type> N;
	type D;
};

template<typename type>
auto Normalized(const TPlane<type>& plane)->TPlane<type>;

template<typename type>
auto DotCoord(const TVec3<type>& point, const TPlane<type>& plane)->type;

template<typename type>
auto ProjectPointOnPlane(const TVec3<type>& point, const TPlane<type>& plane) ->TVec3<type>;

template<typename type>
auto RayIntersectPlane(const TVec3<type>& origin, const TVec3<type>& direction, const TPlane<type>& plane)->TVec3<type>;

template<typename type>
auto AxisAlignedOrientedToPoint(const TVec3<type>& origin, const TVec3<type>& axis, const TVec3<type>& point)->TPlane<type>;

#include <DwarvenCore/Math/Plane.inl>