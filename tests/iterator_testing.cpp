#include "iterator/iterator.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

BOOST_AUTO_TEST_CASE(iterator_library_vector_to_rust_iterator) {
	std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
	rust::Iterator<std::vector<int>::iterator> iterator(vec.begin(), vec.end());

	BOOST_CHECK(std::distance(vec.begin(), vec.end()) == std::distance(iterator.begin(), iterator.end()));
	BOOST_CHECK(std::equal(vec.begin(), vec.end(), iterator.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_infinite_size_failure) {
	std::vector<int> vec{};
	try {
		rust::Iterator<std::vector<int>::iterator> infIterator(vec.begin());
		infIterator.size();
		BOOST_ERROR("Infinite Iterator did not fail when getting its size.");
	} catch(InfiniteRangeException e) {}
}

BOOST_AUTO_TEST_CASE(iterator_library_basic_take) {
	std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
	rust::Iterator<std::vector<int>::iterator> iterator(vec.begin(), vec.end());

	auto newIt = iterator.take(5);

	std::vector<int> expected{1,2,3,4,5};

	BOOST_CHECK(newIt.size() == 5);
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), newIt.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_chained_take) {
	std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
	rust::Iterator<std::vector<int>::iterator> iterator(vec.begin(), vec.end());

	auto newIt = iterator.take(10).take(5);

	std::vector<int> expected{1,2,3,4,5};

	BOOST_CHECK(newIt.size() == 5);
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), newIt.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_rust_iterator_to_list) {
	std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
	rust::Iterator<std::vector<int>::iterator> iterator(vec.begin(), vec.end());

	std::list<int> expected{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30},
	               list = iterator.collect<std::list<int> >();

	BOOST_CHECK(std::distance(expected.begin(), expected.end()) == std::distance(list.begin(), list.end()));
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), list.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_collected_filter) {
	std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
	rust::Iterator<std::vector<int>::iterator> iterator(vec.begin(), vec.end());

	std::vector<int> filtered = iterator.filter([] (int x) {
				return x % 2 == 0;
			}).collect<std::vector<int> >();

	std::vector<int> expected{2,4,6,8,10,12,14,16,18,20,22,24,26,28,30};
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), filtered.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_collected_chained_filter) {
	std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
	rust::Iterator<std::vector<int>::iterator> iterator(vec.begin(), vec.end());

	std::vector<int> filtered = iterator.filter([] (int x) {
				return x % 2 == 0;
			})
			.filter([] (int x) {
				return x % 3 == 0;
			})
			.collect<std::vector<int> >();

	std::vector<int> expected{6,12,18,24,30};
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), filtered.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_collected_took_chained_filter) {
	std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
	rust::Iterator<std::vector<int>::iterator> iterator(vec.begin(), vec.end());

	std::vector<int> filtered = iterator.filter([] (int x) {
				return x % 2 == 0;
			})
			.filter([] (int x) {
				return x % 3 == 0;
			})
			.take(3)
			.collect<std::vector<int> >();

	std::vector<int> expected{6,12,18};
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), filtered.begin()));
}
