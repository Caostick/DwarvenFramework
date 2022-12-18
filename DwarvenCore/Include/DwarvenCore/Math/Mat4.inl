#pragma once

template<typename type>
TMat4<type>::TMat4() {
	M[0] = type(0); M[1] = type(0); M[2] = type(0); M[3] = type(0);
	M[4] = type(0); M[5] = type(0); M[6] = type(0); M[7] = type(0);
	M[8] = type(0); M[9] = type(0); M[10] = type(0); M[11] = type(0);
	M[12] = type(0); M[13] = type(0); M[14] = type(0); M[15] = type(0);
}

template<typename type>
TMat4<type>::TMat4(type m11, type m12, type m13, type m14, type m21, type m22, type m23, type m24, type m31, type m32, type m33, type m34, type m41, type m42, type m43, type m44) {
	M[0] = m11; M[1] = m12; M[2] = m13; M[3] = m14;
	M[4] = m21; M[5] = m22; M[6] = m23; M[7] = m24;
	M[8] = m31; M[9] = m32; M[10] = m33; M[11] = m34;
	M[12] = m41; M[13] = m42; M[14] = m43; M[15] = m44;
}

template<typename type>
template<typename type2>
TMat4<type>::TMat4(const TMat4<type2>& matrix) {
	M[0] = type(matrix.M[0]);
	M[1] = type(matrix.M[1]);
	M[2] = type(matrix.M[2]);
	M[3] = type(matrix.M[3]);
	M[4] = type(matrix.M[4]);
	M[5] = type(matrix.M[5]);
	M[6] = type(matrix.M[6]);
	M[7] = type(matrix.M[7]);
	M[8] = type(matrix.M[8]);
	M[9] = type(matrix.M[9]);
	M[10] = type(matrix.M[10]);
	M[11] = type(matrix.M[11]);
	M[12] = type(matrix.M[12]);
	M[13] = type(matrix.M[13]);
	M[14] = type(matrix.M[14]);
	M[15] = type(matrix.M[15]);
}

template<typename type>
template<typename type2>
TMat4<type>::TMat4(const TMat3<type2>& matrix) {
	M[0] = type(matrix.M[0]);
	M[1] = type(matrix.M[1]);
	M[2] = type(matrix.M[2]);
	M[3] = type(0);
	M[4] = type(matrix.M[3]);
	M[5] = type(matrix.M[4]);
	M[6] = type(matrix.M[5]);
	M[7] = type(0);
	M[8] = type(matrix.M[6]);
	M[9] = type(matrix.M[7]);
	M[10] = type(matrix.M[8]);
	M[11] = type(0);
	M[12] = type(0);
	M[13] = type(0);
	M[14] = type(0);
	M[15] = type(1);
}

template<typename type>
template<typename type2>
TMat4<type>::TMat4(type2 m11, type2 m12, type2 m13, type2 m14,
	type2 m21, type2 m22, type2 m23, type2 m24,
	type2 m31, type2 m32, type2 m33, type2 m34,
	type2 m41, type2 m42, type2 m43, type2 m44) {
	M[0] = type(m11); M[1] = type(m12); M[2] = type(m13); M[3] = type(m14);
	M[4] = type(m21); M[5] = type(m22); M[6] = type(m23); M[7] = type(m24);
	M[8] = type(m31); M[9] = type(m32); M[10] = type(m33); M[11] = type(m34);
	M[12] = type(m41); M[13] = type(m42); M[14] = type(m43); M[15] = type(m44);
}

template<typename type>
auto TMat4<type>::Empty() -> TMat4<type> {
	return TMat4<type>(
		type(0), type(0), type(0), type(0),
		type(0), type(0), type(0), type(0),
		type(0), type(0), type(0), type(0),
		type(0), type(0), type(0), type(0));
}

template<typename type>
auto TMat4<type>::Identity() -> TMat4<type> {
	return TMat4<type>(
		type(1), type(0), type(0), type(0),
		type(0), type(1), type(0), type(0),
		type(0), type(0), type(1), type(0),
		type(0), type(0), type(0), type(1));
}

