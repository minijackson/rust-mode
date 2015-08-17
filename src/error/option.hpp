#ifndef RUST_OPTION
#define RUST_OPTION

#include <ostream>

namespace rust {

	template <typename T>
	struct Option {
		bool const hasSome;
		// Not const because it may not be initialized
		T value;

		Option<T>(bool hasSome, T value) : hasSome(hasSome), value(value) {}
		Option<T>() : hasSome(false) {}

		bool operator==(Option<T> const& other) const {
			return (!other.hasSome && !hasSome) ||
			       (other.hasSome && hasSome && other.value == value);
		}

		bool operator==(Option<void> const&) const {
			return !hasSome;
		}

		friend std::ostream& operator<<(std::ostream& os,
		                                Option<T> const& other) {
			if(other.hasSome) {
				os << other.value;
			} else {
				os << "{None}";
			}
			return os;
		}
	};

	template <>
	struct Option<void> {
		bool const hasSome = false;

		template <typename T>
		bool operator==(Option<T> const& other) {
			return !other.hasSome;
		}

		friend std::ostream& operator<<(std::ostream& os, Option<void>) {
			os << "{None}";
			return os;
		}
	};

	template <typename T>
	Option<T> Some(T value) {
		return Option<T>(true, value);
	}

	template <typename T>
	Option<T> None() {
		return Option<T>();
	}

	Option<void> None() {
		return Option<void>();
	}
}

#endif
