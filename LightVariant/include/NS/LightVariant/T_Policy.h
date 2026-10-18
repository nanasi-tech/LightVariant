#pragma once
#include <cstdint>
#include <concepts>
#include <type_traits>

namespace NS::LightVariant {

	template<size_t _obj_size, typename Type, size_t _align = 8>
	class T_Policy {
	public:
		using _Type = Type;

	public:
		static constexpr size_t getObjectSize() noexcept { return _obj_size; }
		static constexpr size_t getAlign() noexcept { return _align; }

	};

	namespace detail::Concept {

		template<typename T>
		struct c_is_policy : std::false_type {};

		template<size_t _obj_size, typename U, size_t _align>
		struct c_is_policy<T_Policy<_obj_size, U, _align>> : std::true_type {};

		// T_Policy‚Å‚ ‚é‚©‚Ç‚¤‚©
		template<typename T>
		concept C_IsPolicy = c_is_policy<T>::value;

	}

}