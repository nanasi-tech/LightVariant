#pragma once
#include <concepts>
#include <type_traits>

namespace NS::LightVariant {
	namespace detail::Concept {

		// Tagとして使用できる型かどうか
		template<typename T>
		concept C_TagType = std::integral<T> || std::is_enum_v<T>;

	} // namespace Concept

	template<detail::Concept::C_TagType T>
	class T_ObjectHeader {
	public:
		using TagType = T;

	public:
		TagType type;

	public:
		constexpr T_ObjectHeader() = default;
		constexpr explicit T_ObjectHeader(TagType ty) : type(ty) {}

	public:
		constexpr TagType getTag() const noexcept { return type; }

	};

	namespace detail::Concept {

		// 旧式
		//template<typename T>
		//struct c_is_header : std::false_type {};

		//template<typename T>
		//struct c_is_header<T_ObjectHeader<T>> : std::true_type {};

		//// T_ObjectHeaderであるかどうか
		//template<typename T>
		//concept C_IsHeader = c_is_header<T>::value;

		// 独自のヘッダーを定義できるように改造
		// ビルド時にコンストラクタと`getTag`が使用できることが条件
		template<typename T>
		concept C_IsHeader = requires {
			T::type;
			typename T::TagType;
			[]() consteval { T obj{ (typename T::TagType)0 };  obj.getTag(); }();
		};

	}

}