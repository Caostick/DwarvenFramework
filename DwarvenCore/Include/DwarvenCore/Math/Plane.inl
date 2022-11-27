#pragma once

template<typename type>
TPlane<type>::TPlane() 
	: N(type(0), type(1), type(0))
	, D(type(0)) {}

template<typename type>
TPlane<type>::TPlane(type a, type b, type c, type d) 
	: N(a, b, c)
	, D(d) {}

template<typename type>
TPlane<type>::TPlane(const TVec3<type>& normal, const TVec3<type>& point) {
	N = normal;
	D = -TVec3<type>::Dot(normal, point);
}

template<typename type>
TPlane<type>::TPlane(const TVec3<type>& n, type d) 
	: N(n)
	, D(d) {
}

template<typename type>
auto Normalized(const TPlane<type>& plane)->TPlane<type> {
	const type invNorm = 1.0f / plane.N.Length();

	return TPlane<type>(plane.N * invNorm, plane.D * invNorm);
}

template<typename type>
auto DotCoord(const TVec3<type>& point, const TPlane<type>& plane)->type {
	return plane.N.X * point.X + plane.N.Y * point.Y + plane.N.Z * point.Z + plane.D;
}

template<typename type>
auto ProjectPointOnPlane(const TVec3<type>& point, const TPlane<type>& plane)->TVec3<type> {
	const TVec3<type> origin = plane.N * plane.D;
	const TVec3<type> diff = point - origin;
	const type distance = TVec3<type>::Dot(diff, plane.N);
	return point - plane.N * distance;
}

template<typename type>
auto RayIntersectPlane(const TVec3<type>& origin, const TVec3<type>& direction, const TPlane<type>& plane)->TVec3<type> {
	const type t = -(TVec3<type>::Dot(origin, plane.N) + plane.D) / TVec3<type>::Dot(-direction, plane.N);
	return origin - t * direction;
}

template<typename type>
auto AxisAlignedOrientedToPoint(const TVec3<type>& origin, const TVec3<type>& axis, const TVec3<type>& point) -> TPlane<type> {
	return TPlane<type>(TVec3<type>::Normalized(point - origin - axis * TVec3<type>::Dot(axis, point - origin)), origin);
}
