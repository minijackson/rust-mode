#ifndef RUST_BASIC_RANGE_HPP
#define RUST_BASIC_RANGE_HPP

#include "../exceptions/infinite_range_exception.hpp"
#include "../exceptions/unknown_value_exception.hpp"

#include <functional>
#include <iterator>
#include <limits>
#include <type_traits>

namespace rust {

	template<
		class iterator,
		class T         = typename iterator::value_type,
		class Distance  = std::ptrdiff_t,
		class Pointer   = T*,
		class Reference = T&
	> class BasicRange {

		typedef BasicRange<iterator, T, Distance, Pointer, Reference> CurrentType;

	public:
		BasicRange(iterator beginIt, iterator endIt)
			: beginIt(beginIt), endIt(endIt), noEnd(false) {}

		explicit BasicRange(iterator beginIt)
			: beginIt(beginIt), noEnd(true) {}

		explicit BasicRange(bool hasEnd) : noEnd(!hasEnd) {}

		virtual ~BasicRange() = default;

		T currentValue() {
			return *beginIt;
		}

		virtual Distance size() {
			if(noEnd) {
				throw InfiniteRangeException();
			} else {
				return std::distance(beginIt, endIt);
			}
		}

		virtual bool empty() {
			return beginIt == endIt;
		}

		virtual bool hasEnd() {
			return !noEnd;
		}

		virtual bool hasEnded() {
			if(noEnd) {
				return false;
			} else {
				return beginIt == endIt;
			}
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
