#include <iostream>
#include "utils.hpp"

int main() {
	rust::println("{}, {}!", "Hello", "World");
	// Print "Hello, World!"
	std::cerr << rust::format_args("{}+{} = {}", 2, 2, 2+2) << std::endl;
	// Print "2+2 = 4" into stderr

	// The most random number of the universe
	int random = 42;
	if(random >= 0) {
		rust::println("{} >= 0", random);
	} else if(random < 0) {
		rust::println("{} < 0", random);
	} else {
		unreachable("{} neither >= 0 nor < 0", random);
		// Would panic with
		// "internal error: entered unreachable code: 42 neither >= 0 nor < 0"
		// as error message
	}

	//panic("Don't {}!", "panic");
	// panic with "Don't panic!" as error message
}
