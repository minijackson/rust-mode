#ifndef RUST_FILTERED_RANGE_HPP
#define RUST_FILTERED_RANGE_HPP

#include "range_modifier.hpp"
#include "basic_range.hpp"

#include "all_range_modifiers.hpp"

#include <functional>
#include <vector>
#include <algorithm>
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
	> class FilteredRange : public RangeModifier<OriginRange, iterator, Category, T, Distance, Pointer, Reference> {

		typedef std::function<bool(T)> Filter_t;
		typedef FilteredRange<OriginRange, iterator, Category, T, Distance, Pointer, Reference> CurrentType;
		typedef RangeModifier<OriginRange, iterator, Category, T, Distance, Pointer, Reference> ParentType;

	public:
		FilteredRange(OriginRange range, Filter_t predicate)
			: ParentType(range), predicate(predicate) {
		}

		virtual Distance size() {
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
				std::vector<T> temp;
				auto inserter = std::back_inserter(temp);

				while(begin() != end()) {
					*inserter++ = *begin();
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

		iterator begin() {
			if(predicate(*ParentType::origin.begin())) {
				return this->origin.begin();
			} else {
				++(*this);
				return this->origin.begin();
			}
		}

		iterator end() {
			return this->origin.end();
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
	};

}

#endif
