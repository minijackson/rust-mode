#ifndef RUST_RANGE_MODIFIER_HPP
#define RUST_RANGE_MODIFIER_HPP

#include "basic_range.hpp"

#include <iterator>
#include <type_traits>

namespace rust {

	template<
		class OriginRange,
		class T         = typename OriginRange::value_type,
		class Distance  = std::ptrdiff_t,
		class Pointer   = T*,
		class Reference = T&
	> class RangeModifier : public BasicRange<typename OriginRange::iterator, T, Distance, Pointer, Reference> {

		typedef BasicRange<typename OriginRange::iterator, T, Distance, Pointer, Reference> ParentType;
		typedef RangeModifier<OriginRange, T, Distance, Pointer, Reference> CurrentType;

	public:
		typedef typename OriginRange::iterator iterator;
		typedef T                              value_type;

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
