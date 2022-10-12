#pragma once

#include <math.h>
#include <stdint.h>
#include <random>

template<typename type>
type DegToRad(const type& value) {
    return value / type(180) * Constants<type>::Pi;
}

template<typename type>
type RadToDeg(const type& value) {
    return value * Constants<type>::InvPi * type(180);
}

template<typename type>
type Steer(type angleCur, type angleDst, type angleVelocity, type angleMin, type angleMax) {
	const type fullAngle = angleMax - angleMin;
    const type angleBelow = (angleCur > angleDst) ? angleDst : (angleDst - fullAngle);
    const type angleAbove = (angleCur > angleDst) ? (angleDst + fullAngle) : angleDst;
    const type difBelow = Abs<type>(angleBelow - angleCur);
    const type difAbove = Abs<type>(angleAbove - angleCur);
    const type stepSign = (difBelow < difAbove) ? type(-1) : type(1);
    const type angleStep = (difBelow < difAbove)
        ? Min(difBelow, Abs<type>(angleVelocity))
        : Min(difAbove, Abs<type>(angleVelocity));

	return Fract<type>((angleCur + angleStep * stepSign - angleMin) / fullAngle) * fullAngle + angleMin;
}

template<typename type>
type Fract(const type& value) {
    const type result = value - int(value);
    return result >= type(0) ? result : (result + type(1));
}

template<typename type>
auto Fract(const TVec3<type>& value)->TVec3<type> {
    return TVec3<type>(Fract(value.X), Fract(value.Y), Fract(value.Z));
}

template<typename type>
type Floor(const type& value) {
    return value - Fract(value);
}

template<typename type>
type ScaleFloor(const type& value, const type& scale) {
    return Floor(value / scale + type(0.0001)) * scale;
}

template <typename type>
type Random(const type& min, const type& max) {
    return type(((rand() % RAND_MAX) / double(RAND_MAX))) * (max - min) + min;
}

template <typename type>
type Max(const type& a, const type& b) {
    return (a >= b) ? a : b;
}

template <typename type>
auto Max(const TVec2<type>& a, const TVec2<type>& b) -> TVec2<type> {
	return TVec2(
		Max(a.X, b.X),
		Max(a.Y, b.Y)
	);
}

template <typename type>
auto Max(const TVec3<type>& a, const TVec3<type>& b) -> TVec3<type> {
	return TVec3(
		Max(a.X, b.X),
		Max(a.Y, b.Y),
		Max(a.Z, b.Z)
	);
}

template <typename type>
type Min(const type& a, const type& b) {
    return (a < b) ? a : b;
}

template <typename type>
auto Min(const TVec2<type>& a, const TVec2<type>& b) -> TVec2<type> {
	return TVec2(
		Min(a.X, b.X),
		Min(a.Y, b.Y)
	);
}

template <typename type>
auto Min(const TVec3<type>& a, const TVec3<type>& b) -> TVec3<type> {
	return TVec3(
		Min(a.X, b.X),
		Min(a.Y, b.Y),
		Min(a.Z, b.Z)
	);
}

template <typename type>
type Clamp(const type& value, const type& min, const type& max) {
    return Max<type>(min, Min<type>(value, max));
}

template <typename type>
auto Clamp(const TVec2<type>& value, const TVec2<type>& min, const TVec2<type>& max)->TVec2<type> {
	return TVec2(
		Clamp(value.X, min.X, max.X),
        Clamp(value.Y, min.Y, max.Y)
	);
}

template <typename type>
auto Clamp(const TVec3<type>& value, const TVec3<type>& min, const TVec3<type>& max)->TVec3<type> {
	return TVec3(
		Clamp(value.X, min.X, max.X),
		Clamp(value.Y, min.Y, max.Y),
        Clamp(value.Z, min.Z, max.Z)
	);
}

template <typename type>
type Abs(const type& value) {
    return value < type(0) ? -value : value;
}

