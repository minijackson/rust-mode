#include <iostream>
#include "iterator.hpp"

int main() {
	auto iterator = rust::Sequence(1,10);
	// -> 1, 2, 3, 4, 5, 6, 7, 8, 9

	std::vector<int> result = iterator
		.filter([] (int x) {
			return x % 2 == 0;
		})
		// -> 2, 4, 6, 8
		.cycle()
		// -> 2, 4, 6, 8, 2, 4, 6, 8, 2, ...
		.map([] (int x) {
			return -x + 6;
		})
		// -> 4, 2, 0, -2, 4, 2, 0, -2, ...
		.take(2)
		// -> 4, 2
		.collect<std::vector<int>>();
		// Consume the values and store it in a vector of int

	for(int i : result) {
		std::cout << i;
	}
	std::cout << std::endl;
	// Prints 42
}
