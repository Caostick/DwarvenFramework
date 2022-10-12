#pragma once

template<typename type>
struct TVec4 {
    TVec4();
    TVec4(const TVec3<type>& xyz, type w);
    TVec4(type x, const TVec3<type>& yzw);
    TVec4(const TVec2<type>& xy, const TVec2<type>& zw);
    TVec4(const TVec2<type>& xy, type z, type w);
    TVec4(type x, const TVec2<type>& yz, type w);
    TVec4(type x, type y, const TVec2<type>& zw);
    TVec4(type x, type y, type z, type w);

    template<typename type2>
    TVec4(const TVec4<type2>& vec);

	type X;
	type Y;
	type Z;
	type W;
};

template<typename type>
bool operator == (const TVec4<type>& left, const TVec4<type>& right);

template<typename type>
bool operator != (const TVec4<type>& left, const TVec4<type>& right);

template<typename type>
auto operator + (const TVec4<type>& left, const TVec4<type>& right)->TVec4<type>;

template<typename type>
auto operator - (const TVec4<type>& a_Vector)->TVec4<type>;

template<typename type>
auto operator - (const TVec4<type>& left, const TVec4<type>& right)->TVec4<type>;

template<typename type>
auto operator * (type left, const TVec4<type>& right)->TVec4<type>;

template<typename type>
auto operator * (const TVec4<type>& left, type right)->TVec4<type>;

template<typename type>
auto operator * (const TVec4<type>& left, const TVec4<type>& right)->TVec4<type>;

template<typename type>
auto operator / (const TVec4<type>& left, type right)->TVec4<type>;

template<typename type>
auto operator += (TVec4<type>& left, const TVec4<type>& right) -> const TVec4<type>&;

template<typename type>
auto operator -= (TVec4<type>& left, const TVec4<type>& right) -> const TVec4<type>&;

template<typename type>
auto operator *= (TVec4<type>& left, type right) -> const TVec4<type>&;

template<typename type>
auto operator /= (TVec4<type>& left, type right) -> const TVec4<type>&;

#include <DwarvenCore/Math/Vec4.inl>