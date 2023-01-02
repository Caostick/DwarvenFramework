#pragma once

template<typename type>
TQuat<type>::TQuat()
	: X(type(0))
	, Y(type(0))
	, Z(type(0))
	, W(type(0)) {}

template<typename type>
TQuat<type>::TQuat(type x, type y, type z, type w)
	: X(x)
	, Y(y)
	, Z(z)
	, W(w) {

}

template<typename type>
template<typename type2>
TQuat<type>::TQuat(const TQuat<type2>& other)
	: X(type(other.X))
	, Y(type(other.Y))
	, Z(type(other.Z))
	, W(type(other.W)) {

}

template<typename type>
void QuaternionToEulerAngles(const TQuat<type>& quat, type& pitch, type& yaw, type& roll) {
	const type t0 = type(2) * (quat.W * quat.X + quat.Y * quat.Z);
	const type t1 = type(1) - type(2) * (quat.X * quat.X + quat.Y * quat.Y);
	roll = Atan2<type>(t0, t1);

	type t2 = type(2) * (quat.W * quat.Y - quat.Z * quat.X);
	t2 = t2 > type(1) ? type(1) : t2;
	t2 = t2 < type(-1) ? type(-1) : t2;
	pitch = Asin<type>(t2);

	const type t3 = type(2) * (quat.W * quat.Z + quat.X * quat.Y);
	const type t4 = type(1) - type(2) * (quat.Y * quat.Y + quat.Z * quat.Z);
	yaw = Atan2<type>(t3, t4);
}

template<typename type>
auto QuaternionFromEulerAngles(type pitch, type yaw, type roll) -> TQuat<type> {
	const type pitchSin = Sin<type>(pitch * type(0.5));
	const type pitchCos = Cos<type>(pitch * type(0.5));
	const type yawSin = Sin<type>(yaw * type(0.5));
	const type yawCos = Cos<type>(yaw * type(0.5));
	const type rollSin = Sin<type>(roll * type(0.5));
	const type rollCos = Cos<type>(roll * type(0.5));
	const type rotPitchCosYawCos = pitchCos * yawCos;
	const type rotPitchSinYawSin = pitchSin * yawSin;
	return TQuat<type>(
		rollSin * rotPitchCosYawCos - rollCos * rotPitchSinYawSin,
		rollCos * pitchSin * yawCos + rollSin * pitchCos * yawSin,
		rollCos * pitchCos * yawSin - rollSin * pitchSin * yawCos,
		rollCos * rotPitchCosYawCos + rollSin * rotPitchSinYawSin);
}

template<typename type>
auto QuaternionFromAxisRotation(TQuat<type>* quat, const TVec3<type>& vec, type angle) -> TQuat<type> {
	vec.Normalize();
	const type s = Sin<type>(angle * type(0.5));
	const type c = Cos<type>(angle * type(0.5));
	return TQuat<type>(vec.X * s, vec.Y * s, vec.Z * s, c);
}

template<typename type>
auto QuaternionFromMatrix(const TMat3<type>& mat) -> TQuat<type> {
	const type t = mat.M[0] + mat.M[4] + mat.M[8];

	if (t > type(0)) {
		const type s = type(Sqrt<type>(type(1)) + t) * type(2);
		return TQuat<type>(
			(mat.M[7] - mat.M[5]) / s,
			(mat.M[2] - mat.M[6]) / s,
			(mat.M[3] - mat.M[1]) / s,
			type(0.25) * s);
	} else if (mat.M[0] > mat.M[4] && mat.M[0] > mat.M[8]) {
		const type s = type(Sqrt<type>(type(1) + mat.M[0] - mat.M[4] - mat.M[8])) * type(2);
		return TQuat<type>(
			type(0.25) * s,
			(mat.M[3] + mat.M[1]) / s,
			(mat.M[2] + mat.M[6]) / s,
			(mat.M[7] - mat.M[5]) / s);
	} else if (mat.M[4] > mat.M[8]) {
		const type s = type(Sqrt<type>(type(1) + mat.M[4] - mat.M[0] - mat.M[8])) * type(2);
		return TQuat<type>(
			(mat.M[3] + mat.M[1]) / s,
			type(0.25) * s,
			(mat.M[7] + mat.M[5]) / s,
			(mat.M[2] - mat.M[6]) / s);
	} else {
		const type s = type(Sqrt<type>(type(1) + mat.M[8] - mat.M[0] - mat.M[4])) * type(2);
		return TQuat<type>(
			(mat.M[2] + mat.M[6]) / s,
			(mat.M[7] + mat.M[5]) / s,
			type(0.25) * s,
			(mat.M[3] - mat.M[1]) / s);
	}
}

template<typename type>
auto SLerp(const TQuat<type>& quatA, const TQuat<type>& quatB, type factor) -> TQuat<type> {
	type cosom = quatA.X * quatB.X + quatA.Y * quatB.Y + quatA.Z * quatB.Z + quatA.W * quatB.W;

	TQuat<type> end = quatB;
	if (cosom < type(0.0)) {
		cosom = -cosom;
		end.X = -end.X;
		end.Y = -end.Y;
		end.Z = -end.Z;
		end.W = -end.W;
	}

	type sclp, sclq;
	if ((type(1.0) - cosom) > type(0.0001)) {
		type omega, sinom;
		omega = Acos<type>(cosom);
		sinom = Sin<type>(omega);
		sclp = Sin<type>((type(1.0) - factor) * omega) / sinom;
		sclq = Sin<type>(factor * omega) / sinom;
	} else {
		sclp = type(1.0) - factor;
		sclq = factor;
	}

	return TQuat<type>(
		sclp * quatA.X + sclq * end.X,
		sclp * quatA.Y + sclq * end.Y,
		sclp * quatA.Z + sclq * end.Z,
		sclp * quatA.W + sclq * end.W);
}

template<typename type>
auto Normalized(const TQuat<type>& quat)->TQuat<type> {
	const float magnitude = sqrt(quat.W * quat.W + quat.X * quat.X + quat.Y * quat.Y + quat.Z * quat.Z);
	return TQuat<type>(quat.X / magnitude, quat.Y / magnitude, quat.Z / magnitude, quat.W / magnitude);
}

template<typename type>
TQuat<type> operator * (const TQuat<type>& left, type right) {
	return TQuat<type>(left.X * right, left.Y * right, left.Z * right, left.W * right);
}

template<typename type>
const TQuat<type>& operator *= (TQuat<type>& left, type right) {
	left.X *= right;
	left.Y *= right;
	left.Z *= right;
	left.W *= right;
	return left;
}