template<typename type>
auto TMat4<type>::Transposed(const TMat4<type>& mat) -> TMat4<type> {
	return TMat4<type>(
		mat.M[0], mat.M[4], mat.M[8], mat.M[12],
		mat.M[1], mat.M[5], mat.M[9], mat.M[13],
		mat.M[2], mat.M[6], mat.M[10], mat.M[14],
		mat.M[3], mat.M[7], mat.M[11], mat.M[15]);
}

template<typename type>
auto TMat4<type>::InverseTransform(const TMat4<type>& mat) -> TMat4<type> {
	return TMat4<type>(
		mat.M[0], mat.M[4], mat.M[8], -(mat.M[0] * mat.M[3] + mat.M[4] * mat.M[7] + mat.M[8] * mat.M[11]),
		mat.M[1], mat.M[5], mat.M[9], -(mat.M[1] * mat.M[3] + mat.M[5] * mat.M[7] + mat.M[9] * mat.M[11]),
		mat.M[2], mat.M[6], mat.M[10], -(mat.M[2] * mat.M[3] + mat.M[6] * mat.M[7] + mat.M[10] * mat.M[11]),
		mat.M[12], mat.M[13], mat.M[14], mat.M[15]);
}

template<typename type>
auto TMat4<type>::Translation(const TVec3<type>& position) -> TMat4<type> {
	return TMat4<type>(
		type(1), type(0), type(0), position.X,
		type(0), type(1), type(0), position.Y,
		type(0), type(0), type(1), position.Z,
		type(0), type(0), type(0), type(1));
}

template<typename type>
auto TMat4<type>::Translation(type x, type y, type z) -> TMat4<type> {
	return TMat4<type>(
		type(1), type(0), type(0), x,
		type(0), type(1), type(0), y,
		type(0), type(0), type(1), z,
		type(0), type(0), type(0), type(1));
}

template<typename type>
auto TMat4<type>::RotationX(const type angle) -> TMat4<type> {
	const type s = Sin<type>(angle);
	const type c = Cos<type>(angle);
	return TMat4<type>(
		type(1), type(0), type(0), type(0),
		type(0), c, -s, type(0),
		type(0), s, c, type(0),
		type(0), type(0), type(0), type(1));
}

template<typename type>
auto TMat4<type>::RotationY(const type angle) -> TMat4<type> {
	const type s = Sin<type>(angle);
	const type c = Cos<type>(angle);
	return TMat4<type>(
		c, type(0), s, type(0),
		type(0), type(1), type(0), type(0),
		-s, type(0), c, type(0),
		type(0), type(0), type(0), type(1));
}

template<typename type>
auto TMat4<type>::RotationZ(const type angle) -> TMat4<type> {
	const type s = Sin<type>(angle);
	const type c = Cos<type>(angle);
	return TMat4<type>(
		c, -s, type(0), type(0),
		s, c, type(0), type(0),
		type(0), type(0), type(1), type(0),
		type(0), type(0), type(0), type(1));
}

template<typename type>
auto TMat4<type>::Rotation(const type angle, const TVec3<type>& axis) -> TMat4<type> {
	const type s = Sin<type>(angle);
	const type c = Cos<type>(angle);
	const TVec3<type> n = TVec3<type>::Normalized(axis);
	return TMat4<type>(
		c + n.X * n.X * (type(1) - c), n.X * n.Y * (type(1) - c) - n.Z * s, n.X * n.Z * (type(1) - c) + n.Y * s, type(0),
		n.Y * n.X * (type(1) - c) + n.Z * s, c + n.Y * n.Y * (type(1) - c), n.Y * n.Z * (type(1) - c) - n.X * s, type(0),
		n.Z * n.X * (type(1) - c) - n.Y * s, n.Z * n.Y * (type(1) - c) + n.X * s, c + n.Z * n.Z * (type(1) - c), type(0),
		type(0), type(0), type(0), type(1));
}

template<typename type>
auto TMat4<type>::Scaling(const TVec3<type>& scale) -> TMat4<type> {
	return TMat4<type>(
		scale.X, type(0), type(0), type(0),
		type(0), scale.Y, type(0), type(0),
		type(0), type(0), scale.Z, type(0),
		type(0), type(0), type(0), type(1));
}

