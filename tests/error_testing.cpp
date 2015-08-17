#include "error.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(error_library_option_concept) {
	rust::Result<char, const char*> someReturnValue =
	                                    rust::Ok<char, const char*>('y'),
	                                someReturnError =
	                                    rust::Err<char, const char*>("error");

	BOOST_CHECK_EQUAL(someReturnValue, (rust::Ok('y')));
	BOOST_CHECK_NE(someReturnValue, (rust::Err("error")));

	BOOST_CHECK_EQUAL(someReturnError, (rust::Err("error")));
	BOOST_CHECK_NE(someReturnError, (rust::Ok('y')));
}

BOOST_AUTO_TEST_CASE(error_library_result_concept) {
	rust::Option<char> someReturnValue = rust::Some('y'),
	                   someReturnNone = rust::None<char>();

	BOOST_CHECK_EQUAL(someReturnValue, rust::Some('y'));
	BOOST_CHECK_NE(someReturnValue, rust::None<char>());

	BOOST_CHECK_EQUAL(someReturnNone, rust::None());
	BOOST_CHECK_NE(someReturnNone, rust::Some('y'));
}
