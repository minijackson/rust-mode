#ifndef RUST_ERRORS_HPP
#define RUST_ERRORS_HPP

#include <iostream>
#include <thread>
#include <cstdio>

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

#define panic(...) ::rust::realPanic(__FILE__, __LINE__, __VA_ARGS__)

#define unreachable() panic("internal error: entered unreachable code")

#endif