template<typename type>
auto TMat4<type>::Scaling(type x, type y, type z) -> TMat4<type> {
	return TMat4<type>(
		x, type(0), type(0), type(0),
		type(0), y, type(0), type(0),
		type(0), type(0), z, type(0),
		type(0), type(0), type(0), type(1));
}

template<typename type>
auto TMat4<type>::Scaling(type scale) -> TMat4<type> {
	return TMat4<type>(
		scale, type(0), type(0), type(0),
		type(0), scale, type(0), type(0),
		type(0), type(0), scale, type(0),
		type(0), type(0), type(0), type(1));
}

template<typename type>
auto TMat4<type>::LookAt(const TVec3<type>& eye, const TVec3<type>& at, const TVec3<type>& up) -> TMat4<type> {
	const TVec3<type> zaxis = TVec3<type>::Normalized(at - eye);
	const TVec3<type> xaxis = TVec3<type>::Normalized(TVec3<type>::Cross(up, zaxis));
	const TVec3<type> yaxis = TVec3<type>::Cross(zaxis, xaxis);

	return TMat4<type>(
		xaxis.X, xaxis.Y, xaxis.Z, TVec3<type>::Dot(xaxis, -eye),
		yaxis.X, yaxis.Y, yaxis.Z, TVec3<type>::Dot(yaxis, -eye),
		zaxis.X, zaxis.Y, zaxis.Z, TVec3<type>::Dot(zaxis, -eye),
		0, 0, 0, 1);
}

template<typename type>
auto TMat4<type>::Ortho(type width, type height, type zNear, type zFar) -> TMat4<type> {
#if 0
	return TMat4<type>(
		type(2) / width, type(0), type(0), type(0),
		type(0), type(2) / height, type(0), type(0),
		type(0), type(0), type(2) / (zFar - zNear), type(0),
		type(0), type(0), type(0), type(1));
#else
	return TMat4<type>(
		type(2) / width, type(0), type(0), type(0),
		type(0), -type(2) / height, type(0), type(0),
		type(0), type(0), type(1) / (zFar - zNear), type(0),
		type(0), type(0), type(0), type(1));
#endif
}

template<typename type>
auto TMat4<type>::Ortho(type xMin, type xMax, type yMin, type yMax, type zMin, type zMax) -> TMat4<type> {
#if 0
	return TMat4<type>(
		type(2) / (xMax - xMin), type(0), type(0), (xMax + xMin) / (xMin - xMax),
		type(0), type(2) / (yMax - yMin), type(0), (yMax + yMin) / (yMin - yMax),
		type(0), type(0), type(2) / (zMax - zMin), (zMax + zMin) / (zMin - zMax),
		type(0), type(0), type(0), type(1));
#else
	return TMat4<type>(
		type(2) / (xMax - xMin), type(0), type(0), type(0),
		type(0), type(2) / (yMin - yMax), type(0), type(0),
		type(0), type(0), -type(1) / (zMin - zMax), type(0),
		(xMax + xMin) / (xMax - xMin), (yMin + yMax) / (yMin - yMax), -zMin / (zMin - zMax), type(1)
		);
#endif
}

template<typename type>
auto TMat4<type>::Perspective(type fov, type aspect, type zNear, type zFar) -> TMat4<type> {
	const type ymax = zNear * Tan<type>(fov / type(2));
	const type ymin = ymax;
	const type xmax = ymax * aspect;
	const type xmin = ymin * aspect;
	const type depth = zFar - zNear;
#if 0 // [-1, 1]
	return TMat4<type>(
		type(2) * zNear / (xmin + xmax), type(0), type(0), type(0),
		type(0), -type(2) * zNear / (ymin + ymax), type(0), type(0),
		type(0), type(0), (zFar + zNear) / depth, -type(2) * zFar * zNear / depth,
		type(0), type(0), type(1), type(0));
#else // [0, 1]
	return TMat4<type>(
		type(2) * zNear / (xmin + xmax), type(0), type(0), type(0),
		type(0), -type(2) * zNear / (ymin + ymax), type(0), type(0),
		type(0), type(0), zFar / depth, -zFar * zNear / depth,
		type(0), type(0), type(1), type(0));
#endif
}

