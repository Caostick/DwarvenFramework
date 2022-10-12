#pragma once

template<typename type>
struct TVec3 {
    TVec3();
    TVec3(type value);
    TVec3(type x, type y, type z);
    TVec3(const TVec3<type>& vec);
    TVec3(const TVec2<type>& xy, type z);
    TVec3(type x, const TVec2<type>& yz);

    template<typename type2>
    TVec3(const TVec3<type2>& vec);

    auto LengthSqr() const -> type;
    auto Length() const -> type;
    void Normalize();

    static auto LengthSqr(const TVec3<type>& vec) -> type;
    static auto Length(const TVec3<type>& vec) -> type;
    static auto Normalized(const TVec3<type>& vec) -> TVec3<type>;
    static auto Dot(const TVec3<type>& left, const TVec3<type>& right) -> type;
    static auto Cross(const TVec3<type>& left, const TVec3<type>& right) -> TVec3<type>;

	type	X;
	type	Y;
	type	Z;
};

template<typename type>
bool operator == (const TVec3<type>& left, const TVec3<type>& right);

template<typename type>
bool operator != (const TVec3<type>& left, const TVec3<type>& right);

template<typename type>
auto operator + (const TVec3<type>& left, const TVec3<type>& right)->TVec3<type>;

template<typename type>
auto operator - (const TVec3<type>& vec)->TVec3<type>;

template<typename type>
auto operator - (const TVec3<type>& left, const TVec3<type>& right)->TVec3<type>;

template<typename type>
auto operator * (type left, const TVec3<type>& right)->TVec3<type>;

template<typename type>
auto operator * (const TVec3<type>& left, type right)->TVec3<type>;

template<typename type>
auto operator * (const TVec3<type>& left, const TVec3<type>& right)->TVec3<type>;

template<typename type>
auto operator / (const TVec3<type>& left, type right)->TVec3<type>;

template<typename type>
auto operator / (const TVec3<type>& left, const TVec3<type>& right)->TVec3<type>;

template<typename type>
auto operator += (TVec3<type>& left, const TVec3<type>& right) -> const TVec3<type>&;

template<typename type>
auto operator -= (TVec3<type>& left, const TVec3<type>& right) -> const TVec3<type>&;

template<typename type>
auto operator *= (TVec3<type>& left, type right) -> const TVec3<type>&;

template<typename type>
auto operator /= (TVec3<type>& left, type right) -> const TVec3<type>&;

#include <DwarvenCore/Math/Vec3.inl>