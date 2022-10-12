#pragma once

enum class EFrustumPlane {
	Front,
	Back,
	Top,
	Bottom,
	Left,
	Right,

	MAX
};

template<typename type>
class TFrustum {
public:
	void FromMatrix(const TMat4<type>& matrix);
	bool TestPoint(const TVec3<type>& origin);
	bool TestSphere(const TVec3<type>& origin, const type radius);
	bool TestAABB(const TVec3<type>& mins, const TVec3<type>& maxs);
	auto GetPlane(EFrustumPlane index) -> const TPlane<type>&;

private:
	TPlane<type> m_Planes[6];
};

template<typename type>
void CalcFrustumFarCorners(
	const TMat4<type>& matInvView, type fovInDegree, type aspect, type far,
	TVec3<type>& outLeftBottom,TVec3<type>& outRightBottom,TVec3<type>& outLeftTop,TVec3<type>& outRightTop);

#include <DwarvenCore/Math/Frustum.inl>