template<typename type>
auto TMat4<type>::Perspective(type left, type right, type bottom, type top, type zNear, type zFar) -> TMat4<type> {
	return TMat4<type>(
		type(2) * zNear / (right - left), type(0), (right + left) / (right - left), type(0),
		type(0), type(2) * zNear / (top - bottom), (top + bottom) / (top - bottom), type(0),
		type(0), type(0), (zFar + zNear) / (zFar - zNear), type(2) * zFar * zNear / (zNear - zFar),
		type(0), type(0), type(1), type(0));
}

template<typename type>
auto TMat4<type>::Pick(type x, type y, type width, type height) -> TMat4<type> {
	return TMat4<type>(
		width, type(0), type(0), width - x * type(2),
		type(0), height, type(0), y * type(2) - height,
		type(0), type(0), type(1), type(0),
		type(0), type(0), type(0), type(1));
}

template<typename type>
auto TMat4<type>::Inversed(const TMat4<type>& mat) -> TMat4<type> {
	TMat4<type> res;

	res.M[0] = mat.M[5] * mat.M[10] * mat.M[15] -
		mat.M[5] * mat.M[11] * mat.M[14] -
		mat.M[9] * mat.M[6] * mat.M[15] +
		mat.M[9] * mat.M[7] * mat.M[14] +
		mat.M[13] * mat.M[6] * mat.M[11] -
		mat.M[13] * mat.M[7] * mat.M[10];

	res.M[4] = -mat.M[4] * mat.M[10] * mat.M[15] +
		mat.M[4] * mat.M[11] * mat.M[14] +
		mat.M[8] * mat.M[6] * mat.M[15] -
		mat.M[8] * mat.M[7] * mat.M[14] -
		mat.M[12] * mat.M[6] * mat.M[11] +
		mat.M[12] * mat.M[7] * mat.M[10];

	res.M[8] = mat.M[4] * mat.M[9] * mat.M[15] -
		mat.M[4] * mat.M[11] * mat.M[13] -
		mat.M[8] * mat.M[5] * mat.M[15] +
		mat.M[8] * mat.M[7] * mat.M[13] +
		mat.M[12] * mat.M[5] * mat.M[11] -
		mat.M[12] * mat.M[7] * mat.M[9];

	res.M[12] = -mat.M[4] * mat.M[9] * mat.M[14] +
		mat.M[4] * mat.M[10] * mat.M[13] +
		mat.M[8] * mat.M[5] * mat.M[14] -
		mat.M[8] * mat.M[6] * mat.M[13] -
		mat.M[12] * mat.M[5] * mat.M[10] +
		mat.M[12] * mat.M[6] * mat.M[9];

	res.M[1] = -mat.M[1] * mat.M[10] * mat.M[15] +
		mat.M[1] * mat.M[11] * mat.M[14] +
		mat.M[9] * mat.M[2] * mat.M[15] -
		mat.M[9] * mat.M[3] * mat.M[14] -
		mat.M[13] * mat.M[2] * mat.M[11] +
		mat.M[13] * mat.M[3] * mat.M[10];

	res.M[5] = mat.M[0] * mat.M[10] * mat.M[15] -
		mat.M[0] * mat.M[11] * mat.M[14] -
		mat.M[8] * mat.M[2] * mat.M[15] +
		mat.M[8] * mat.M[3] * mat.M[14] +
		mat.M[12] * mat.M[2] * mat.M[11] -
		mat.M[12] * mat.M[3] * mat.M[10];

	res.M[9] = -mat.M[0] * mat.M[9] * mat.M[15] +
		mat.M[0] * mat.M[11] * mat.M[13] +
		mat.M[8] * mat.M[1] * mat.M[15] -
		mat.M[8] * mat.M[3] * mat.M[13] -
		mat.M[12] * mat.M[1] * mat.M[11] +
		mat.M[12] * mat.M[3] * mat.M[9];

	res.M[13] = mat.M[0] * mat.M[9] * mat.M[14] -
		mat.M[0] * mat.M[10] * mat.M[13] -
		mat.M[8] * mat.M[1] * mat.M[14] +
		mat.M[8] * mat.M[2] * mat.M[13] +
		mat.M[12] * mat.M[1] * mat.M[10] -
		mat.M[12] * mat.M[2] * mat.M[9];

	res.M[2] = mat.M[1] * mat.M[6] * mat.M[15] -
		mat.M[1] * mat.M[7] * mat.M[14] -
		mat.M[5] * mat.M[2] * mat.M[15] +
		mat.M[5] * mat.M[3] * mat.M[14] +
		mat.M[13] * mat.M[2] * mat.M[7] -
		mat.M[13] * mat.M[3] * mat.M[6];

	res.M[6] = -mat.M[0] * mat.M[6] * mat.M[15] +
		mat.M[0] * mat.M[7] * mat.M[14] +
		mat.M[4] * mat.M[2] * mat.M[15] -
		mat.M[4] * mat.M[3] * mat.M[14] -
		mat.M[12] * mat.M[2] * mat.M[7] +
		mat.M[12] * mat.M[3] * mat.M[6];

	res.M[10] = mat.M[0] * mat.M[5] * mat.M[15] -
		mat.M[0] * mat.M[7] * mat.M[13] -
		mat.M[4] * mat.M[1] * mat.M[15] +
		mat.M[4] * mat.M[3] * mat.M[13] +
		mat.M[12] * mat.M[1] * mat.M[7] -
		mat.M[12] * mat.M[3] * mat.M[5];

	res.M[14] = -mat.M[0] * mat.M[5] * mat.M[14] +
		mat.M[0] * mat.M[6] * mat.M[13] +
		mat.M[4] * mat.M[1] * mat.M[14] -
		mat.M[4] * mat.M[2] * mat.M[13] -
		mat.M[12] * mat.M[1] * mat.M[6] +
		mat.M[12] * mat.M[2] * mat.M[5];

	res.M[3] = -mat.M[1] * mat.M[6] * mat.M[11] +
		mat.M[1] * mat.M[7] * mat.M[10] +
		mat.M[5] * mat.M[2] * mat.M[11] -
		mat.M[5] * mat.M[3] * mat.M[10] -
		mat.M[9] * mat.M[2] * mat.M[7] +
		mat.M[9] * mat.M[3] * mat.M[6];

	res.M[7] = mat.M[0] * mat.M[6] * mat.M[11] -
		mat.M[0] * mat.M[7] * mat.M[10] -
		mat.M[4] * mat.M[2] * mat.M[11] +
		mat.M[4] * mat.M[3] * mat.M[10] +
		mat.M[8] * mat.M[2] * mat.M[7] -
		mat.M[8] * mat.M[3] * mat.M[6];

	res.M[11] = -mat.M[0] * mat.M[5] * mat.M[11] +
		mat.M[0] * mat.M[7] * mat.M[9] +
		mat.M[4] * mat.M[1] * mat.M[11] -
		mat.M[4] * mat.M[3] * mat.M[9] -
		mat.M[8] * mat.M[1] * mat.M[7] +
		mat.M[8] * mat.M[3] * mat.M[5];

	res.M[15] = mat.M[0] * mat.M[5] * mat.M[10] -
		mat.M[0] * mat.M[6] * mat.M[9] -
		mat.M[4] * mat.M[1] * mat.M[10] +
		mat.M[4] * mat.M[2] * mat.M[9] +
		mat.M[8] * mat.M[1] * mat.M[6] -
		mat.M[8] * mat.M[2] * mat.M[5];

	type det = type(1) / (mat.M[0] * res.M[0] + mat.M[1] * res.M[4] + mat.M[2] * res.M[8] + mat.M[3] * res.M[12]);
	//1 / det should be != 0 ^

	for (int i = 0; i < 16; ++i) {
		res.M[i] *= det;
	}
	return res;
}




