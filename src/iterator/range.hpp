#ifndef RUST_RANGE_HPP
#define RUST_RANGE_HPP

#include "../exceptions/infinite_range_exception.hpp"
#include "basic_range.hpp"
#include "filtered_range.hpp"

#include <iterator>
#include <limits>
#include <type_traits>
#include <functional>

namespace rust {

	template<
		class iterator,
		class Category  = typename iterator::iterator_category,
		class T         = typename iterator::value_type,
		class Distance  = std::ptrdiff_t,
		class Pointer   = T*,
		class Reference = T&
	> class Range : public BasicRange<iterator, Category, T, Distance, Pointer, Reference> {
	public:
		typedef FilteredRange<iterator, Category, T, Distance, Pointer, Reference> FilteredCurrentType;
		typedef BasicRange<iterator, Category, T, Distance, Pointer, Reference> ParentType;

		Range(iterator beginIt, iterator endIt) : ParentType(beginIt, endIt) {}
		explicit Range(iterator beginIt) : ParentType(beginIt) {}

		FilteredCurrentType filter(std::function<bool(T)> predicate) {
			return FilteredCurrentType(*this, predicate);
		}
	};

}

#endif
