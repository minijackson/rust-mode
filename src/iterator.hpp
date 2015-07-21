#include "iterator/iterator.hpp"

#include "iterator/filtered_range.hpp"
#include "iterator/cycled_range.hpp"
#include "iterator/took_range.hpp"
#include "iterator/mapped_range.hpp"
#include "iterator/inspected_range.hpp"

namespace rust {
	template<class Container> Iterator<typename Container::iterator> iter(Container& cont) {
		return Iterator<typename Container::iterator>(cont.begin(), cont.end());
	}
}