template<typename type>
auto operator + (const TMat4<type>& left, const TMat4<type>& right) -> TMat4<type> {
	return TMat4<type>(
		left.M[0] + right.M[0], left.M[1] + right.M[1], left.M[2] + right.M[2], left.M[3] + right.M[3],
		left.M[4] + right.M[4], left.M[5] + right.M[5], left.M[6] + right.M[6], left.M[7] + right.M[7],
		left.M[8] + right.M[8], left.M[9] + right.M[9], left.M[10] + right.M[10], left.M[11] + right.M[11],
		left.M[12] + right.M[12], left.M[13] + right.M[13], left.M[14] + right.M[14], left.M[15] + right.M[15]);
}

template<typename type>
auto operator - (const TMat4<type>& left, const TMat4<type>& right) -> TMat4<type> {
	return TMat4<type>(
		left.M[0] - right.M[0], left.M[1] - right.M[1], left.M[2] - right.M[2], left.M[3] - right.M[3],
		left.M[4] - right.M[4], left.M[5] - right.M[5], left.M[6] - right.M[6], left.M[7] - right.M[7],
		left.M[8] - right.M[8], left.M[9] - right.M[9], left.M[10] - right.M[10], left.M[11] - right.M[11],
		left.M[12] - right.M[12], left.M[13] - right.M[13], left.M[14] - right.M[14], left.M[15] - right.M[15]);
}

