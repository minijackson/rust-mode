#include "infinite_range_exception.hpp"

const char* InfiniteRangeException::what() const noexcept {
	return "Range has an infinite number of value";
}
