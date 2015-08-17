#include "utils.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(utils_library_format_args) {
	const char format[] = "{}, {}!", firstArg[] = "Hello",
	           secondArg[] = "World";

	BOOST_CHECK_EQUAL(
	    std::string(rust::format_args(format, firstArg, secondArg)),
	    std::string("Hello, World!"));
}
