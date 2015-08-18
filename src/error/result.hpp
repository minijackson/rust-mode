#ifndef RUST_RESULT_HPP
#define RUST_RESULT_HPP

#include "../exceptions/err_cast_exception.hpp"
#include "../utils/errors.hpp"

#include <ostream>

namespace rust {

	template <typename T, typename E>
	struct Result {
		// Not const because may not be initialized
		T value;
		E errorMsg;
		bool const hasSome;

		Result<T, E>(T value, bool) : value(value), hasSome(true) {}
		Result<T, E>(E errorMsg) : errorMsg(errorMsg), hasSome(false) {}

		bool operator==(Result<T, E> const& other) const {
			return (!other.hasSome && !hasSome && other.errorMsg == errorMsg) ||
			       (other.hasSome && hasSome && other.value == value);
		}

		bool operator==(Result<T, void> const& other) const {
			return hasSome && other.value == value;
		}

		bool operator==(Result<void, E> const& other) const {
			return !hasSome && other.errorMsg == errorMsg;
		}

		operator T() {
			if(hasSome) {
				return value;
			} else {
				throw ErrCastException();
			}
		}

		T unwrap() {
			if(!hasSome) {
				panic("Called `Result::unwrap` on an `Err` value: `{}`", errorMsg);
			}
			return value;
		}

		friend std::ostream& operator<<(std::ostream& os,
		                                Result<T, E> const& other) {
			if(other.hasSome) {
				os << other.value;
			} else {
				os << "Err: " << other.errorMsg;
			}
			return os;
		}
	};

	template <typename T>
	struct Result<T, void> {
		T const value;
		bool const hasSome = true;

		Result<T, void>(T value) : value(value) {}

		bool operator==(Result<T, void> const& other) const {
			return other.value == value;
		}

		template <typename E>
		bool operator==(Result<T, E> const& other) const {
			return other.hasSome && other.value == value;
		}

		operator T() {
			return value;
		}

		T unwrap() {
			return value;
		}

		friend std::ostream& operator<<(std::ostream& os,
		                                Result<T, void> const& other) {
			os << other.value;
			return os;
		}
	};

	template <typename E>
	struct Result<void, E> {
		E const errorMsg;
		bool const hasSome = false;

		Result<void, E>(E errorMsg) : errorMsg(errorMsg) {}

		bool operator==(Result<void, E> const& other) const {
			return other.errorMsg == errorMsg;
		}

		template <typename T>
		bool operator==(Result<T, E> const& other) const {
			return !other.hasSome && other.errorMsg == errorMsg;
		}

		friend std::ostream& operator<<(std::ostream& os, Result<void, E> const& other) {
			os << "Err: " << other.errorMsg;
			return os;
		}
	};

	template <typename T, typename E>
	Result<T, E> Ok(T value) {
		return Result<T, E>(value, true);
	}

	template <typename T>
	Result<T, void> Ok(T value) {
		return Result<T, void>(value);
	}

	template <typename T, typename E>
	Result<T, E> Err(E errorMsg) {
		return Result<T, E>(errorMsg);
	}

	template <typename E>
	Result<void, E> Err(E errorMsg) {
		return Result<void, E>(errorMsg);
	}
}

#endif
