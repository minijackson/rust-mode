#include "err_cast_exception.hpp"

const char* ErrCastException::what() const noexcept {
	return "Tried to cast a value out of an {Err}";
}
