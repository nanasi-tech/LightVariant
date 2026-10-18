#pragma once
#include "T_Policy.h"
#include "T_ObjectHeader.h"

namespace NS::LightVariant {

	/// このクラスを継承したクラスはLightVariantのデータとして使用することができます。
	/// ただし、trivially copyableである必要があります。
	/// TyPolicy: グループの識別や、最大サイズなどを保持しておくものです。
	/// TyHeader: ヘッダーのタイプを指定します。
	/// TyHeader static_header: このクラスの識別ヘッダーを指定します。
	template<detail::Concept::C_IsHeader TyHeader, detail::Concept::C_IsPolicy TyPolicy, TyHeader static_header>
	class T_BaseVariantObject {
	public:
		using HeaderType = TyHeader;
		using PolicyType = TyPolicy;

	public:
		// 静的な識別用データ
		static constexpr HeaderType _header = static_header;

	protected:
		// ランタイムでの識別用データ
		HeaderType header = _header;

	public:
		const HeaderType& getHeader() const noexcept { return header; }

	};

	namespace detail::Concept {

		// T_VariantObjectとして使えるかどうか
		template<typename T>
		concept C_IsVariantObject =
			std::is_trivially_copyable_v<T> &&
			std::is_base_of_v<T_BaseVariantObject<typename T::HeaderType, typename T::PolicyType, T::_header>, T> &&
			(alignof(T) <= T::PolicyType::getAlign()) &&
			(sizeof(T) <= T::PolicyType::getObjectSize());

	} // namespace Concept

}