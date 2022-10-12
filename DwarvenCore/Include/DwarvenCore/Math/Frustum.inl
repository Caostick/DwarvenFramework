#pragma once

template<typename type>
void TFrustum<type>::FromMatrix(const TMat4<type>& matrix) {
	m_Planes[int(EFrustumPlane::Front)].A = matrix.M[12] - matrix.M[8];
	m_Planes[int(EFrustumPlane::Front)].B = matrix.M[13] - matrix.M[9];
	m_Planes[int(EFrustumPlane::Front)].C = matrix.M[14] - matrix.M[10];
	m_Planes[int(EFrustumPlane::Front)].D = matrix.M[15] - matrix.M[11];
	//m_Planes[int(EFrustumPlane::Front)].Normalize();

	m_Planes[int(EFrustumPlane::Back)].A = matrix.M[12] + matrix.M[8];
	m_Planes[int(EFrustumPlane::Back)].B = matrix.M[13] + matrix.M[9];
	m_Planes[int(EFrustumPlane::Back)].C = matrix.M[14] + matrix.M[10];
	m_Planes[int(EFrustumPlane::Back)].D = matrix.M[15] + matrix.M[11];
	//m_Planes[int(EFrustumPlane::Back)].Normalize();

	m_Planes[int(EFrustumPlane::Left)].A = matrix.M[12] + matrix.M[0];
	m_Planes[int(EFrustumPlane::Left)].B = matrix.M[13] + matrix.M[1];
	m_Planes[int(EFrustumPlane::Left)].C = matrix.M[14] + matrix.M[2];
	m_Planes[int(EFrustumPlane::Left)].D = matrix.M[15] + matrix.M[3];
	//m_Planes[int(EFrustumPlane::Left)].Normalize();

	m_Planes[int(EFrustumPlane::Right)].A = matrix.M[12] - matrix.M[0];
	m_Planes[int(EFrustumPlane::Right)].B = matrix.M[13] - matrix.M[1];
	m_Planes[int(EFrustumPlane::Right)].C = matrix.M[14] - matrix.M[2];
	m_Planes[int(EFrustumPlane::Right)].D = matrix.M[15] - matrix.M[3];
	//m_Planes[int(EFrustumPlane::Right)].Normalize();

	m_Planes[int(EFrustumPlane::Top)].A = matrix.M[12] - matrix.M[4];
	m_Planes[int(EFrustumPlane::Top)].B = matrix.M[13] - matrix.M[5];
	m_Planes[int(EFrustumPlane::Top)].C = matrix.M[14] - matrix.M[6];
	m_Planes[int(EFrustumPlane::Top)].D = matrix.M[15] - matrix.M[7];
	//m_Planes[int(EFrustumPlane::Top)].Normalize();

	m_Planes[int(EFrustumPlane::Bottom)].A = matrix.M[12] + matrix.M[4];
	m_Planes[int(EFrustumPlane::Bottom)].B = matrix.M[13] + matrix.M[5];
	m_Planes[int(EFrustumPlane::Bottom)].C = matrix.M[14] + matrix.M[6];
	m_Planes[int(EFrustumPlane::Bottom)].D = matrix.M[15] + matrix.M[7];
	//m_Planes[int(EFrustumPlane::Bottom)].Normalize();
}

template<typename type>
bool TFrustum<type>::TestPoint(const TVec3<type>& origin) {
	for (int i = 0; i < 6; i++) {
		if (m_Planes[i].DotCoord(origin) < 0.0f) {
			return false;
		}
	}
	return true;
}

template<typename type>
bool TFrustum<type>::TestSphere(const TVec3<type>& origin, const type radius) {
	for (int i = 0; i < 6; i++) {
		if (m_Planes[i].DotCoord(origin) < -radius) {
			return false;
		}
	}
	return true;
}

template<typename type>
bool TFrustum<type>::TestAABB(const TVec3<type>& mins, const TVec3<type>& maxs) {
	const TVec3<type> corners[8] =
	{
		TVec3<type>(mins.X, mins.Y, mins.Z),
		TVec3<type>(mins.X, mins.Y, maxs.Z),
		TVec3<type>(mins.X, maxs.Y, mins.Z),
		TVec3<type>(mins.X, maxs.Y, maxs.Z),
		TVec3<type>(maxs.X, mins.Y, mins.Z),
		TVec3<type>(maxs.X, mins.Y, maxs.Z),
		TVec3<type>(maxs.X, maxs.Y, mins.Z),
		TVec3<type>(maxs.X, maxs.Y, maxs.Z)
	};

	for (int i = 0; i < int(EFrustumPlane::MAX); i++) {
		bool outside = false;
		for (int j = 0; j < 8; j++) {
			if (m_Planes[i].DotCoord(corners[j]) >= 0.0f) {
				outside = true;
				break;
			}
		}
		if (outside) {
			continue;
		}
		return false;
	}

	return true;
}

template<typename type>
auto TFrustum<type>::GetPlane(EFrustumPlane index) -> const TPlane<type>& {
	return m_Planes[int(index)];
}

template<typename type>
void CalcFrustumFarCorners(
	const TMat4<type>& matInvView, type fovInDegree, type aspect, type far,
	TVec3<type>& outLeftBottom, TVec3<type>& outRightBottom, TVec3<type>& outLeftTop, TVec3<type>& outRightTop) {

	const type yFac = tanf(DegToRad(fovInDegree));
	const type xFac = yFac * aspect;

	const TVec3<type> right = TVec3<type>(matInvView.M[0], matInvView.M[4], matInvView.M[8]);
	const TVec3<type> up = TVec3<type>(matInvView.M[1], matInvView.M[5], matInvView.M[9]);
	const TVec3<type> forward = TVec3<type>(matInvView.M[2], matInvView.M[6], matInvView.M[10]);
	const TVec3<type> position = TVec3<type>(matInvView.M[3], matInvView.M[7], matInvView.M[11]);

	outLeftTop = position + forward * far - far * right * xFac * far + up * yFac * far;
	outRightTop = position + forward * far + far * right * xFac * far + up * yFac * far;
	outLeftBottom = position + forward * far - far * right * xFac * far - up * yFac * far;
	outRightBottom = position + forward * far + far * right * xFac * far - up * yFac * far;
}