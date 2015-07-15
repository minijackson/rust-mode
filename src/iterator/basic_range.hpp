#ifndef RUST_BASIC_RANGE_HPP
#define RUST_BASIC_RANGE_HPP

#include "../exceptions/infinite_range_exception.hpp"
#include "../exceptions/unknown_value_exception.hpp"

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
	> class BasicRange {
	public:
		BasicRange(iterator beginIt, iterator endIt)
			: beginIt(beginIt), endIt(endIt), noEnd(false) {}

		explicit BasicRange(iterator beginIt)
			: beginIt(beginIt), noEnd(true) {}

		iterator& begin() {
			return beginIt;
		}

		iterator& end() {
			if(noEnd) {
				throw InfiniteRangeException();
			} else {
				return endIt;
			}
		}

		virtual Distance size() {
			if(noEnd) {
				throw InfiniteRangeException();
			} else {
				return std::distance(beginIt, endIt);
			}
		}

		bool empty() {
			return beginIt == endIt;
		}

		bool hasEnd() {
			return !noEnd;
		}

		typedef BasicRange<iterator, Category, T, Distance, Pointer, Reference> CurrentType;

		CurrentType take(size_t count) {
			auto newUpperIt = beginIt;
			std::advance(newUpperIt, count);
			return CurrentType(beginIt, newUpperIt);
		}

		template<typename Container>
		Container collect() {
			if(noEnd) {
				throw InfiniteRangeException();
			} else {
				Container cont(size());
				std::copy(beginIt, endIt, cont.begin());
				return cont;
			}
		}

		CurrentType& operator++() {
			++beginIt;
			return *this;
		}

		CurrentType operator++(int) {
			CurrentType other = *this;
			++beginIt;
			return other;
		}

	protected:
		iterator beginIt, endIt;
		bool noEnd;
	};

}

#endif
