#ifndef ERR_CAST_EXCEPTION_HPP
#define ERR_CAST_EXCEPTION_HPP

#include <exception>

class ErrCastException : public std::exception {
public:
	virtual const char* what() const noexcept __attribute__((const));
};

#endif
