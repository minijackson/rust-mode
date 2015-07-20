#ifndef RUST_RANGE_MODIFIER_HPP
#define RUST_RANGE_MODIFIER_HPP

#include "basic_range.hpp"

#include <iterator>
#include <type_traits>

namespace rust {

	template<
		class OriginRange,
		class iterator,
		class Category  = typename iterator::iterator_category,
		class T         = typename iterator::value_type,
		class Distance  = std::ptrdiff_t,
		class Pointer   = T*,
		class Reference = T&
	> class RangeModifier : public BasicRange<iterator, Category, T, Distance, Pointer, Reference> {
	public:
		typedef BasicRange<iterator, Category, T, Distance, Pointer, Reference> ParentType;
		typedef RangeModifier<iterator, Category, T, Distance, Pointer, Reference> CurrentType;

		static_assert(std::is_base_of<ParentType, OriginRange>::value,
				"The range must inherit BaseRange");

		RangeModifier(OriginRange origin)
			: ParentType(origin.begin(), origin.end()), origin(origin) {}

		virtual Distance size()  = 0;
		virtual bool     empty() = 0;

		iterator& begin() {
			return origin.begin();
		}

		iterator& end() {
			return origin.end();
		}

	protected:
		OriginRange origin;
	};

}

#endif
