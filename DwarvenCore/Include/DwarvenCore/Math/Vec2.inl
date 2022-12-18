#pragma once

template<typename type>
TVec2<type>::TVec2()
	: X(type(0))
	, Y(type(0)) {
}


template<typename type>
TVec2<type>::TVec2(type x, type y)
	: X(x)
	, Y(y) {
}

template<typename type>
template<typename type2>
TVec2<type>::TVec2(type2 x, type2 y)
	: X(type(x))
	, Y(type(y)) {
}

template<typename type>
template<typename type2>
TVec2<type>::TVec2(const TVec2<type2>& xy)
	: X(type(xy.X))
	, Y(type(xy.Y)) {
}

template<typename type>
void TVec2<type>::Normalize() {
	const type length = TVec2<type>::Length();
	X /= (length < type(0.000001)) ? type(1) : length;
	Y /= (length < type(0.000001)) ? type(1) : length;
}

template<typename type>
auto TVec2<type>::LengthSqr() const -> type {
	return X * X + Y * Y;
}

template<typename type>
auto TVec2<type>::Length() const -> type {
	return Sqrt<type>(X * X + Y * Y);
}

template<typename type>
auto TVec2<type>::LengthSqr(const TVec2<type>& vec) -> type {
	return vec.X * vec.X + vec.Y * vec.Y;
}

template<typename type>
auto TVec2<type>::Length(const TVec2<type>& vec) -> type {
	return Sqrt<type>(vec.X * vec.X + vec.Y * vec.Y);
}

template<typename type>
auto TVec2<type>::Normalized(const TVec2<type>& vec) -> TVec2<type> {
	const type length = TVec2<type>::Length(vec);
	return (length < type(0.000001)) ? TVec2<type>(type(0), type(0)) : TVec2<type>(vec.X / length, vec.Y / length);
}

template<typename type>
auto TVec2<type>::Dot(const TVec2<type>& left, const TVec2<type>& right) -> type {
	return left.X * right.X + left.Y * right.Y;
}

template<typename type>
auto TVec2<type>::Cross(const TVec2<type>& left, const TVec2<type>& right) -> type {
	return left.X * right.Y - left.Y * right.X;
}

template<typename type>
auto TVec2<type>::Angle(const TVec2<type>& left, const TVec2<type>& right) -> type {
	const type cr = type(TVec2<type>::Cross(left, right) > type(0)) * type(2) - type(1);
	return Acos<type>(TVec2<type>::Dot(left, right)) * cr;
}

template<typename type>
auto TVec2<type>::Rotated(const TVec2<type>& vec, type angle) -> TVec2<type> {
	const type c = Cos<type>(angle);
	const type s = Sin<type>(angle);
	return TVec2<type>(vec.X * c - vec.Y * s, vec.X * s + vec.Y * c);
}




template<typename type>
bool operator == (const TVec2<type>& left, const TVec2<type>& right) {
	return (left.X == right.X) && (left.Y == right.Y);
}

template<typename type>
bool operator != (const TVec2<type>& left, const TVec2<type>& right) {
	return (left.X != right.X) || (left.Y != right.Y);
}

template<typename type>
auto operator + (const TVec2<type>& left, const TVec2<type>& right) -> TVec2<type> {
	return TVec2<type>(left.X + right.X, left.Y + right.Y);
}

template<typename type>
auto operator - (const TVec2<type>& left, const TVec2<type>& right) -> TVec2<type> {
	return TVec2<type>(left.X - right.X, left.Y - right.Y);
}

template<typename type>
auto operator * (type left, const TVec2<type> right) -> TVec2<type> {
	return TVec2<type>(left * right.X, left * right.Y);
}

template<typename type>
auto operator * (const TVec2<type>& left, type right) -> TVec2<type> {
	return TVec2<type>(left.X * right, left.Y * right);
}

template<typename type>
auto operator * (const TVec2<type>& left, const TVec2<type> right) -> TVec2<type> {
	return TVec2<type>(left.X * right.X, left.Y * right.Y);
}

template<typename type>
auto operator / (const TVec2<type>& left, type right) -> TVec2<type> {
	return TVec2<type>(left.X / right, left.Y / right);
}

template<typename type>
auto operator += (TVec2<type>& left, const TVec2<type>& right) -> const TVec2<type>& {
	left.X += right.X;
	left.Y += right.Y;
	return left;
}

template<typename type>
auto operator -= (TVec2<type>& left, const TVec2<type>& right) -> const TVec2<type>& {
	left.X -= right.X;
	left.Y -= right.Y;
	return left;
}

template<typename type>
auto operator *= (TVec2<type>& left, type right)-> const TVec2<type>& {
	left.X *= right;
	left.Y *= right;
	return left;
}

template<typename type>
auto operator /= (TVec2<type>& left, type right) -> const TVec2<type>& {
	left.X /= right;
	left.Y /= right;
	return left;
}