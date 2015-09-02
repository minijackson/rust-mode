# rust-mode

[![Build Status](https://travis-ci.org/minijackson/rust-mode.svg?branch=master)](https://travis-ci.org/minijackson/rust-mode)
[![Coverage Status](https://coveralls.io/repos/minijackson/rust-mode/badge.svg?branch=master&service=github)](https://coveralls.io/github/minijackson/rust-mode?branch=master)

Library to imitate some Rust concepts in C++

## Why?

Because I can.

## What has been done so far?

### Iterator library

An example:
```cpp
#include <iostream>
#include <rust/iterator.hpp>

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
```

These classes are implemented:

| Class               | Description                                |
|---------------------|--------------------------------------------|
| Sequence            | Generate a sequence of consecutive values. |

These methods are implemented for Iterator/Range classes, with ```T``` the
range value type:

#### Modifiers

| Modifier                  | Description                                                                |
|---------------------------|----------------------------------------------------------------------------|
| ```chain(Range)```        | Chain ranges. (e.g.: ```{1,2,3}``` + ```{4,5,6}``` -> ```{1,2,3,4,5,6}```) |
| ```cycle()```             | Cycle the range. (e.g.: ```{1,2,3}``` -> ```{1,2,3,1,2,3,...}```)          |
| ```filter(bool(T))```     | Filter the range with a given predicate. (e.g.: ```{1,2,3}``` + ```[] (int x) {return x%2 == 1}``` -> ```{1,3}```) |
| ```inspect(void(T))```    | Call the given function for each element in the range.                     |
| ```map(T(T))```           | Map the value of the range. (e.g.: ```{1,2,3}``` + ```[] (int x) {return x+1}``` -> ```{2,3,4}```) |
| ```take(int)```           | Take a given number of elements in the range                               |
| ```take_while(bool(T))``` | Take the elements of the range while a given predicate is true             |
| ```zip(Range)```          | Zip elements of two ranges. (e.g.: ```{1,2,3}``` + ```{'a','b','c'}``` -> ```{(1,'a'),(2,'b'),(3,'c')}```) |

### Range informations

Please consider the ```collect``` method instead because each of these methods
may throw because it may be impossible to know these informations before
collection.

| Method                    | Description                                                                |
|---------------------------|----------------------------------------------------------------------------|
| size()                    | Get the number of elements in the range.                                   |
| empty()                   | Returns ```true``` if the range is empty.                                  |

#### Other methods

| Method                    | Description                                                                |
|---------------------------|----------------------------------------------------------------------------|
| ```collect<Container>()```| Consume the values and returns a Container with these values.              |
| ```fold(T, T(T, T))```    | Fold the range with the first argument as initial value.                   |
| ```partition<Container1, Container2>(bool(T))```| Returns a pair<Container1, Container2>. The first container contains the value of the range for which the predicate is true, else they are stored in the second container |
| ```unzip<Container1, Container2>()```| Unzip a zipped range (e.g.: ```{(1,'a'),(2,'b'),(3,'c')}``` -> ```({1,2,3},{'a','b','c'})```) |

### Error library

An exemple:
```cpp
#include <iostream>
#include <rust/utils.hpp>

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
```


These structures are implemented:

| Structure                 | Description                       |
|---------------------------|-----------------------------------|
| ```Option<T>```           | An optional value.                |
| ```Result<T, E>```        | A result or an error.             |

These methods are implemented for the Option and Result structures:

| Method                    | Description                                                                    |
|---------------------------|--------------------------------------------------------------------------------|
| unwrap()                  | Panic if the Option is a None or the Result is an Err. Else, return the value. |

These functions are defined:

| Function                  | Description                             |
|---------------------------|-----------------------------------------|
| ```Some(T)```             | Return an Option<T> with a value.       |
| ```None()```              | Return an Option<void> with no value.   |
| ```None<T>()```           | Return an Option<T> with no value.      |
| ```Ok(T)```               | Return a Result<T, void> with a value.  |
| ```Ok<T, E>(T)```         | Return a Result<T, E> with a value.     |
| ```Err(E)```              | Return a Result<void, E> with an error. |
| ```Err<T, E>(T)```        | Return a Result<T, E> with an error.    |

These macros are defined:

| Macro                  | Description                                                                                         |
|------------------------|-----------------------------------------------------------------------------------------------------|
| ```rTry(expression)``` | Execute ```expression``` (which must return a Result<T, E>) and if the result is an Err, return it. |

### Utils library

An example:
```cpp
#include <iostream>
#include <rust/utils.hpp>

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

	panic("Don't {}!", "panic");
	// panic with "Don't panic!" as error message
}
```

These functions are implemented:

| Function                        | Description                                               |
|---------------------------------|-----------------------------------------------------------|
| ```format_args(string, ...)```  | Format the string with the given arguments (see example). |
| ```print(string, ...)```        | Format and print the string to stdout.                    |
| ```println(string, ...)```      | Format and print the string to stdout with a newline.     |

These macros are defined:

| Macro                          | Description                                                                                            |
|--------------------------------|--------------------------------------------------------------------------------------------------------|
| ```panic(string, ...)```       | Make the program exit with a positive error code (101) and print the formated string as error message. |
| ```unreachable(string, ...)``` | Make the program exit with a positive error code (101) and print the formated string as error message while indicating that some normally unreachable code has been encountered. |
