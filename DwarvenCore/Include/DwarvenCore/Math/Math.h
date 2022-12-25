#pragma once

template<typename type> struct TVec2;
template<typename type> struct TVec3;
template<typename type> struct TVec4;
template<typename type> struct TQuat;
template<typename type> struct TMat3;
template<typename type> struct TMat4;
template<typename type> struct TPlane;
template<typename type> struct TTransform;
template<typename type> class TFrustum;

#include <DwarvenCore/Types.h>

#include <DwarvenCore/Math/Vec2.h>
#include <DwarvenCore/Math/Vec3.h>
#include <DwarvenCore/Math/Vec4.h>
#include <DwarvenCore/Math/Mat3.h>
#include <DwarvenCore/Math/Mat4.h>
#include <DwarvenCore/Math/Quat.h>
#include <DwarvenCore/Math/Plane.h>
#include <DwarvenCore/Math/Frustum.h>
#include <DwarvenCore/Math/Intersection2d.h>
#include <DwarvenCore/Math/Intersection3d.h>
#include <DwarvenCore/Math/Transform.h>
#include <DwarvenCore/Math/TangentFrame.h>
#include <DwarvenCore/Math/Constants.h>

template<typename type>
type DegToRad(type value);

template<typename type>
type RadToDeg(type value);

template<typename type>
type Steer(type angleCur, type angleDst, type angleVelocity, type angleMin, type angleMax);

template<typename type>
type Fract(type value);

template<typename type>
auto Fract(const TVec3<type>& value) -> TVec3<type>;

template<typename type>
type Floor(type value);

template<typename type>
type ScaleFloor(type value, type scale);

template <typename type>
type Random(type min, type max);

template <typename type>
type Max(type a, type b);

template <typename type>
auto Max(const TVec2<type>& a, const TVec2<type>& b)->TVec2<type>;

template <typename type>
auto Max(const TVec3<type>& a, const TVec3<type>& b)->TVec3<type>;

template <typename type>
type Min(type a, type b);

template <typename type>
auto Min(const TVec2<type>& a, const TVec2<type>& b)->TVec2<type>;

template <typename type>
auto Min(const TVec3<type>& a, const TVec3<type>& b) -> TVec3<type>;

template <typename type>
type Clamp(type value, type min, type max);

template <typename type>
auto Clamp(const TVec2<type>& value, const TVec2<type>& min, const TVec2<type>& max) -> TVec2<type>;

template <typename type>
auto Clamp(const TVec3<type>& value, const TVec3<type>& min, const TVec3<type>& max)->TVec3<type>;

template <typename type>
type Abs(type value);

template <typename type>
auto Abs(const TVec2<type>& value) -> TVec2<type>;

template <typename type>
auto Abs(const TVec3<type>& value)->TVec3<type>;

template <typename type>
auto Sin(type value) -> type;

template <typename type>
auto Cos(type value)->type;

template <typename type>
auto Tan(type value)->type;

template <typename type>
auto Asin(type value)->type;

template <typename type>
auto Acos(type value)->type;

template <typename type>
auto Atan(type value)->type;

template <typename type>
auto Atan2(type y, type x)->type;

template <typename type>
auto Sqrt(type value)->type;

template <typename type>
void Swap(type& a, type& b);

template <typename type>
auto Mod(type a, type b) -> type;

template <typename type, typename ltype>
auto Lerp(type a, type b, const ltype& lerp) -> type;

template <typename type, typename ltype>
auto Mix(type a, type b, const ltype& lerp) -> type;

template<typename T>
auto CalculateBezierPoint(const TVec2<T>& p0, const TVec2<T>& p1, const TVec2<T>& p2, T t) -> T;

template<typename T>
auto CalculateBezierPoint(const TVec3<T>& p0, const TVec3<T>& p1, const TVec3<T>& p2, T t) -> T;

template<typename T>
auto CalculateBezierPoint(const TVec2<T>& p0, const TVec2<T>& p1, const TVec2<T>& p2, const TVec2<T>& p3, T t) -> T;

template<typename T>
auto CalculateBezierPoint(const TVec3<T>& p0, const TVec3<T>& p1, const TVec3<T>& p2, const TVec3<T>& p3, T t) -> T;

template <typename type>
bool HitTestAABBsCenterExt(const TVec3<type>& centerA, const TVec3<type>& extA, const TVec3<type>& centerB, const TVec3<type>& extB);

template <typename type>
bool HitTestAABBsMinMax(const TVec3<type>& minA, const TVec3<type>& maxA, const TVec3<type>& minB, const TVec3<type>& maxB);

template <typename type>
const TVec3<type> TraceToRay(
    const TVec3<type>& rayPos,
    const TVec3<type>& rayDir,
    const TVec3<type>& tracePos,
    const TVec3<type>& traceDir);

template <typename type>
const TVec3<type> TraceToLine(
    const TVec3<type>& p0,
    const TVec3<type>& p1,
    const TVec3<type>& tracePos,
    const TVec3<type>& traceDir);

using Vec2 = TVec2<float>;
using Vec3 = TVec3<float>;
using Vec4 = TVec4<float>;
using Mat3 = TMat3<float>;
using Mat4 = TMat4<float>;
using Quat = TQuat<float>;
using Plane = TPlane<float>;
using Frustum = TFrustum<float>;
using Transform = TTransform<float>;

using DVec2 = TVec2<double>;
using DVec3 = TVec3<double>;
using DVec4 = TVec4<double>;
using DMat3 = TMat3<double>;
using DMat4 = TMat4<double>;
using DQuat = TQuat<double>;
using DPlane = TPlane<double>;
using DFrustum = TFrustum<double>;
using DTransform = TTransform<double>;

using IVec2 = TVec2<int32>;
using IVec3 = TVec3<int32>;
using IVec4 = TVec4<int32> ;

using UIVec2 = TVec2<uint32>;
using UIVec3 = TVec3<uint32>;
using UIVec4 = TVec4<uint32>;

#include <DwarvenCore/Math/Math.inl>