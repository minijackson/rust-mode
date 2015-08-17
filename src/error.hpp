#ifndef ERROR_LIBRARY_MAIN_HPP
#define ERROR_LIBRARY_MAIN_HPP

#include <iostream>
#include <thread>
#include <cstdio>

#define panic(...) ::rust::realPanic(__FILE__, __LINE__, __VA_ARGS__)

namespace rust {
	template <typename... Args>
	void realPanic(const char* filename, int lineNo, const char* msg,
	               Args... args) {
		std::cerr << "Thread " << std::this_thread::get_id()
		          << " panicked at '";
		std::fprintf(stderr, msg, args...);
		std::cerr << "', " << filename << ":" << lineNo << std::endl;
		exit(101);
	}
}

#include "error/option.hpp"
#include "error/result.hpp"

#endif
