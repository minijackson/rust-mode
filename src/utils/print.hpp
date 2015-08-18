#ifndef RUST_PRINT_HPP
#define RUST_PRINT_HPP

#include <string>
#include <sstream>
#include <iostream>

namespace rust {
	std::string format_args(std::string format);
	template <typename T, typename... Args>
	std::string format_args(std::string format, T firstArg, Args... args);
}

#include "errors.hpp"

namespace rust {

	std::string format_args(std::string format) {
		if(format.find("{}") != std::string::npos) {
			panic("unused brackets while formating");
		}
		return format;
	}

	template <typename T, typename... Args>
	std::string format_args(std::string format, T firstArg, Args... args) {
		std::ostringstream os;

		size_t pos = format.find("{}");
		if(pos == std::string::npos) {
			panic("too many arguments for formating");
		}
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
