#ifndef RUST_TOOK_RANGE_HPP
#define RUST_TOOK_RANGE_HPP

#include "range_modifier.hpp"
#include "all_range_modifiers.hpp"

#include <iterator>

namespace rust {

	template<class OriginRange>
	class TookRange : public RangeModifier<OriginRange> {

		typedef TookRange<OriginRange>     CurrentType;
		typedef RangeModifier<OriginRange> ParentType;

	public:
		TookRange(OriginRange range, size_t count)
			: ParentType(range), count(count) {
		}

		virtual typename CurrentType::difference_type size() {
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

		typename ParentType::iterator& begin() {
			if(progress >= count) {
				return this->origin.end();
			} else {
				return this->origin.begin();
			}
		}

		typename CurrentType::value_type beginValue() {
			return this->origin.beginValue();
		}

		typename ParentType::iterator& end() {
			return this->origin.end();
		}

	private:
		size_t count, progress = 0;

		template<typename Container>
		Container collectSizeAware(size_t size) {
			Container cont(size);
			for(typename CurrentType::value_type& value: cont) {
				value = beginValue();
				++(*this);
			}
			return cont;
		}

		template<typename Container>
		Container collectSizeUnaware() {
			std::vector<typename CurrentType::value_type> temp;
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
