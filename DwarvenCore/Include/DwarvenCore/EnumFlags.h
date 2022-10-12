#pragma once

#include <type_traits>

namespace df {
	template <typename TEnum>
	class EnumFlags {
	public:
		static_assert(std::is_enum<TEnum>::value, "EnumFlags should be enum type");

		using EnumType = TEnum;
		using DataType = std::underlying_type_t<TEnum>;

	public:
		constexpr EnumFlags();
		constexpr EnumFlags(const TEnum value);
		constexpr EnumFlags(const EnumFlags& other) = default;

		constexpr void Raise(const TEnum flag);
		constexpr void Clear(const TEnum flag);
		constexpr void Toggle(const TEnum flag);

		constexpr void Set(const TEnum flag, const bool value);

		constexpr bool Has(const TEnum flag) const;

		constexpr bool HasAny() const;
		constexpr bool HasNone() const;

		constexpr bool HasNone(const EnumFlags flags) const;
		constexpr bool HasAny(const EnumFlags flags) const;
		constexpr bool HasAll(const EnumFlags flags) const;

		constexpr void ClearAll();
		constexpr void RaiseAll();

		constexpr explicit operator DataType() const;

		constexpr explicit operator bool() const;
		constexpr bool operator!() const;

		constexpr auto operator~() const->EnumFlags;

		constexpr auto operator-(const EnumFlags other) const->EnumFlags;

		constexpr auto operator&(const EnumFlags other) const->EnumFlags;
		constexpr auto operator|(const EnumFlags other) const->EnumFlags;
		constexpr auto operator^(const EnumFlags other) const->EnumFlags;

		constexpr auto operator&=(const EnumFlags other)->EnumFlags&;
		constexpr auto operator|=(const EnumFlags other)->EnumFlags&;
		constexpr auto operator^=(const EnumFlags other)->EnumFlags&;

		constexpr auto operator=(const TEnum value)->EnumFlags&;
		constexpr auto operator=(const EnumFlags& other)->EnumFlags & = default;

		constexpr bool operator==(const EnumFlags& other) const;
		constexpr bool operator!=(const EnumFlags& other) const;

	private:
		explicit constexpr EnumFlags(const DataType data);

	private:
		DataType m_Data;
	};

	template <typename T>
	static constexpr bool v_IsEnumFlags = false;
	template <typename T>
	static constexpr bool v_IsEnumFlags<EnumFlags<T>> = true;
}

#define DF_DEFINE_ENUM_FLAG_OPERATORS(Flags) \
    constexpr inline Flags operator|(const typename Flags::EnumType a, const typename Flags::EnumType b) { return Flags(a) | Flags(b); } \
    constexpr inline Flags operator&(const typename Flags::EnumType a, const typename Flags::EnumType b) { return Flags(a) & Flags(b); } \
    constexpr inline Flags operator^(const typename Flags::EnumType a, const typename Flags::EnumType b) { return Flags(a) ^ Flags(b); }

#include <DwarvenCore/EnumFlags.inl>