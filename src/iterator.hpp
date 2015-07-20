#include "iterator/iterator.hpp"

namespace rust {
	template<class Container> Iterator<typename Container::iterator> iter(Container& cont) {
		return Iterator<typename Container::iterator>(cont.begin(), cont.end());
	}
}
