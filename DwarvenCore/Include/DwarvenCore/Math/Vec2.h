#pragma once

template<typename type>
struct TVec2 {
	TVec2();
	TVec2(type x, type y);

	template<typename type2>
	TVec2(type2 x, type2 y);

	template<typename type2>
	TVec2(const TVec2<type2>& xy);

	void Normalize();
	auto LengthSqr() const->type;
	auto Length() const->type;

	static auto LengthSqr(const TVec2<type>& vec)->type;
	static auto Length(const TVec2<type>& vec)->type;
	static auto Normalized(const TVec2<type>& vec)->TVec2<type>;
	static auto Dot(const TVec2<type>& left, const TVec2<type>& right)->type;
	static auto Cross(const TVec2<type>& left, const TVec2<type>& right)->type;
	static auto Angle(const TVec2<type>& left, const TVec2<type>& right)->type;
	static auto Rotated(const TVec2<type>& vec, type angle)->TVec2<type>;

	type	X;
	type	Y;
};

template<typename type>
bool operator == (const TVec2<type>& left, const TVec2<type>& right);

template<typename type>
bool operator != (const TVec2<type>& left, const TVec2<type>& right);

template<typename type>
auto operator + (const TVec2<type>& left, const TVec2<type>& right)->TVec2<type>;

template<typename type>
auto operator - (const TVec2<type>& left, const TVec2<type>& right)->TVec2<type>;

template<typename type>
auto operator * (type left, const TVec2<type> right)->TVec2<type>;

template<typename type>
auto operator * (const TVec2<type>& left, type right)->TVec2<type>;

template<typename type>
auto operator * (const TVec2<type>& left, const TVec2<type> right)->TVec2<type>;

template<typename type>
auto operator / (const TVec2<type>& left, type right)->TVec2<type>;

template<typename type>
auto operator += (TVec2<type>& left, const TVec2<type>& right) -> const TVec2<type>&;

template<typename type>
auto operator -= (TVec2<type>& left, const TVec2<type>& right) -> const TVec2<type>&;

template<typename type>
auto operator *= (TVec2<type>& left, type right)-> const TVec2<type>&;

template<typename type>
auto operator /= (TVec2<type>& left, type right) -> const TVec2<type>&;

#include <DwarvenCore/Math/Vec2.inl>