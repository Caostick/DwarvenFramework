#pragma once

#include <limits>

template <typename TEnum>
constexpr df::EnumFlags<TEnum>::EnumFlags()
	: m_Data(0) {}

template <typename TEnum>
constexpr df::EnumFlags<TEnum>::EnumFlags(const TEnum value)
	: m_Data(DataType(value)) {}

template <typename TEnum>
constexpr void df::EnumFlags<TEnum>::Raise(const TEnum flag) {
	m_Data |= DataType(flag);
}

template <typename TEnum>
constexpr void df::EnumFlags<TEnum>::Clear(const TEnum flag) {
	m_Data &= ~DataType(flag);
}

template <typename TEnum>
constexpr void df::EnumFlags<TEnum>::Toggle(const TEnum flag) {
	m_Data ^= DataType(flag);
}

template <typename TEnum>
constexpr void df::EnumFlags<TEnum>::Set(const TEnum flag, const bool value) {
	if (value)
		Raise(flag);
	else
		Clear(flag);
}

template <typename TEnum>
constexpr bool df::EnumFlags<TEnum>::Has(const TEnum flag) const {
	return (m_Data & DataType(flag)) == DataType(flag);
}

template <typename TEnum>
constexpr bool df::EnumFlags<TEnum>::HasAny() const {
	return m_Data != 0;
}

template <typename TEnum>
constexpr bool df::EnumFlags<TEnum>::HasNone() const {
	return m_Data == 0;
}

template <typename TEnum>
constexpr bool df::EnumFlags<TEnum>::HasNone(const EnumFlags flags) const {
	return (m_Data & flags.m_Data) == 0;
}

template <typename TEnum>
constexpr bool df::EnumFlags<TEnum>::HasAny(const EnumFlags flags) const {
	return (m_Data & flags.m_Data) != 0;
}

template <typename TEnum>
constexpr bool df::EnumFlags<TEnum>::HasAll(const EnumFlags flags) const {
	return (m_Data & flags.m_Data) == flags.m_Data;
}

template <typename TEnum>
constexpr void df::EnumFlags<TEnum>::ClearAll() {
	m_Data = 0;
}

template <typename TEnum>
constexpr void df::EnumFlags<TEnum>::RaiseAll() {
	m_Data = std::numeric_limits<DataType>::max();
}

template <typename TEnum>
constexpr df::EnumFlags<TEnum>::operator DataType() const {
	return m_Data;
}

template <typename TEnum>
constexpr df::EnumFlags<TEnum>::operator bool() const {
	return HasAny();
}

template <typename TEnum>
constexpr bool df::EnumFlags<TEnum>::operator!() const {
	return HasNone();
}

template <typename TEnum>
constexpr auto df::EnumFlags<TEnum>::operator~() const -> EnumFlags {
	return EnumFlags(~m_Data);
}

template <typename TEnum>
constexpr auto df::EnumFlags<TEnum>::operator-(const EnumFlags other) const -> EnumFlags {
	return EnumFlags(m_Data & ~other.m_Data);
}

template <typename TEnum>
constexpr auto df::EnumFlags<TEnum>::operator&(const EnumFlags other) const -> EnumFlags {
	return EnumFlags(m_Data & other.m_Data);
}

template <typename TEnum>
constexpr auto df::EnumFlags<TEnum>::operator|(const EnumFlags other) const -> EnumFlags {
	return EnumFlags(m_Data | other.m_Data);
}

template <typename TEnum>
constexpr auto df::EnumFlags<TEnum>::operator^(const EnumFlags other) const -> EnumFlags {
	return EnumFlags(m_Data ^ other.m_Data);
}

template <typename TEnum>
constexpr auto df::EnumFlags<TEnum>::operator&=(const EnumFlags other) -> EnumFlags& {
	m_Data &= other.m_Data;
	return *this;
}

template <typename TEnum>
constexpr auto df::EnumFlags<TEnum>::operator|=(const EnumFlags other) -> EnumFlags& {
	m_Data |= other.m_Data;
	return *this;
}

template <typename TEnum>
constexpr auto df::EnumFlags<TEnum>::operator^=(const EnumFlags other) -> EnumFlags& {
	m_Data ^= other.m_Data;
	return *this;
}

template <typename TEnum>
constexpr auto df::EnumFlags<TEnum>::operator=(const TEnum value) -> EnumFlags& {
	m_Data = DataType(value);
	return *this;
}

template <typename TEnum>
constexpr bool df::EnumFlags<TEnum>::operator==(const EnumFlags& other) const {
	return m_Data == other.m_Data;
}

template <typename TEnum>
constexpr bool df::EnumFlags<TEnum>::operator!=(const EnumFlags& other) const {
	return m_Data != other.m_Data;
}

template <typename TEnum>
constexpr df::EnumFlags<TEnum>::EnumFlags(const DataType data)
	: m_Data(data) {}