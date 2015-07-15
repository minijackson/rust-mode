#include "unknown_value_exception.hpp"

UnknownValueException::UnknownValueException(const char* message)
	: message(message) {}

const char* UnknownValueException::what() const noexcept {
	return message;
}
