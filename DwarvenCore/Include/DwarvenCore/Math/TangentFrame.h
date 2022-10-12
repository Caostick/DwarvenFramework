#pragma once

#include <DwarvenCore/Math/Math.h>

template<typename type>
auto ComputeQTangentFromTangentFrame(const TMat3<type>& tangentFrame)->TVec4<type>;

template<typename type>
auto ComputeQTangentFromTangentFrame(TVec3<type> tangent, TVec3<type> binormal, const TVec3<type>& normal) -> TVec4<type>;

#include <DwarvenCore/Math/TangentFrame.inl>