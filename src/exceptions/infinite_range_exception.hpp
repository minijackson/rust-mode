#ifndef INFINITE_RANGE_EXCEPTION_HPP
#define INFINITE_RANGE_EXCEPTION_HPP

#include <exception>

class InfiniteRangeException : public std::exception {
public:
	virtual const char* what() const noexcept __attribute__((const));
};

#endif
