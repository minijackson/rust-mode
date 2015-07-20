#ifndef RUST_CYCLED_RANGE_HPP
#define RUST_CYCLED_RANGE_HPP

#include "range_modifier.hpp"
#include "basic_range.hpp"

#include "all_range_modifiers.hpp"

namespace rust {

	template<
		class OriginRange,
		class iterator,
		class Category  = typename iterator::iterator_category,
		class T         = typename iterator::value_type,
		class Distance  = std::ptrdiff_t,
		class Pointer   = T*,
		class Reference = T&
	> class CycledRange : public RangeModifier<OriginRange, iterator, Category, T, Distance, Pointer, Reference> {

		typedef CycledRange<OriginRange, iterator, Category, T, Distance, Pointer, Reference> CurrentType;
		typedef RangeModifier<OriginRange, iterator, Category, T, Distance, Pointer, Reference> ParentType;

	public:
		CycledRange(OriginRange range)
			: ParentType(range), backup(range) {
			this->noEnd = true;
		}

		virtual Distance size() {
			throw InfiniteRangeException();
		}

		virtual bool empty() {
			return this->origin.empty();
		}

		RANGE_MODIFIERS

		template<typename Container>
		Container collect() {
			throw InfiniteRangeException();
		}

		CurrentType& operator++() {
			++this->origin;
			if(this->origin.begin() == this->origin.end()) {
				this->origin = backup;
			}
			return *this;
		}

		CurrentType& operator++(int) {
			CurrentType other = *this;
			++this->origin;
			if(this->origin.begin() == this->origin.end()) {
				this->origin = backup;
			}
			return other;
		}

		T beginValue() {
			return this->origin.beginValue();
		}

	private:
		OriginRange backup;
	};

}

#endif
