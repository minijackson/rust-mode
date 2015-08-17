#ifndef NONE_CAST_EXCEPTION_HPP
#define NONE_CAST_EXCEPTION_HPP

#include <exception>

class NoneCastException : public std::exception {
public:
	virtual const char* what() const noexcept __attribute__((const));
};

#endif
