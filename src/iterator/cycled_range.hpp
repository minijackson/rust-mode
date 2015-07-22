#ifndef RUST_CYCLED_RANGE_HPP
#define RUST_CYCLED_RANGE_HPP

#include "range_modifier.hpp"
#include "basic_range.hpp"

#include "all_range_modifiers.hpp"

namespace rust {

	template<
		class OriginRange,
		class T         = typename OriginRange::value_type,
		class Distance  = std::ptrdiff_t,
		class Pointer   = T*,
		class Reference = T&
	> class CycledRange : public RangeModifier<OriginRange, T, Distance, Pointer, Reference> {

		typedef CycledRange<OriginRange, T, Distance, Pointer, Reference> CurrentType;
		typedef RangeModifier<OriginRange, T, Distance, Pointer, Reference> ParentType;

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
			advance();
			return *this;
		}

		CurrentType& operator++(int) {
			CurrentType other = *this;
			advance();
			return other;
		}

		T beginValue() {
			return this->origin.beginValue();
		}

	private:
		OriginRange backup;

		void advance() {
			++this->origin;
			if(this->origin.begin() == this->origin.end()) {
				this->origin = backup;
			}
		}
	};

}

#endif
