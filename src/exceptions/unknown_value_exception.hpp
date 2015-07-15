#ifndef UNKNOWN_VALUE_EXCEPTION_HPP
#define UNKNOWN_VALUE_EXCEPTION_HPP

#include <exception>

class UnknownValueException : public std::exception {
public:
	explicit UnknownValueException(const char* message);
	virtual const char* what() const noexcept __attribute__((pure));
private:
	const char* message;
};

#endif
