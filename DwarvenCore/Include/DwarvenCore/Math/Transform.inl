#pragma once

template<typename type>
TTransform<type>::TTransform() {
	m_Transform[0] = TVec4<type>(1.0f, 0.0f, 0.0f, 0.0f);
	m_Transform[1] = TVec4<type>(0.0f, 1.0f, 0.0f, 0.0f);
	m_Transform[2] = TVec4<type>(0.0f, 0.0f, 1.0f, 0.0f);
}

template<typename type>
TTransform<type>::TTransform(const TMat4<type>& transform) {
	m_Transform[0] = TVec4<type>(transform.M[0], transform.M[1], transform.M[2], transform.M[3]);
	m_Transform[1] = TVec4<type>(transform.M[4], transform.M[5], transform.M[6], transform.M[7]);
	m_Transform[2] = TVec4<type>(transform.M[8], transform.M[9], transform.M[10], transform.M[11]);
}

template<typename type>
TTransform<type>::TTransform(const TVec3<type>& position) {
	m_Transform[0] = TVec4<type>(1.0f, 0.0f, 0.0f, position.X);
	m_Transform[1] = TVec4<type>(0.0f, 1.0f, 0.0f, position.Y);
	m_Transform[2] = TVec4<type>(0.0f, 0.0f, 1.0f, position.Z);
}

template<typename type>
TTransform<type>::TTransform(const TVec3<type>& position, const TMat3<type>& orientation) {
	m_Transform[0] = TVec4<type>(orientation.M[0], orientation.M[1], orientation.M[2], position.X);
	m_Transform[1] = TVec4<type>(orientation.M[3], orientation.M[4], orientation.M[5], position.Y);
	m_Transform[2] = TVec4<type>(orientation.M[6], orientation.M[7], orientation.M[8], position.Z);
}

template<typename type>
auto TTransform<type>::GetPosition() const->TVec3<type> {
	return TVec3<type>(m_Transform[0].W, m_Transform[1].W, m_Transform[2].W);
}

template<typename type>
auto TTransform<type>::GetOrientation() const->TMat3<type> {
	return TMat3<type>(
		m_Transform[0].X, m_Transform[0].Y, m_Transform[0].Z,
		m_Transform[1].X, m_Transform[1].Y, m_Transform[1].Z,
		m_Transform[2].X, m_Transform[2].Y, m_Transform[2].Z
	);
}

template<typename type>
TTransform<type>::operator TMat4<type>() const {
	return TMat4<type>(
		m_Transform[0].X, m_Transform[0].Y, m_Transform[0].Z, m_Transform[0].W,
		m_Transform[1].X, m_Transform[1].Y, m_Transform[1].Z, m_Transform[1].W,
		m_Transform[2].X, m_Transform[2].Y, m_Transform[2].Z, m_Transform[2].W,
		type(0), type(0), type(0), type(1)
	);
}