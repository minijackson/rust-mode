#ifndef RUST_ERROR_LIBRARY_MAIN_HPP
#define RUST_ERROR_LIBRARY_MAIN_HPP

#include "error/option.hpp"
#include "error/result.hpp"

namespace rust {
	template<typename>
	struct is_option : std::false_type {};

	template<typename T>
	struct is_option<Option<T>> : std::true_type {};

	template<typename>
	struct is_result : std::false_type {};

	template<typename T, typename E>
	struct is_result<Result<T, E>> : std::true_type {};
}

#define rTry(...)                                                              \
	static_assert(::rust::is_result<decltype(__VA_ARGS__)>::value,             \
	              "rTry needs an expression returning a value of type "        \
	              "`Result`");                                                 \
	{                                                                          \
		auto _maybeAnError = __VA_ARGS__;                                      \
		if(!_maybeAnError.hasSome)                                             \
			return _maybeAnError;                                              \
	}

#endif
