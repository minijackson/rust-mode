#ifndef RUST_MAPPED_RANGE_HPP
#define RUST_MAPPED_RANGE_HPP

#include "range_modifier.hpp"
#include "all_range_modifiers.hpp"

#include <vector>
#include <functional>
#include <iterator>

namespace rust {

	template<class OriginRange>
	class MappedRange : public RangeModifier<OriginRange> {

		typedef std::function<typename OriginRange::value_type(typename OriginRange::value_type)> Map_t;
		typedef MappedRange<OriginRange>   CurrentType;
		typedef RangeModifier<OriginRange> ParentType;

	public:

		MappedRange(OriginRange range, Map_t mapFunc)
			: ParentType(range), mapFunc(mapFunc) {}

		virtual typename CurrentType::difference_type size() {
			return this->origin.size();
		}

		virtual bool empty() {
			return this->origin.empty();
		}

		RANGE_MODIFIERS

		template<typename Container>
		Container collect() {
			try {
				size_t contSize = size();
				return collectSizeAware<Container>(contSize);
			} catch(UnknownValueException) {
				return collectSizeUnaware<Container>();
			}
		}

		typename ParentType::iterator& begin() {
			return this->origin.begin();
		}

		typename CurrentType::value_type beginValue() {
			return mapFunc(this->origin.beginValue());
		}

		typename ParentType::iterator& end() {
			return this->origin.end();
		}

		CurrentType& operator++() {
			++this->origin;
			return *this;
		}

		CurrentType& operator++(int) {
			CurrentType other = *this;
			++this->origin;
			return other;
		}

	private:
		Map_t mapFunc;

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

			while(begin() != end()) {
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
