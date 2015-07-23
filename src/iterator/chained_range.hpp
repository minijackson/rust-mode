#ifndef RUST_CHAINED_RANGE_HPP
#define RUST_CHAINED_RANGE_HPP

#include "range_modifier.hpp"
#include "all_range_modifiers.hpp"

#include <type_traits>
#include <vector>

namespace rust {

	template<class OriginRange, class OtherRange>
	class ChainedRange : public RangeModifier<OriginRange> {

		typedef ChainedRange<OriginRange, OtherRange>  CurrentType;
		typedef RangeModifier<OriginRange>             ParentType;

	public:
		ChainedRange(OriginRange range, OtherRange other)
			: ParentType(range, range.hasEnd() && other.hasEnd()), other(other) {}

		virtual typename CurrentType::difference_type size() {
			return this->origin.size() + other.size();
		}

		virtual bool empty() {
			return this->origin.empty() && other.empty();
		}

		virtual bool hasEnded() {
			return other.hasEnded();
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

		CurrentType& operator++() {
			advance();
			return *this;
		}

		CurrentType operator++(int) {
			CurrentType other;
			advance();
			return other;
		}

		typename CurrentType::value_type currentValue() {
			if(isIteratingOther) {
				return other.currentValue();
			} else {
				return this->origin.currentValue();
			}
		}

	private:
		OtherRange other;
		bool isIteratingOther = false;

		void advance() {
			if(isIteratingOther) {
				++other;
			} else {
				++this->origin;
				if(this->origin.hasEnded()) {
					isIteratingOther = true;
				}
			}
		}

		template<typename Container>
		Container collectSizeAware(size_t size) {
			Container cont(size);
			for(typename CurrentType::value_type& value: cont) {
				value = currentValue();
				++(*this);
			}
			return cont;
		}

		template<typename Container>
		Container collectSizeUnaware() {
			std::vector<typename CurrentType::value_type> temp;
			auto inserter = std::back_inserter(temp);

			while(hasEnded()) {
				*inserter++ = currentValue();
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
