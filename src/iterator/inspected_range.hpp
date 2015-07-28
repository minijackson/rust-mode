#ifndef RUST_INSPECTED_RANGE_HPP
#define RUST_INSPECTED_RANGE_HPP

#include "range_modifier.hpp"
#include "all_range_modifiers.hpp"

namespace rust {

	template<class OriginRange>
	class InspectedRange : public RangeModifier<OriginRange> {

		typedef std::function<void(typename OriginRange::value_type)> InspectFunc_t;
		typedef InspectedRange<OriginRange> CurrentType;
		typedef RangeModifier<OriginRange>  ParentType;

	public:
		InspectedRange(OriginRange range, InspectFunc_t inspectFunc)
			: ParentType(range, range.hasEnd()), inspectFunc(inspectFunc) {}

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

		typename CurrentType::value_type currentValue() {
			typename CurrentType::value_type value = this->origin.currentValue();
			inspectFunc(value);
			return value;
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
		InspectFunc_t inspectFunc;

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

			while(!this->hasEnded()) {
				*inserter++ = currentValue();
				++(*this);
			}

			// Can't use template specialization in this case but I can do
			// THIS:
		    if(std::is_same<
		           Container,
		           std::vector<typename CurrentType::value_type>>::value) {
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
