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
			std::vector<T> temp;
			auto inserter = std::back_inserter(temp);
			OriginRange& origin = ParentType::origin;

			unsigned int i = 0;
			while(i < count && origin.begin() != origin.end()) {
				*inserter++ = *origin.begin();
				++i;
				++origin;
			}

			Container cont(temp.size());
			std::copy(temp.begin(), temp.end(), cont.begin());
			return cont;
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

		iterator begin() {
			if(progress >= count) {
				return this->origin.end();
			} else {
				return this->origin.begin();
			}
		}

	private:
		size_t count, progress = 0;
	};

}

#endif
