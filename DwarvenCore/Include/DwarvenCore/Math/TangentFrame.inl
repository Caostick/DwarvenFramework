#pragma once

#include <DwarvenCore/Types.h>

template<typename type>
auto ComputeQTangentFromTangentFrame(const TMat3<type>& tangentFrame)->TVec4<type> {
	const TVec3<type> tangent = TVec3<type>(0.0f, 0.0f, 1.0f);
	const TVec3<type> binormal = TVec3<type>(0.0f, 1.0f, 0.0f);
	const TVec3<type> normal = TVec3<type>(1.0f, 0.0f, 0.0f);

	return ComputeQTangentFromTangentFrame(tangent * tangentFrame, binormal * tangentFrame, normal * tangentFrame);
}

template<typename type>
auto ComputeQTangentFromTangentFrame(TVec3<type> tangent, TVec3<type> binormal, const TVec3<type>& normal)->TVec4<type> {
	// Orthogonalization by the Gram-Schmidt process. NB: Function assumes that normal has already been normalized.
	const type threshold = type(0.00001f);
	const type NdotT = TVec3<type>::Dot(normal, tangent);
	const type NdotB = TVec3<type>::Dot(normal, binormal);
	const type TdotB = TVec3<type>::Dot(tangent, binormal);

	if (fabs(NdotT) > threshold ||
		fabs(NdotB) > threshold ||
		fabs(TdotB) > threshold) {
		tangent = tangent - normal * NdotT;
		binormal = binormal - normal * NdotB - tangent * (TVec3<type>::Dot(tangent, binormal) / TVec3<type>::Dot(tangent, tangent));

		tangent.Normalize();
		binormal.Normalize();
	}

	const type s = TVec3<type>::Dot(tangent, TVec3<type>::Cross(binormal, normal)) < 0.0f ? -1.0f : 1.0f;
	const TVec3<type> sNormal = normal * s;

	const TMat3<type> frame = TMat3<type>(
		tangent.X, binormal.X, sNormal.X,
		tangent.Y, binormal.Y, sNormal.Y,
		tangent.Z, binormal.Z, sNormal.Z);

	TQuat<type> qTangent = QuaternionFromMatrix(frame);

	union {
		float F;
		uint32 I;
	} signCheck;
	signCheck.F = float(qTangent.W);

	if (isnan(qTangent.W) || isinf(qTangent.W)) {
		// Flush NaN and Inf to positive/negative zero.
		signCheck.I &= 0x80000000;
		qTangent.W = type(signCheck.F);
	}

	// Make sure we don't end up with 0 as w component
	signCheck.F = qTangent.W;
	const bool isNegative = (signCheck.I & 0x80000000) == 0x80000000;
	const bool flipSign = (!isNegative && s < 0.0f) || (isNegative && s > 0.0f);

	if ((isNegative || flipSign) && fabsf(qTangent.W) < threshold) {
		const type renomalization = type(sqrt(type(1)) - threshold * threshold);

		qTangent.W = isNegative ? -threshold : threshold;
		qTangent.Z *= renomalization;
		qTangent.Y *= renomalization;
		qTangent.X *= renomalization;
	}

	// Encode reflection into quaternion's w element by making sign of w negative
	// if y axis needs to be flipped, positive otherwise
	if (flipSign) {
		qTangent *= -1.0f;
	}

	return TVec4<type>(qTangent.X, qTangent.Y, qTangent.Z, qTangent.W);
}