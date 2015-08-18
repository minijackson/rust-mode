#include <iostream>
#include "error.hpp"

rust::Option<size_t> find(std::string str, std::string substr) {
	size_t pos = str.find(substr);
	if(pos == std::string::npos) {
		return rust::None<size_t>();
	} else {
		return rust::Some(pos);
	}
}

rust::Result<size_t, std::string> findOrErr(std::string str, std::string substr) {
	size_t pos = str.find(substr);
	if(pos == std::string::npos) {
		return rust::Err<size_t, std::string>("Substring not found.");
	} else {
		return rust::Ok<size_t, std::string>(pos);
	}
}

int main() {
	size_t pos = find("Hello, World!", ",").unwrap();
	std::cout << pos << std::endl;
	// Print 5

	pos = find("Hello, World!", "World").unwrap();
	std::cout << pos << std::endl;
	// Print 7

	// Fails:
	//pos = find("Hello, World!", "z").unwrap();
	// Also fails:
	//pos = findOrErr("Hello, World!", "x").unwrap();
}