template<typename type>
auto operator * (const TMat4<type>& left, const TMat4<type>& right) -> TMat4<type> {
	return TMat4<type>(
		left.M[0] * right.M[0] + left.M[4] * right.M[1] + left.M[8] * right.M[2] + left.M[12] * right.M[3],
		left.M[1] * right.M[0] + left.M[5] * right.M[1] + left.M[9] * right.M[2] + left.M[13] * right.M[3],
		left.M[2] * right.M[0] + left.M[6] * right.M[1] + left.M[10] * right.M[2] + left.M[14] * right.M[3],
		left.M[3] * right.M[0] + left.M[7] * right.M[1] + left.M[11] * right.M[2] + left.M[15] * right.M[3],
		left.M[0] * right.M[4] + left.M[4] * right.M[5] + left.M[8] * right.M[6] + left.M[12] * right.M[7],
		left.M[1] * right.M[4] + left.M[5] * right.M[5] + left.M[9] * right.M[6] + left.M[13] * right.M[7],
		left.M[2] * right.M[4] + left.M[6] * right.M[5] + left.M[10] * right.M[6] + left.M[14] * right.M[7],
		left.M[3] * right.M[4] + left.M[7] * right.M[5] + left.M[11] * right.M[6] + left.M[15] * right.M[7],
		left.M[0] * right.M[8] + left.M[4] * right.M[9] + left.M[8] * right.M[10] + left.M[12] * right.M[11],
		left.M[1] * right.M[8] + left.M[5] * right.M[9] + left.M[9] * right.M[10] + left.M[13] * right.M[11],
		left.M[2] * right.M[8] + left.M[6] * right.M[9] + left.M[10] * right.M[10] + left.M[14] * right.M[11],
		left.M[3] * right.M[8] + left.M[7] * right.M[9] + left.M[11] * right.M[10] + left.M[15] * right.M[11],
		left.M[0] * right.M[12] + left.M[4] * right.M[13] + left.M[8] * right.M[14] + left.M[12] * right.M[15],
		left.M[1] * right.M[12] + left.M[5] * right.M[13] + left.M[9] * right.M[14] + left.M[13] * right.M[15],
		left.M[2] * right.M[12] + left.M[6] * right.M[13] + left.M[10] * right.M[14] + left.M[14] * right.M[15],
		left.M[3] * right.M[12] + left.M[7] * right.M[13] + left.M[11] * right.M[14] + left.M[15] * right.M[15]);
}

template<typename type>
auto operator * (const TMat3<type>& left, const TMat4<type>& right) -> TMat4<type> {
	return TMat4<type>(
		left.M[0] * right.M[0] + left.M[3] * right.M[1] + left.M[6] * right.M[2],
		left.M[1] * right.M[0] + left.M[4] * right.M[1] + left.M[7] * right.M[2],
		left.M[2] * right.M[0] + left.M[5] * right.M[1] + left.M[8] * right.M[2],
		right.M[3],
		left.M[0] * right.M[4] + left.M[3] * right.M[5] + left.M[6] * right.M[6],
		left.M[1] * right.M[4] + left.M[4] * right.M[5] + left.M[7] * right.M[6],
		left.M[2] * right.M[4] + left.M[5] * right.M[5] + left.M[8] * right.M[6],
		right.M[7],
		left.M[0] * right.M[8] + left.M[3] * right.M[9] + left.M[6] * right.M[10],
		left.M[1] * right.M[8] + left.M[4] * right.M[9] + left.M[7] * right.M[10],
		left.M[2] * right.M[8] + left.M[5] * right.M[9] + left.M[8] * right.M[10],
		right.M[11],
		left.M[0] * right.M[12] + left.M[3] * right.M[13] + left.M[6] * right.M[14],
		left.M[1] * right.M[12] + left.M[4] * right.M[13] + left.M[7] * right.M[14],
		left.M[2] * right.M[12] + left.M[5] * right.M[13] + left.M[8] * right.M[14],
		right.M[15]);
}

