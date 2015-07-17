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
			: ParentType(range), cyclingRange(range) {
			this->noEnd = true;
		}

		virtual Distance size() {
			throw InfiniteRangeException();
		}

		virtual bool empty() {
			return this->origin.empty();
		}

		CurrentType take(size_t count) {
			specifiedCount = true;
			this->noEnd = false;
			this->count = count;
			return *this;
		}

		FilteredRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>
		filter(std::function<bool(T)> predicate) {
			return FilteredRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>(*this, predicate);
		}

		CycledRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>
		cycle() {
			return CycledRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>(*this);
		}

		template<typename Container>
		Container collect() {
			if(this->noEnd) {
				throw InfiniteRangeException();
			} else {
				Container result(count);
				for(auto& value : result) {
					value = *cyclingRange.begin();
					++(*this);
				}
				return result;
			}
		}

		CurrentType& operator++() {
			++progress;
			if(specifiedCount && progress > count) {
				ended = true;
			} else {
				++cyclingRange;
				if(cyclingRange.begin() == this->origin.end()) {
					cyclingRange = this->origin;
				}
			}
			return *this;
		}

		CurrentType& operator++(int) {
			CurrentType other = *this;
			++progress;
			if(specifiedCount && progress > count) {
				ended = true;
			} else {
				++cyclingRange;
				if(cyclingRange.begin() == this->origin.end()) {
					cyclingRange.begin() = this->origin.begin();
				}
			}
			return other;
		}

		iterator begin() {
			if(ended) {
				return this->origin.end();
			} else {
				return cyclingRange.begin();
			}
		}

	private:
		bool specifiedCount = false;
		bool ended = false;
		size_t count;
		size_t progress = 0;
		OriginRange cyclingRange;
	};

}

#endif
