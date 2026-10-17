#pragma once
#include <cstdint>

#include "T_ObjectHeader.h"
#include "T_Policy.h"
#include "T_BaseVariantObject.h"

namespace NS::LightVariant {

	template<detail::Concept::C_IsHeader TyHeader, detail::Concept::C_IsPolicy TyPolicy>
	class T_VariantObject {
	public:
		using HeaderType = TyHeader;
		using PolicyType = TyPolicy;

	private:
		union {
			HeaderType header; // ランタイムデータ
			int8_t _reserved_data_field[PolicyType::getObjectSize()];
		};

	public:
		T_VariantObject() = default;
		
		template<detail::Concept::C_IsVariantObject Ty>
		requires std::is_same_v<HeaderType, typename Ty::HeaderType> && std::is_same_v<PolicyType, typename Ty::PolicyType>
		T_VariantObject(const Ty& ty) {
			std::memcpy(this, &ty, sizeof(Ty));
		}

	public:
		const HeaderType& getHeader() const noexcept { return header; }

	public:
		template<detail::Concept::C_IsVariantObject Ty>
		requires std::is_same_v<HeaderType, typename Ty::HeaderType>&& std::is_same_v<PolicyType, typename Ty::PolicyType>
		Ty& force_cast() {
			return *reinterpret_cast<Ty*>(this);
		}

		template<detail::Concept::C_IsVariantObject Ty>
		requires std::is_same_v<HeaderType, typename Ty::HeaderType>&& std::is_same_v<PolicyType, typename Ty::PolicyType>
		const Ty& force_cast() const {
			return *reinterpret_cast<const Ty*>(this);
		}

	public:
		template<detail::Concept::C_IsVariantObject Ty>
		requires std::is_same_v<HeaderType, typename Ty::HeaderType>&& std::is_same_v<PolicyType, typename Ty::PolicyType>
		Ty* try_cast() {
			if (Ty::_header.getTag() != header.getTag()) return nullptr;
			return reinterpret_cast<Ty*>(this);
		}

		template<detail::Concept::C_IsVariantObject Ty>
		requires std::is_same_v<HeaderType, typename Ty::HeaderType>&& std::is_same_v<PolicyType, typename Ty::PolicyType>
		const Ty* try_cast() const {
			if (Ty::_header.getTag() != header.getTag()) return nullptr;
			return reinterpret_cast<const Ty*>(this);
		}

	};

}