#ifndef RUST_FILTERED_RANGE_HPP
#define RUST_FILTERED_RANGE_HPP

#include "range_modifier.hpp"
#include "all_range_modifiers.hpp"

#include <functional>
#include <vector>
#include <algorithm>
#include <type_traits>

namespace rust {

	template<class OriginRange>
	class FilteredRange : public RangeModifier<OriginRange> {

		typedef std::function<bool(typename OriginRange::value_type)> Filter_t;
		typedef FilteredRange<OriginRange> CurrentType;
		typedef RangeModifier<OriginRange> ParentType;

	public:
		FilteredRange(OriginRange range, Filter_t predicate)
			: ParentType(range, range.hasEnd()), predicate(predicate) {
		}

		virtual typename CurrentType::difference_type size() {
			throw UnknownValueException("Cannot know the size of a filtered range before consuming values");
		}

		virtual bool empty() {
			throw UnknownValueException("Cannot know if a filtered range is empty before consuming values");
		}

		RANGE_MODIFIERS

		template<typename Container>
		Container collect() {
			if(!this->origin.hasEnd()) {
				throw InfiniteRangeException();
			} else {
				std::vector<typename CurrentType::value_type> temp;
				auto inserter = std::back_inserter(temp);

				while(!this->hasEnded()) {
					*inserter++ = currentValue();
					++(*this);
				}

				// Can't use template specialization in this case but I can do
				// THIS:
				if(std::is_same<Container, std::vector<int> >::value) {
					return temp;
				} else {
					Container cont(temp.size());
					std::copy(temp.begin(), temp.end(), cont.begin());
					return cont;
				}
			}
		}

		typename CurrentType::value_type currentValue() {
			if(!predicate(this->origin.currentValue())) {
				++(*this);
			}
			return this->origin.currentValue();
		}

		typename ParentType::iterator& end() {
			return this->origin.end();
		}

		CurrentType& operator++() {
			advance();
			return *this;
		}

		CurrentType operator++(int) {
			CurrentType other = *this;
			advance();
			return other;
		}

	private:
		Filter_t predicate;

		void advance() {
			while(!this->origin.hasEnded()
			  && !predicate((++this->origin).currentValue())  );
		}
	};

}

#endif
