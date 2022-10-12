#pragma once

/*
* Intersection of two ray and AABB
* Returns true, if intersection exists, false otherwise
* In case of success @tMin, @tMax contain near and far TOI(time of intersection)
*/
template <typename type>
bool RayAABBTest(
	const TVec3<type>& rayOrigin,
	const TVec3<type>& rayDir,
	const TVec3<type>& aabbMin,
	const TVec3<type>& aabbMax,
	float& tMin, float& tMax);

#include <DwarvenCore/Math/Intersection3d.inl>