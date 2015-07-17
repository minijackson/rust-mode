#ifndef RUST_ITERATOR_HPP
#define RUST_ITERATOR_HPP

#include "../exceptions/infinite_range_exception.hpp"
#include "basic_range.hpp"

#include "filtered_range.hpp"
#include "cycled_range.hpp"

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
	> class Iterator : public BasicRange<iterator, Category, T, Distance, Pointer, Reference> {
	public:
		typedef Iterator<iterator, Category, T, Distance, Pointer, Reference> CurrentType;
		typedef BasicRange<iterator, Category, T, Distance, Pointer, Reference> ParentType;

		Iterator(iterator beginIt, iterator endIt) : ParentType(beginIt, endIt) {}
		explicit Iterator(iterator beginIt) : ParentType(beginIt) {}

		FilteredRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>
		filter(std::function<bool(T)> predicate) {
			return FilteredRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>(*this, predicate);
		}

		CycledRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>
		cycle() {
			return CycledRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>(*this);
		}
	};

}

#endif
