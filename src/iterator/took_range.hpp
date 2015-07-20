#ifndef RUST_TOOK_RANGE_HPP
#define RUST_TOOK_RANGE_HPP

#include "range_modifier.hpp"
#include "all_range_modifiers.hpp"

#include <iterator>

namespace rust {

	template<
		class OriginRange,
		class iterator,
		class Category  = typename iterator::iterator_category,
		class T         = typename iterator::value_type,
		class Distance  = std::ptrdiff_t,
		class Pointer   = T*,
		class Reference = T&
	> class TookRange : public RangeModifier<OriginRange, iterator, Category, T, Distance, Pointer, Reference> {

		typedef TookRange<OriginRange, iterator, Category, T, Distance, Pointer, Reference> CurrentType;
		typedef RangeModifier<OriginRange, iterator, Category, T, Distance, Pointer, Reference> ParentType;

	public:
		TookRange(OriginRange range, size_t count)
			: ParentType(range), count(count) {
		}

		virtual Distance size() {
			return count;
		}

		virtual bool empty() {
			return count == 0 || this->origin.empty();
		}

		RANGE_MODIFIERS

		template<typename Container>
		Container collect() {
			try {
				size_t originSize = this->origin.size(),
				       finalSize  = (count > originSize) ? originSize : count;
				return collectSizeAware<Container>(finalSize);
			} catch(InfiniteRangeException) {
				return collectSizeAware<Container>(count);
			} catch(UnknownValueException) {
				return collectSizeUnaware<Container>();
			}
		}

		CurrentType& operator++() {
			++this->origin;
			++progress;
			return *this;
		}

		CurrentType operator++(int) {
			CurrentType other = *this;
			++this->origin;
			++progress;
			return other;
		}

		iterator& begin() {
			if(progress >= count) {
				return this->origin.end();
			} else {
				return this->origin.begin();
			}
		}

		T beginValue() {
			return this->origin.beginValue();
		}

		iterator& end() {
			return this->origin.end();
		}

	private:
		size_t count, progress = 0;

		template<typename Container>
		Container collectSizeAware(size_t size) {
			Container cont(size);
			for(T& value: cont) {
				value = beginValue();
				++(*this);
			}
			return cont;
		}

		template<typename Container>
		Container collectSizeUnaware() {
			std::vector<T> temp;
			auto inserter = std::back_inserter(temp);

			while(progress < count && begin() != end()) {
				*inserter++ = beginValue();
				++(*this);
			}

			// Can't use template specialization in this case but I can do
			// THIS:
			if(std::is_same<Container, std::vector<int> >::value) {
				return temp;
			} else {
				Container cont(temp.size());
				std::copy(temp.begin(), temp.end(), cont.begin());
				return cont;
			}
		}
	};

}

#endif
