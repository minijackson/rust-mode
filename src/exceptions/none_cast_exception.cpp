#include "none_cast_exception.hpp"

const char* NoneCastException::what() const noexcept {
	return "Tried to cast a value out of a {None}";
}
