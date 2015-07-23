#ifndef RUST_CYCLED_RANGE_HPP
#define RUST_CYCLED_RANGE_HPP

#include "range_modifier.hpp"
#include "all_range_modifiers.hpp"

namespace rust {

	template<class OriginRange> 
	class CycledRange : public RangeModifier<OriginRange> {

		typedef CycledRange<OriginRange>   CurrentType;
		typedef RangeModifier<OriginRange> ParentType;

	public:
		CycledRange(OriginRange range)
			: ParentType(range, false), backup(range) {
			this->noEnd = true;
		}

		virtual typename CurrentType::difference_type size() {
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

		typename CurrentType::value_type currentValue() {
			return this->origin.currentValue();
		}

	private:
		OriginRange backup;

		void advance() {
			++this->origin;
			if(this->origin.hasEnded()) {
				this->origin = backup;
			}
		}
	};

}

#endif