template <typename type>
auto Abs(const TVec2<type>& value)->TVec2<type> {
    return TVec2<type>(
        Abs(value.X),
        Abs(value.Y)
    );
}

template <typename type>
auto Abs(const TVec3<type>& value)->TVec3<type> {
	return TVec3<type>(
		Abs(value.X),
		Abs(value.Y),
        Abs(value.Z)
    );
}

template <typename type>
void SinCos(const type& angle, type& sin, type& cos) {
    sin = sin(angle);
    cos = cos(angle);
}

template <typename type>
void Swap(type& a, type& b) {
    type tmp = a;
    a = b;
    b = tmp;
}

template <typename type>
const type Mod(const type& a, const type& b) {
    type result = a % b;
    return result + b * type(result < type(0));
}

template <typename type, typename ltype>
const type Lerp(const type& a, const type& b, const ltype& lerp) {
    return (b - a) * lerp + a;
}

template <typename type, typename ltype>
const type Mix(const type& a, const type& b, const ltype& lerp) {
    return (b - a) * lerp + a;
}

template <typename type, typename  ltype>
const type Bezier(const type& p0, const type& p1, const type& p2, const ltype& lerp) {
	return Mix<type, ltype>(Mix<type, ltype>(p0, p1, lerp), Mix<type, ltype>(p1, p2, lerp), lerp);
}

template <typename type>
bool HitTestAABBsCenterExt(const TVec3<type>& centerA, const TVec3<type>& extA, const TVec3<type>& centerB, const TVec3<type>& extB) {
    const bool xHits = Abs(centerA.X - centerB.X) < ((extA.X + extB.X) * 0.5f);
    const bool yHits = Abs(centerA.X - centerB.X) < ((extA.X + extB.X) * 0.5f);
    const bool zHits = Abs(centerA.X - centerB.X) < ((extA.X + extB.X) * 0.5f);

    return xHits && yHits && zHits;
}

template <typename type>
bool HitTestAABBsMinMax(const TVec3<type>& minA, const TVec3<type>& maxA, const TVec3<type>& minB, const TVec3<type>& maxB) {
    const TVec3<type> centerA = (maxA + minA) * 0.5f;
    const TVec3<type> centerB = (maxB + minB) * 0.5f;
	const TVec3<type> extA = maxA - minA;
	const TVec3<type> extB = maxB - minA;

    return HitTestAABBsCenterExt(centerA, extA, centerB, extB);
}

template <typename type>
const TVec3<type> TraceToRay(const TVec3<type>& rayPos,
                             const TVec3<type>& rayDir,
                             const TVec3<type>& tracePos,
                             const TVec3<type>& traceDir) {
    const TVec3<type> tangent = TVec3<type>::Normalized(TVec3<type>::Cross(tracePos - rayPos, rayDir));
    const TVec3<type> norm = TVec3<type>::Normalized(TVec3<type>::Cross(rayDir, tangent));
    TPlane<type> plane = TPlane<type>(norm, rayPos);
    const TVec3<type> proj = RayIntersectPlane(tracePos, traceDir, plane);
    return TVec3<type>(rayPos + rayDir * TVec3<type>::Dot(rayDir, proj - rayPos));
}

template <typename type>
const TVec3<type> TraceToLine(const TVec3<type>& p0,
                              const TVec3<type>& p1,
                              const TVec3<type>& tracePos,
                              const TVec3<type>& traceDir) {
    const TVec3<type> rayPos = p0;
    const TVec3<type> rayDir = TVec3<type>::Normalized(p1 - p0);
    const TVec3<type> tangent = TVec3<type>::Normalized(TVec3<type>::Cross(tracePos - rayPos, rayDir));
    const TVec3<type> norm = TVec3<type>::Normalized(TVec3<type>::Cross(rayDir, tangent));
    TPlane<type> plane = TPlane<type>(norm, rayPos);
    const TVec3<type> proj = RayIntersectPlane(tracePos, traceDir, plane);
    return TVec3<type>(rayPos + rayDir * TVec3<type>::Dot(rayDir, proj - rayPos));
}