template<typename type>
auto operator * (const TMat4<type>& left, const TMat3<type>& right) -> TMat4<type> {
	return TMat4<type>(
		left.M[0] * right.M[0] + left.M[4] * right.M[1] + left.M[8] * right.M[2],
		left.M[1] * right.M[0] + left.M[5] * right.M[1] + left.M[9] * right.M[2],
		left.M[2] * right.M[0] + left.M[6] * right.M[1] + left.M[10] * right.M[2],
		left.M[3] * right.M[0] + left.M[7] * right.M[1] + left.M[11] * right.M[2],
		left.M[0] * right.M[3] + left.M[4] * right.M[4] + left.M[8] * right.M[5],
		left.M[1] * right.M[3] + left.M[5] * right.M[4] + left.M[9] * right.M[5],
		left.M[2] * right.M[3] + left.M[6] * right.M[4] + left.M[10] * right.M[5],
		left.M[3] * right.M[3] + left.M[7] * right.M[4] + left.M[11] * right.M[5],
		left.M[0] * right.M[6] + left.M[4] * right.M[7] + left.M[8] * right.M[8],
		left.M[1] * right.M[6] + left.M[5] * right.M[7] + left.M[9] * right.M[8],
		left.M[2] * right.M[6] + left.M[6] * right.M[7] + left.M[10] * right.M[8],
		left.M[3] * right.M[6] + left.M[7] * right.M[7] + left.M[11] * right.M[8],
		left.M[12],
		left.M[13],
		left.M[14],
		left.M[15]);
}

template<typename type>
auto operator * (const TMat4<type>& left, type right) -> TMat4<type> {
	return TMat4<type>(
		left.M[0] * right, left.M[1] * right, left.M[2] * right, left.M[3] * right,
		left.M[4] * right, left.M[5] * right, left.M[6] * right, left.M[7] * right,
		left.M[8] * right, left.M[9] * right, left.M[10] * right, left.M[11] * right,
		left.M[12] * right, left.M[13] * right, left.M[14] * right, left.M[15] * right);
}

template<typename type>
auto operator * (type left, const TMat4<type>& right) -> TMat4<type> {
	return TMat4<type>(
		left * right.M[0], left * right.M[1], left * right.M[2], left * right.M[3],
		left * right.M[4], left * right.M[5], left * right.M[6], left * right.M[7],
		left * right.M[8], left * right.M[9], left * right.M[10], left * right.M[11],
		left * right.M[12], left * right.M[13], left * right.M[14], left * right.M[15]);
}



template<typename type>
auto operator * (const TVec3<type>& vec, const TMat4<type>& mat) -> TVec3<type> {
	return TVec3<type>(
		vec.X * mat.M[0] + vec.Y * mat.M[1] + vec.Z * mat.M[2],
		vec.X * mat.M[4] + vec.Y * mat.M[5] + vec.Z * mat.M[6],
		vec.X * mat.M[8] + vec.Y * mat.M[9] + vec.Z * mat.M[10]);
}

template<typename type>
auto operator * (const TVec4<type>& vec, const TMat4<type>& mat) -> TVec4<type> {
	return TVec4<type>(
		vec.X * mat.M[0] + vec.Y * mat.M[1] + vec.Z * mat.M[2] + vec.W * mat.M[3],
		vec.X * mat.M[4] + vec.Y * mat.M[5] + vec.Z * mat.M[6] + vec.W * mat.M[7],
		vec.X * mat.M[8] + vec.Y * mat.M[9] + vec.Z * mat.M[10] + vec.W * mat.M[11],
		vec.X * mat.M[12] + vec.Y * mat.M[13] + vec.Z * mat.M[14] + vec.W * mat.M[15]);
}
