#pragma once

template<typename type>
TMat3<type>::TMat3() {
	M[0] = type(0); M[1] = type(0); M[2] = type(0);
	M[3] = type(0); M[4] = type(0); M[5] = type(0);
	M[6] = type(0); M[7] = type(0); M[8] = type(0);
}

template<typename type>
TMat3<type>::TMat3(type m0, type m1, type m2, type m3, type m4, type m5, type m6, type m7, type m8) {
	M[0] = m0; M[1] = m1; M[2] = m2;
	M[3] = m3; M[4] = m4; M[5] = m5;
	M[6] = m6; M[7] = m7; M[8] = m8;
}

template<typename type>
TMat3<type>::TMat3(const TVec3<type>& r0, const TVec3<type>& r1, const TVec3<type>& r2) {
	M[0] = r0.X; M[1] = r0.Y; M[2] = r0.Z;
	M[3] = r1.X; M[4] = r1.Y; M[5] = r1.Z;
	M[6] = r2.X; M[7] = r2.Y; M[8] = r2.Z;
}

template<typename type>
template<typename type2>
TMat3<type>::TMat3(const TMat3<type2>& a_Matrix) {
	M[0] = type(a_Matrix.M[0]);
	M[1] = type(a_Matrix.M[1]);
	M[2] = type(a_Matrix.M[2]);
	M[3] = type(a_Matrix.M[3]);
	M[4] = type(a_Matrix.M[4]);
	M[5] = type(a_Matrix.M[5]);
	M[6] = type(a_Matrix.M[6]);
	M[7] = type(a_Matrix.M[7]);
	M[8] = type(a_Matrix.M[8]);
}


template<typename type>
void TMat3<type>::ToEulerAngles(type& pitch, type& yaw, type& roll) const {
	const TQuat<type> quat = TQuat<type>::FromMatrix(*this);
	quat.ToEulerAngles(pitch, yaw, roll);
}

template<typename type>
auto TMat3<type>::Empty() -> TMat3<type> {
	return TMat3<type>(
		type(0), type(0), type(0),
		type(0), type(0), type(0),
		type(0), type(0), type(0));
}

template<typename type>
auto TMat3<type>::Identity() -> TMat3<type> {
	return TMat3<type>(
		type(1), type(0), type(0),
		type(0), type(1), type(0),
		type(0), type(0), type(1));
}

template<typename type>
auto TMat3<type>::Transposed(const TMat3<type>& mat) -> TMat3<type> {
	return TMat3<type>(
		mat.M[0], mat.M[3], mat.M[6],
		mat.M[1], mat.M[4], mat.M[7],
		mat.M[2], mat.M[5], mat.M[8]);
}

template<typename type>
auto TMat3<type>::RotationX(type angle) -> TMat3<type> {
	const type s = Sin<type>(angle);
	const type c = Cos<type>(angle);
	return TMat3<type>(
		type(1), type(0), type(0),
		type(0), c, -s,
		type(0), s, c);
}

template<typename type>
auto TMat3<type>::RotationY(type angle) -> TMat3<type> {
	const type s = Sin<type>(angle);
	const type c = Cos<type>(angle);
	return TMat3<type>(
		c, type(0), s,
		type(0), type(1), type(0),
		-s, type(0), c);
}

template<typename type>
auto TMat3<type>::RotationZ(type angle) -> TMat3<type> {
	const type s = Sin<type>(angle);
	const type c = Cos<type>(angle);
	return TMat3<type>(
		c, -s, type(0),
		s, c, type(0),
		type(0), type(0), type(1));
}

template<typename type>
auto TMat3<type>::Rotation(type angle, const TVec3<type>& axis) -> TMat3<type> {
	const type s = Sin<type>(angle);
	const type c = Cos<type>(angle);
	const TVec3<type> n = TVec3<type>::Normalized(axis);
	return TMat3<type>(
		c + n.X * n.X * (type(1) - c),
		n.X * n.Y * (type(1) - c) - n.Z * s,
		n.X * n.Z * (type(1) - c) + n.Y * s,
		n.Y * n.X * (type(1) - c) + n.Z * s,
		c + n.Y * n.Y * (type(1) - c),
		n.Y * n.Z * (type(1) - c) - n.X * s,
		n.Z * n.X * (type(1) - c) - n.Y * s,
		n.Z * n.Y * (type(1) - c) + n.X * s,
		c + n.Z * n.Z * (type(1) - c));
}

template<typename type>
auto TMat3<type>::FromEulerAngles(type pitch, type yaw, type roll) -> TMat3<type> {
	return TMat3<type>::FromQuaternion(TQuat<type>::FromEulerAngles(pitch, yaw, roll));
}

template<typename type>
auto TMat3<type>::Scaling(const TVec3<type>& scale) -> TMat3<type> {
	return TMat3<type>(
		scale.X, type(0), type(0),
		type(0), scale.Y, type(0),
		type(0), type(0), scale.Z);
}

template<typename type>
auto TMat3<type>::Scaling(type scale) -> TMat3<type> {
	return TMat3<type>(
		scale, type(0), type(0),
		type(0), scale, type(0),
		type(0), type(0), scale);
}

