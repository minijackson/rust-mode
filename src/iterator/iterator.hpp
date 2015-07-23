#ifndef RUST_ITERATOR_HPP
#define RUST_ITERATOR_HPP

#include "../exceptions/infinite_range_exception.hpp"
#include "basic_range.hpp"

#include "all_range_modifiers.hpp"

#include <iterator>
#include <limits>
#include <type_traits>
#include <functional>

namespace rust {

	template<
		class myIterator,
		class T         = typename myIterator::value_type,
		class Distance  = std::ptrdiff_t,
		class Pointer   = T*,
		class Reference = T&
	> class Iterator : public BasicRange<myIterator, T, Distance, Pointer, Reference> {
		typedef Iterator<myIterator, T, Distance, Pointer, Reference> CurrentType;
		typedef BasicRange<myIterator, T, Distance, Pointer, Reference> ParentType;

	public:
		typedef myIterator iterator;
		typedef T          value_type;
		typedef Distance   difference_type;

		Iterator(iterator beginIt, iterator endIt) : ParentType(beginIt, endIt) {}
		explicit Iterator(iterator beginIt) : ParentType(beginIt) {}

		RANGE_MODIFIERS
	};

}

#endif
