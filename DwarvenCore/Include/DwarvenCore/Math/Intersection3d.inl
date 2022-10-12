#pragma once

template <typename type>
bool RayAABBTest(
	const TVec3<type>& rayOrigin,
	const TVec3<type>& rayDir,
	const TVec3<type>& aabbMin,
	const TVec3<type>& aabbMax,
	float& tMin, float& tMax) {
	const TVec3<type> invRayDir = TVec3<type>(1.0f) / rayDir;

	const TVec3<type> t1 = (aabbMin - rayOrigin) * invRayDir;
	const TVec3<type> t2 = (aabbMax - rayOrigin) * invRayDir;

	const TVec3<type> tmin = Min(t1, t2);
	const TVec3<type> tmax = Max(t1, t2);

	tMin = Max(tMin, Max(Max(type(0), tmin.X), Max(tmin.Y, tmin.Z)));
	tMax = Min(tMax, Min(Min(type(99999), tmax.X), Min(tmax.Y, tmax.Z)));

	return tMax > tMin;
}