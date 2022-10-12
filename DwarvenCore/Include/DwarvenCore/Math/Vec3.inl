#pragma once

template<typename type>
TVec3<type>::TVec3()
	: X(type(0))
	, Y(type(0))
	, Z(type(0)) {
}

template<typename type>
TVec3<type>::TVec3(type value) 
	: X(value)
	, Y(value)
	, Z(value) {
}

template<typename type>
TVec3<type>::TVec3(type x, type y, type z)
	: X(x)
	, Y(y)
	, Z(z) {
}

template<typename type>
TVec3<type>::TVec3(const TVec3<type>& vec) 
	: X(vec.X)
	, Y(vec.Y)
	, Z(vec.Z) {
}

template<typename type>
TVec3<type>::TVec3(const TVec2<type>& xy, type z) 
	: X(xy.X)
	, Y(xy.Y)
	, Z(z) {
}

template<typename type>
TVec3<type>::TVec3(type x, const TVec2<type>& yz) 
	: X(x)
	, Y(yz.X)
	, Z(yz.Y) {
}

template<typename type>
template<typename type2>
TVec3<type>::TVec3(const TVec3<type2>& vec) 
	: X(type(vec.X))
	, Y(type(vec.Y))
	, Z(type(vec.Z)) {
}

template<typename type>
auto TVec3<type>::LengthSqr() const -> type {
	return X * X + Y * Y + Z * Z;
}

template<typename type>
auto TVec3<type>::Length() const -> type {
	return sqrt(X * X + Y * Y + Z * Z);
}

template<typename type>
void TVec3<type>::Normalize() {
	const type length = Length();
	X /= (length < type(0.000001)) ? type(1) : length;
	Y /= (length < type(0.000001)) ? type(1) : length;
	Z /= (length < type(0.000001)) ? type(1) : length;
}

template<typename type>
auto TVec3<type>::LengthSqr(const TVec3<type>& vec) -> type {
	return vec.X * vec.X + vec.Y * vec.Y + vec.Z * vec.Z;
}

template<typename type>
auto TVec3<type>::Length(const TVec3<type>& vec) -> type {
	return sqrt(vec.X * vec.X + vec.Y * vec.Y + vec.Z * vec.Z);
}

template<typename type>
auto TVec3<type>::Normalized(const TVec3<type>& vec) -> TVec3<type> {
	const type length = Vec3::Length(vec);
	return (length < type(0.000001)) ? TVec3<type>(type(0), type(0), type(0)) : TVec3<type>(vec.X / length, vec.Y / length, vec.Z / length);
}

template<typename type>
auto TVec3<type>::Dot(const TVec3<type>& left, const TVec3<type>& right) -> type {
	return left.X * right.X + left.Y * right.Y + left.Z * right.Z;
}

template<typename type>
auto TVec3<type>::Cross(const TVec3<type>& left, const TVec3<type>& right) -> TVec3<type> {
	return TVec3<type>(left.Y * right.Z - left.Z * right.Y, left.Z * right.X - left.X * right.Z, left.X * right.Y - left.Y * right.X);
}





template<typename type>
bool operator == (const TVec3<type>& left, const TVec3<type>& right) {
	return (left.X == right.X) && (left.Y == right.Y) && (left.Z == right.Z);
}

template<typename type>
bool operator != (const TVec3<type>& left, const TVec3<type>& right) {
	return (left.X != right.X) || (left.Y != right.Y) || (left.Z != right.Z);
}

template<typename type>
auto operator + (const TVec3<type>& left, const TVec3<type>& right) -> TVec3<type> {
	return TVec3<type>(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
}

template<typename type>
auto operator - (const TVec3<type>& vec) -> TVec3<type> {
	return TVec3<type>(-vec.X, -vec.Y, -vec.Z);
}

template<typename type>
auto operator - (const TVec3<type>& left, const TVec3<type>& right) -> TVec3<type> {
	return TVec3<type>(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
}

template<typename type>
auto operator * (type left, const TVec3<type>& right) -> TVec3<type> {
	return TVec3<type>(left * right.X, left * right.Y, left * right.Z);
}

template<typename type>
auto operator * (const TVec3<type>& left, type right) -> TVec3<type> {
	return TVec3<type>(left.X * right, left.Y * right, left.Z * right);
}

template<typename type>
auto operator * (const TVec3<type>& left, const TVec3<type>& right) -> TVec3<type> {
	return TVec3<type>(left.X * right.X, left.Y * right.Y, left.Z * right.Z);
}

template<typename type>
auto operator / (const TVec3<type>& left, type right) -> TVec3<type> {
	return TVec3<type>(left.X / right, left.Y / right, left.Z / right);
}

template<typename type>
auto operator / (const TVec3<type>& left, const TVec3<type>& right)->TVec3<type> {
	return TVec3<type>(left.X / right.X, left.Y / right.Y, left.Z / right.Z);
}

template<typename type>
auto operator += (TVec3<type>& left, const TVec3<type>& right) -> const TVec3<type>& {
	left.X += right.X;
	left.Y += right.Y;
	left.Z += right.Z;
	return left;
}

template<typename type>
auto operator -= (TVec3<type>& left, const TVec3<type>& right) -> const TVec3<type>& {
	left.X -= right.X;
	left.Y -= right.Y;
	left.Z -= right.Z;
	return left;
}

template<typename type>
auto operator *= (TVec3<type>& left, type right) -> const TVec3<type>& {
	left.X *= right;
	left.Y *= right;
	left.Z *= right;
	return left;
}

template<typename type>
auto operator /= (TVec3<type>& left, type right) -> const TVec3<type>& {
	left.X /= right;
	left.Y /= right;
	left.Z /= right;
	return left;
}
