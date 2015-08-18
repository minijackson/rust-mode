#ifndef RUST_ERRORS_HPP
#define RUST_ERRORS_HPP

namespace rust {
	template <typename... Args>
	void realPanic(const char* filename, int lineNo, const char* msg,
	               Args... args);
}

#define panic(...) ::rust::realPanic(__FILE__, __LINE__, __VA_ARGS__)
#define unreachable(...)                                                       \
	::rust::realUnreachable(__FILE__, __LINE__, __VA_ARGS__)

#include "print.hpp"

#include <iostream>
#include <thread>
#include <cstdio>

namespace rust {
	template <typename... Args>
	void realPanic(const char* filename, int lineNo, const char* msg,
	               Args... args) {
		std::cerr << "Thread " << std::this_thread::get_id() << " panicked at '"
		          << format_args(msg, args...) << "', " << filename << ":"
		          << lineNo << std::endl;
		exit(101);
	}

	template <typename... Args>
	void realUnreachable(const char* filename, int lineNo, const char* msg,
	                     Args... args) {
		realPanic(filename, lineNo,
		          "internal error: entered unreachable code: {}",
		          ::rust::format_args(msg, args...));
	}
}

#endif
