#ifndef RUST_MAPPED_RANGE_HPP
#define RUST_MAPPED_RANGE_HPP

#include "range_modifier.hpp"
#include "all_range_modifiers.hpp"

#include <vector>
#include <functional>
#include <iterator>

namespace rust {

	template<
		class OriginRange,
		class T         = typename OriginRange::value_type,
		class Distance  = std::ptrdiff_t,
		class Pointer   = T*,
		class Reference = T&
	> class MappedRange : public RangeModifier<OriginRange, T, Distance, Pointer, Reference> {

		typedef std::function<T(T)> Map_t;
		typedef MappedRange<OriginRange, T, Distance, Pointer, Reference> CurrentType;
		typedef RangeModifier<OriginRange, T, Distance, Pointer, Reference> ParentType;

	public:

		MappedRange(OriginRange range, Map_t mapFunc)
			: ParentType(range), mapFunc(mapFunc) {}

		virtual Distance size() {
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

		T beginValue() {
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
