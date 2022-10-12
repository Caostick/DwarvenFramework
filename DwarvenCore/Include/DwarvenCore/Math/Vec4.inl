#pragma once

template<typename type>
TVec4<type>::TVec4()
	: X(type(0))
	, Y(type(0))
	, Z(type(0))
	, W(type(0)) {
}

template<typename type>
TVec4<type>::TVec4(const TVec3<type>& xyz, type w)
	: X(xyz.X)
	, Y(xyz.Y)
	, Z(xyz.Z)
	, W(w) {
}

template<typename type>
TVec4<type>::TVec4(type x, const TVec3<type>& yzw)
	: X(x)
	, Y(yzw.X)
	, Z(yzw.Y)
	, W(yzw.Z) {
}

template<typename type>
TVec4<type>::TVec4(const TVec2<type>& xy, const TVec2<type>& zw)
	: X(xy.X)
	, Y(xy.Y)
	, Z(zw.X)
	, W(zw.Y) {
}

template<typename type>
TVec4<type>::TVec4(const TVec2<type>& xy, type z, type w) 
	: X(xy.X)
	, Y(xy.Y)
	, Z(z)
	, W(w) {
}

template<typename type>
TVec4<type>::TVec4(type x, const TVec2<type>& yz, type w)
	: X(x)
	, Y(yz.X)
	, Z(yz.Y)
	, W(w) {
}

template<typename type>
TVec4<type>::TVec4(type x, type y, const TVec2<type>& zw) 
	: X(x)
	, Y(y)
	, Z(zw.X)
	, W(zw.Y) {
}

template<typename type>
TVec4<type>::TVec4(type x, type y, type z, type w) 
	: X(x)
	, Y(y)
	, Z(z)
	, W(w) {
}

template<typename type>
template<typename type2>
TVec4<type>::TVec4(const TVec4<type2>& vec)
	: X(type(vec.X))
	, Y(type(vec.Y))
	, Z(type(vec.Z))
	, W(type(vec.W)) {
}






template<typename type>
bool operator == (const TVec4<type>& left, const TVec4<type>& right) {
	return (left.X == right.X) && (left.Y == right.Y) && (left.Z == right.Z) && (left.W == right.W);
}

template<typename type>
bool operator != (const TVec4<type>& left, const TVec4<type>& right) {
	return (left.X != right.X) || (left.Y != right.Y) || (left.Z != right.Z) || (left.W != right.W);
}

template<typename type>
auto operator + (const TVec4<type>& left, const TVec4<type>& right) -> TVec4<type> {
	return TVec4<type>(left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W);
}

template<typename type>
auto operator - (const TVec4<type>& a_Vector) -> TVec4<type> {
	return TVec4<type>(-a_Vector.X, -a_Vector.Y, -a_Vector.Z, -a_Vector.W);
}

template<typename type>
auto operator - (const TVec4<type>& left, const TVec4<type>& right) -> TVec4<type> {
	return TVec4<type>(left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W);
}

template<typename type>
auto operator * (type left, const TVec4<type>& right) -> TVec4<type> {
	return TVec4<type>(left * right.X, left * right.Y, left * right.Z, left * right.W);
}

template<typename type>
auto operator * (const TVec4<type>& left, type right) -> TVec4<type> {
	return TVec4<type>(left.X * right, left.Y * right, left.Z * right, left.W * right);
}

template<typename type>
auto operator * (const TVec4<type>& left, const TVec4<type>& right) -> TVec4<type> {
	return TVec4<type>(left.X * right.X, left.Y * right.Y, left.Z * right.Z, left.W * right.W);
}

template<typename type>
auto operator / (const TVec4<type>& left, type right) -> TVec4<type> {
	return TVec4<type>(left.X / right, left.Y / right, left.Z / right, left.W / right);
}

template<typename type>
auto operator += (TVec4<type>& left, const TVec4<type>& right) -> const TVec4<type>& {
	left.X += right.X;
	left.Y += right.Y;
	left.Z += right.Z;
	left.W += right.W;
	return left;
}

template<typename type>
auto operator -= (TVec4<type>& left, const TVec4<type>& right) -> const TVec4<type>& {
	left.X -= right.X;
	left.Y -= right.Y;
	left.Z -= right.Z;
	left.W -= right.W;
	return left;
}

template<typename type>
auto operator *= (TVec4<type>& left, type right) -> const TVec4<type>& {
	left.X *= right;
	left.Y *= right;
	left.Z *= right;
	left.W *= right;
	return left;
}

template<typename type>
auto operator /= (TVec4<type>& left, type right) -> const TVec4<type>& {
	left.X /= right;
	left.Y /= right;
	left.Z /= right;
	left.W /= right;
	return left;
}