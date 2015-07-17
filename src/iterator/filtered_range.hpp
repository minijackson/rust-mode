#ifndef RUST_FILTERED_RANGE_HPP
#define RUST_FILTERED_RANGE_HPP

#include "range_modifier.hpp"
#include "basic_range.hpp"

#include "all_range_modifiers.hpp"

#include <functional>
#include <vector>
#include <algorithm>

namespace rust {

	template<
		class OriginRange,
		class iterator,
		class Category  = typename iterator::iterator_category,
		class T         = typename iterator::value_type,
		class Distance  = std::ptrdiff_t,
		class Pointer   = T*,
		class Reference = T&
	> class FilteredRange : public RangeModifier<OriginRange, iterator, Category, T, Distance, Pointer, Reference> {

		typedef std::function<bool(T)> Filter_t;
		typedef FilteredRange<OriginRange, iterator, Category, T, Distance, Pointer, Reference> CurrentType;
		typedef RangeModifier<OriginRange, iterator, Category, T, Distance, Pointer, Reference> ParentType;

	public:
		FilteredRange(OriginRange range, Filter_t predicate)
			: ParentType(range), predicate(predicate) {
		}

		virtual Distance size() {
			if(this->noEnd) {
				throw InfiniteRangeException();
			} if(!specifiedCount) {
				throw UnknownValueException("Cannot know the size of a filtered range before consuming values");
			} else {
				return count;
			}
		}

		virtual bool empty() {
			throw UnknownValueException("Cannot know if a filtered range is empty before consuming values");
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
				std::vector<T> temp;
				auto inserter = std::back_inserter(temp);
				OriginRange& origin = ParentType::origin;
				if(!specifiedCount) {
					while(origin.begin() != origin.end()) {
						if(predicate(*origin.begin())) {
							*inserter++ = *origin.begin();
						}
						++origin;
					}
				} else {
					unsigned int i = 0;
					while((origin.begin() != origin.end()) && i < count) {
						if(predicate(*origin.begin())) {
							*inserter++ = *origin.begin();
							++i;
						}
						++origin;
					}
				}
				Container cont(temp.size());
				std::copy(temp.begin(), temp.end(), cont.begin());
				return cont;
			}
		}

		iterator begin() {
			if(predicate(*ParentType::origin.begin())) {
				return ParentType::origin.begin();
			} else {
				++(*this);
				return ParentType::origin.begin();
			}
		}

		CurrentType& operator++() {
			while(ParentType::origin.begin() != ParentType::origin.end() && !predicate(*(++ParentType::origin).begin()));
			return *this;
		}

		CurrentType operator++(int) {
			CurrentType other = *this;
			while(ParentType::origin.begin() != ParentType::origin.end() && !predicate(*(++ParentType::origin).begin()));
			return other;
		}

	private:
		Filter_t predicate;
		size_t count = 0;
		bool specifiedCount = false;
	};

}

#endif
