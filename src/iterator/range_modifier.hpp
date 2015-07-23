#ifndef RUST_RANGE_MODIFIER_HPP
#define RUST_RANGE_MODIFIER_HPP

#include "basic_range.hpp"

#include <iterator>
#include <type_traits>

namespace rust {

	template<class OriginRange>
	class RangeModifier : public BasicRange<typename OriginRange::iterator> {

		typedef BasicRange<typename OriginRange::iterator> ParentType;
		typedef RangeModifier<OriginRange>                 CurrentType;

	public:
		typedef typename OriginRange::iterator        iterator;
		typedef typename OriginRange::value_type      value_type;
		typedef typename OriginRange::difference_type difference_type;

		RangeModifier(OriginRange origin)
			: ParentType(origin.begin(), origin.end()), origin(origin) {}

		virtual difference_type size()  = 0;
		virtual bool            empty() = 0;

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
