#pragma once

template<typename type>
struct TTransform {
	TTransform();
	TTransform(const TMat4<type>& transform);
	TTransform(const TVec3<type>& position);
	TTransform(const TVec3<type>& position, const TMat3<type>& orientation);
	TTransform(const TTransform&) = default;
	TTransform(TTransform&&) = default;
	TTransform& operator = (const TTransform&) = default;
	TTransform& operator = (TTransform&&) = default;

	auto GetPosition() const->TVec3<type>;
	auto GetOrientation() const->TMat3<type>;

	operator TMat4<type>() const;

	TVec4<type> m_Transform[3];
};

#include <DwarvenCore/Math/Transform.inl>