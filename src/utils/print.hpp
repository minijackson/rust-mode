#ifndef RUST_PRINT_HPP
#define RUST_PRINT_HPP

#include <string>
#include <sstream>
#include <iostream>

namespace rust {

	const char* format_args(std::string format) {
		return format.c_str();
	}

	template <typename T, typename... Args>
	const char* format_args(std::string format, T firstArg, Args... args) {
		std::ostringstream os;

		int pos = format.find("{}");
		os << format.substr(0, pos) << firstArg << format.substr(pos+2);
		return format_args(os.str(), args...);
	}

	template <typename... Args>
	void print(const char* format, Args... args) {
		std::cout << format_args(format, args...);
	}

	template <typename... Args>
	void println(const char* format, Args... args) {
		std::cout << format_args(format, args...) << std::endl;
	}

}

#endif
