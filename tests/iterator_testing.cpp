#include "iterator.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <tuple>

BOOST_AUTO_TEST_CASE(iterator_library_sequence) {
	auto iterator = rust::Sequence(1, 5);
	std::vector<int> sequence = iterator.collect<std::vector<int>>();
	std::vector<int> expected{1, 2, 3, 4};

	BOOST_CHECK_EQUAL(sequence.size(), 4);
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), sequence.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_iter_function) {
	std::vector<int> vec{1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
	                     11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	                     21, 22, 23, 24, 25, 26, 27, 28, 29, 30};

	rust::Iterator<std::vector<int>::iterator> rtIt(vec.begin(), vec.end());
	auto ltIt = rust::iter(vec);

	std::vector<int> rt = rtIt.collect<std::vector<int>>(),
	                 lt = ltIt.collect<std::vector<int>>();

	BOOST_CHECK_EQUAL(std::distance(rt.begin(), rt.end()),
	                  std::distance(lt.begin(), lt.end()));
	BOOST_CHECK(std::equal(rt.begin(), rt.end(), lt.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_vector_to_rust_iterator) {
	std::vector<int> vec{1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
	                     11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	                     21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
	auto iterator = rust::iter(vec);
	std::vector<int> result = iterator.collect<std::vector<int>>();

	BOOST_CHECK_EQUAL(std::distance(vec.begin(), vec.end()),
	                  std::distance(result.begin(), result.end()));
	BOOST_CHECK(std::equal(vec.begin(), vec.end(), result.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_infinite_size_failure) {
	std::vector<int> vec{};
	rust::Iterator<std::vector<int>::iterator> infIterator(vec.begin());
	BOOST_CHECK_THROW(infIterator.size(), InfiniteRangeException);
}

BOOST_AUTO_TEST_CASE(iterator_library_basic_take) {
	auto iterator = rust::Sequence(1, 31).take(5);
	std::vector<int> result = iterator.collect<std::vector<int>>();

	std::vector<int> expected{1, 2, 3, 4, 5};

	BOOST_CHECK_EQUAL(result.size(), 5);
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), result.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_chained_take) {
	auto iterator = rust::Sequence(1, 31).take(10).take(5);
	std::vector<int> result = iterator.collect<std::vector<int>>();

	std::vector<int> expected{1, 2, 3, 4, 5};

	BOOST_CHECK_EQUAL(result.size(), 5);
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), result.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_rust_iterator_to_list) {
	auto iterator = rust::Sequence(1, 31);

	std::list<int> expected{1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
	                        11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	                        21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
	    list = iterator.collect<std::list<int>>();

	BOOST_CHECK_EQUAL(std::distance(expected.begin(), expected.end()),
	                  std::distance(list.begin(), list.end()));
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), list.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_collected_filter) {
	auto iterator = rust::Sequence(1, 31);

	std::vector<int> filtered = iterator.filter([](int x) {
		return x % 2 == 0;
	}).collect<std::vector<int>>();

	std::vector<int> expected{2,  4,  6,  8,  10, 12, 14, 16,
	                          18, 20, 22, 24, 26, 28, 30};
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), filtered.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_collected_chained_filter) {
	auto iterator = rust::Sequence(1, 31);

	std::vector<int> filtered =
	    iterator.filter([](int x) { return x % 2 == 0; })
	        .filter([](int x) { return x % 3 == 0; })
	        .collect<std::vector<int>>();

	std::vector<int> expected{6, 12, 18, 24, 30};

	BOOST_CHECK(std::equal(expected.begin(), expected.end(), filtered.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_collected_took_chained_filter) {
	auto iterator = rust::Sequence(1);

	std::vector<int> filtered =
	    iterator.filter([](int x) { return x % 2 == 0; })
	        .filter([](int x) { return x % 3 == 0; })
	        .take(3)
	        .collect<std::vector<int>>();

	std::vector<int> expected{6, 12, 18};

	BOOST_CHECK_EQUAL(filtered.size(), 3);
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), filtered.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_collected_cycled_iterator) {
	std::vector<int> vec{1, 2, 3};
	auto iterator = rust::iter(vec);

	std::vector<int> cycled =
	                     iterator.cycle().take(8).collect<std::vector<int>>(),
	                 expected{1, 2, 3, 1, 2, 3, 1, 2};
	BOOST_CHECK_EQUAL(cycled.size(), 8);
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), cycled.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_collected_filtered_cycled_iterator) {
	std::vector<int> vec{1, 2, 3};
	auto iterator = rust::iter(vec);

	std::vector<int> filteredCycled = iterator.cycle().filter([](int x) {
		return x % 2 != 0;
	}).take(5).collect<std::vector<int>>();

	std::vector<int> expected{1, 3, 1, 3, 1};
	BOOST_CHECK_EQUAL(filteredCycled.size(), 5);
	BOOST_CHECK(
	    std::equal(expected.begin(), expected.end(), filteredCycled.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_collected_filtered_took_cycled_iterator) {
	std::vector<int> vec{1, 2, 3};
	auto iterator = rust::iter(vec);

	std::vector<int> filteredTookCycled =
	    iterator.cycle().take(9).filter([](int x) {
		    return x == 1;
		}).collect<std::vector<int>>();

	std::vector<int> expected{1, 1, 1};
	BOOST_CHECK_EQUAL(filteredTookCycled.size(), 3);
	BOOST_CHECK(std::equal(expected.begin(), expected.end(),
	                       filteredTookCycled.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_collected_cycled_filtered_iterator) {
	std::vector<int> vec{1, 2, 3};
	auto iterator = rust::iter(vec);

	std::vector<int> cycledFiltered =
	    iterator.filter([](int x) { return x % 2 != 0; })
	        .cycle()
	        .take(5)
	        .cycle()
	        .take(6)
	        .cycle()
	        .take(12)
	        .collect<std::vector<int>>();

	std::vector<int> expected{1, 3, 1, 3, 1, 1, 1, 3, 1, 3, 1, 1};
	BOOST_CHECK_EQUAL(cycledFiltered.size(), 12);
	BOOST_CHECK(
	    std::equal(expected.begin(), expected.end(), cycledFiltered.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_collected_mapped_iterator) {
	std::vector<int> vec{1, 2, 3, 4, 5};
	auto iterator = rust::iter(vec);

	std::vector<int> mapped =
	    iterator.map([](int x) { return x * 2; }).collect<std::vector<int>>();

	std::vector<int> expected{2, 4, 6, 8, 10};
	BOOST_CHECK_EQUAL(mapped.size(), 5);
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), mapped.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_filtered_mapped_iterator) {
	std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	auto iterator = rust::iter(vec);

	std::vector<int> filteredMapped =
	    iterator.map([](int x) { return x * 2; })
	        .filter([](int x) { return x % 4 == 0; })
	        .collect<std::vector<int>>();

	std::vector<int> expected{4, 8, 12, 16, 20};
	BOOST_CHECK_EQUAL(filteredMapped.size(), 5);
	BOOST_CHECK(
	    std::equal(expected.begin(), expected.end(), filteredMapped.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_mapped_filtered_iterator) {
	std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	auto iterator = rust::iter(vec);

	std::vector<int> mappedFiltered = iterator.filter([](int x) {
		return x % 2 == 0;
	}).map([](int x) { return x * 2; }).collect<std::vector<int>>();

	std::vector<int> expected{4, 8, 12, 16, 20};
	BOOST_CHECK_EQUAL(mappedFiltered.size(), 5);
	BOOST_CHECK(
	    std::equal(expected.begin(), expected.end(), mappedFiltered.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_inspected_iterator) {
	std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	auto iterator = rust::iter(vec);

	std::ostringstream os;

	std::vector<int> inspected = iterator.inspect([&os](int x) {
		os << x << ",";
	}).collect<std::vector<int>>();

	std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	BOOST_CHECK_EQUAL(inspected.size(), 10);
	BOOST_CHECK(
	    std::equal(expected.begin(), expected.end(), inspected.begin()));
	BOOST_CHECK_EQUAL(os.str(), "1,2,3,4,5,6,7,8,9,10,");
}

BOOST_AUTO_TEST_CASE(iterator_library_inspected_inspected_iterator) {
	std::vector<int> vec{1, 2, 3, 4, 5};
	auto iterator = rust::iter(vec);

	std::ostringstream os;

	std::vector<int> inspectedInspected =
	    iterator.inspect([&os](int x) { os << x << ","; })
	        .inspect([&os](int x) { os << x << ","; })
	        .collect<std::vector<int>>();

	std::vector<int> expected{1, 2, 3, 4, 5};
	BOOST_CHECK_EQUAL(inspectedInspected.size(), 5);
	BOOST_CHECK(std::equal(expected.begin(), expected.end(),
	                       inspectedInspected.begin()));
	BOOST_CHECK_EQUAL(os.str(), "1,1,2,2,3,3,4,4,5,5,");
}

BOOST_AUTO_TEST_CASE(iterator_library_zipped_iterator) {
	std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
	std::vector<char> vec2{'a', 'b', 'c', 'd', 'e'};
	auto iterator = rust::iter(vec);
	auto iterator2 = rust::iter(vec2);

	std::vector<std::pair<int, char>> zipped =
	    iterator.zip(iterator2).collect<std::vector<std::pair<int, char>>>();

	std::vector<std::pair<int, char>> expected{
	    {1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}, {5, 'e'}};
	BOOST_CHECK_EQUAL(zipped.size(), 5);
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), zipped.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_filitered_zipped_iterator) {
	std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
	std::vector<char> vec2{'a', 'b', 'c', 'd', 'e'};
	auto iterator = rust::iter(vec);
	auto iterator2 = rust::iter(vec2);

	std::vector<std::pair<int, char>> filteredZipped =
	    iterator.zip(iterator2).filter([](std::pair<int, char> x) {
		    return x != std::make_pair(3, 'c');
		}).collect<std::vector<std::pair<int, char>>>();

	std::vector<std::pair<int, char>> expected{
	    {1, 'a'}, {2, 'b'}, {4, 'd'}, {5, 'e'}};
	BOOST_CHECK_EQUAL(filteredZipped.size(), 4);
	BOOST_CHECK(
	    std::equal(expected.begin(), expected.end(), filteredZipped.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_unzipped_filitered_zipped_iterator) {
	std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
	std::vector<char> vec2{'a', 'b', 'c', 'd', 'e'};
	auto iterator = rust::iter(vec);
	auto iterator2 = rust::iter(vec2);

	std::vector<int> result1;
	std::vector<char> result2;

	std::tie(result1, result2) =
	    iterator.zip(iterator2).filter([](std::pair<int, char> x) {
		    return x != std::make_pair(3, 'c');
		}).unzip<std::vector<int>, std::vector<char>>();

	std::vector<int> expected1{1, 2, 4, 5};
	std::vector<char> expected2{'a', 'b', 'd', 'e'};
	BOOST_CHECK_EQUAL(result1.size(), 4);
	BOOST_CHECK(
	    std::equal(expected1.begin(), expected1.end(), result1.begin()));
	BOOST_CHECK_EQUAL(result2.size(), 4);
	BOOST_CHECK(
	    std::equal(expected2.begin(), expected2.end(), result2.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_chained_iterator) {
	auto iterator = rust::Sequence(1, 5), iterator2 = rust::Sequence(5, 10),
	     expectedIt = rust::Sequence(1, 10);

	std::vector<int> chained =
	    iterator.chain(iterator2).collect<std::vector<int>>();

	std::vector<int> expected = expectedIt.collect<std::vector<int>>();

	BOOST_CHECK_EQUAL(chained.size(), 9);
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), chained.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_mapped_chained_iterator) {
	auto iterator = rust::Sequence(1, 5), iterator2 = rust::Sequence(5, 10),
	     expectedIt = rust::Sequence(11, 20);

	std::vector<int> mappedChained = iterator.chain(iterator2).map([](int x) {
		return x + 10;
	}).collect<std::vector<int>>();

	std::vector<int> expected = expectedIt.collect<std::vector<int>>();

	BOOST_CHECK_EQUAL(mappedChained.size(), 9);
	BOOST_CHECK(
	    std::equal(expected.begin(), expected.end(), mappedChained.begin()));
}

BOOST_AUTO_TEST_CASE(iterator_library_partitioned_iterator) {
	auto iterator = rust::Sequence(1, 10);

	std::vector<int> even, odd, expectedEven{2, 4, 6, 8},
	    expectedOdd{1, 3, 5, 7, 9};

	std::tie(even, odd) =
	    iterator.partition<std::vector<int>, std::vector<int>>(
	        [](int x) { return x % 2 == 0; });

	BOOST_CHECK_EQUAL(even.size(), 4);
	BOOST_CHECK(
	    std::equal(expectedEven.begin(), expectedEven.end(), even.begin()));
	BOOST_CHECK_EQUAL(odd.size(), 5);
	BOOST_CHECK(
	    std::equal(expectedOdd.begin(), expectedOdd.end(), odd.begin()));
}