template<typename type>
auto TMat3<type>::Scaling(type sx, type sy, type sz) -> TMat3<type> {
	return TMat3<type>(
		sx, type(0), type(0),
		type(0), sy, type(0),
		type(0), type(0), sz);
}

template<typename type>
auto TMat3<type>::FromQuaternion(const TQuat<type>& quat) -> TMat3<type> {
	const type& x = quat.X;
	const type& y = quat.Y;
	const type& z = quat.Z;
	const type& w = quat.W;

	return TMat3<type>(
		type(1) - type(2) * (y * y + z * z),
		type(2) * (x * y - z * w),
		type(2) * (x * z + y * w),
		type(2) * (x * y + z * w),
		type(1) - type(2) * (x * x + z * z),
		type(2) * (y * z - x * w),
		type(2) * (x * z - y * w),
		type(2) * (y * z + x * w),
		type(1) - type(2) * (x * x + y * y));
}

template<typename type>
auto TMat3<type>::LookAt(const TVec3<type>& origin, const TVec3<type>& target, const TVec3<type>& up) -> TMat3<type> {
	TVec3<type> z = TVec3<type>::Normalized(target - origin);
	TVec3<type> x = TVec3<type>::Normalized(TVec3<type>::Cross(z, up));
	TVec3<type> y = TVec3<type>::Normalized(TVec3<type>::Cross(x, z));
	return TMat3<type>(
		x.X, y.X, z.X,
		x.Y, y.Y, z.Y,
		x.Z, y.Z, z.Z);
}

template<typename type>
auto TMat3<type>::RightVector(const TMat3<type>& mat) -> TVec3<type> {
	return TVec3<type>(mat.M[0], mat.M[3], mat.M[6]);
}

template<typename type>
auto TMat3<type>::UpVector(const TMat3<type>& mat) -> TVec3<type> {
	return TVec3<type>(mat.M[1], mat.M[4], mat.M[7]);
}

template<typename type>
auto TMat3<type>::ForwardVector(const TMat3<type>& mat) -> TVec3<type> {
	return TVec3<type>(mat.M[2], mat.M[5], mat.M[8]);
}



template<typename type>
auto operator + (const TMat3<type>& left, const TMat3<type>& right) -> TMat3<type> {
	return TMat3<type>(
		left.M[0] + right.M[0], left.M[1] + right.M[1], left.M[2] + right.M[2],
		left.M[3] + right.M[3], left.M[4] + right.M[4], left.M[5] + right.M[5],
		left.M[6] + right.M[6], left.M[7] + right.M[7], left.M[8] + right.M[8]);
}

template<typename type>
auto operator - (const TMat3<type>& left, const TMat3<type>& right) -> TMat3<type> {
	return TMat3<type>(
		left.M[0] - right.M[0], left.M[1] - right.M[1], left.M[2] - right.M[2],
		left.M[3] - right.M[3], left.M[4] - right.M[4], left.M[5] - right.M[5],
		left.M[6] - right.M[6], left.M[7] - right.M[7], left.M[8] - right.M[8]);
}

template<typename type>
auto operator * (const TMat3<type>& left, const TMat3<type>& right) -> TMat3<type> {
	return TMat3<type>(
		left.M[0] * right.M[0] + left.M[3] * right.M[1] + left.M[6] * right.M[2],
		left.M[1] * right.M[0] + left.M[4] * right.M[1] + left.M[7] * right.M[2],
		left.M[2] * right.M[0] + left.M[5] * right.M[1] + left.M[8] * right.M[2],
		left.M[0] * right.M[3] + left.M[3] * right.M[4] + left.M[6] * right.M[5],
		left.M[1] * right.M[3] + left.M[4] * right.M[4] + left.M[7] * right.M[5],
		left.M[2] * right.M[3] + left.M[5] * right.M[4] + left.M[8] * right.M[5],
		left.M[0] * right.M[6] + left.M[3] * right.M[7] + left.M[6] * right.M[8],
		left.M[1] * right.M[6] + left.M[4] * right.M[7] + left.M[7] * right.M[8],
		left.M[2] * right.M[6] + left.M[5] * right.M[7] + left.M[8] * right.M[8]);
}

template<typename type>
auto operator * (const TMat3<type>& left, type right) -> TMat3<type> {
	return TMat3<type>(
		left.M[0] * right, left.M[1] * right, left.M[2] * right,
		left.M[3] * right, left.M[4] * right, left.M[5] * right,
		left.M[6] * right, left.M[7] * right, left.M[8] * right);
}

template<typename type>
auto operator * (type left, const TMat3<type>& right) -> TMat3<type> {
	return TMat3<type>(
		left * right.M[0], left * right.M[1], left * right.M[2],
		left * right.M[3], left * right.M[4], left * right.M[5],
		left * right.M[6], left * right.M[7], left * right.M[8]);
}

template<typename type>
auto operator * (const TVec3<type>& vec, const TMat3<type>& mat) -> TVec3<type> {
	return TVec3<type>(
		vec.X * mat.M[0] + vec.Y * mat.M[1] + vec.Z * mat.M[2],
		vec.X * mat.M[3] + vec.Y * mat.M[4] + vec.Z * mat.M[5],
		vec.X * mat.M[6] + vec.Y * mat.M[7] + vec.Z * mat.M[8]);
}