#ifndef RUST_FILTERED_RANGE_HPP
#define RUST_FILTERED_RANGE_HPP

#include "basic_range.hpp"

#include <functional>
#include <vector>
#include <algorithm>

namespace rust {

	template<
		class iterator,
		class Category  = typename iterator::iterator_category,
		class T         = typename iterator::value_type,
		class Distance  = std::ptrdiff_t,
		class Pointer   = T*,
		class Reference = T&
	> class FilteredRange : public BasicRange<iterator, Category, T, Distance, Pointer, Reference> {

		typedef std::function<bool(T)> Filter_t;
		typedef FilteredRange<iterator, Category, T, Distance, Pointer, Reference> CurrentType;
		typedef BasicRange<iterator, Category, T, Distance, Pointer, Reference> ParentType;

	public:
		FilteredRange(ParentType range, Filter_t predicate)
			: ParentType(range), predicate(predicate) {
		}

		FilteredRange(iterator beginIt, iterator endIt, Filter_t predicate)
			: ParentType(beginIt, endIt), predicate(predicate) {
		}

		virtual Distance size() {
			if(!specifiedCount) {
				throw;
			} else {
				return count;
			}
		}

		CurrentType take(size_t count) {
			specifiedCount = true;
			this->count = count;
			return *this;
		}

		template<typename Container>
		Container collect() {
			if(this->noEnd) {
				throw InfiniteRangeException();
			} else {
				std::vector<T> temp;
				auto inserter = std::back_inserter(temp);
				if(!specifiedCount) {
					std::copy_if(this->beginIt, this->endIt, inserter, predicate);
				} else {
					unsigned int i = 0;
					ParentType range = *this;
					while(!range.empty() && i < count) {
						if(predicate(*range.begin())) {
							*inserter++ = *range.begin();
							++range;
							++i;
						} else {
							++range;
						}
					}
				}
				Container cont(temp.size());
				std::copy(temp.begin(), temp.end(), cont.begin());
				return cont;
			}
		}

	private:
		Filter_t predicate;
		size_t count = 0;
		bool specifiedCount = false;
	};

}

#endif
