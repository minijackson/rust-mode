#ifndef RUST_TOOK_WHILE_RANGE_HPP
#define RUST_TOOK_WHILE_RANGE_HPP

#include "range_modifier.hpp"
#include "all_range_modifiers.hpp"

#include <iterator>

namespace rust {

	template<class OriginRange>
	class TookWhileRange : public RangeModifier<OriginRange> {

		typedef std::function<bool(typename OriginRange::value_type)> Predicate_t;
		typedef TookWhileRange<OriginRange> CurrentType;
		typedef RangeModifier<OriginRange>  ParentType;

	public:
	  TookWhileRange(OriginRange range, Predicate_t predicate)
	      : ParentType(range, true),
	        predicate(predicate),
	        value(range.currentValue()),
			ended(!predicate(value)) {}

	    virtual typename CurrentType::difference_type size() {
			throw UnknownValueException("Cannot know the size of a \"tooked while\" range before consuming values");
		}

		virtual bool empty() {
			throw UnknownValueException("Cannot know if a \"took while\" range is empty before consuming values");
		}

		virtual bool hasEnded() {
			return ended || this->origin.hasEnded();
		}

		RANGE_MODIFIERS

		template<typename Container>
		Container collect() {
			std::vector<typename CurrentType::value_type> temp;
			auto inserter = std::back_inserter(temp);

			while(!hasEnded()) {
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

		CurrentType& operator++() {
			value = (++this->origin).currentValue();

			if(!predicate(value)) {
				ended = true;
			}

			return *this;
		}

		CurrentType operator++(int) {
			CurrentType other = *this;
			value = (++this->origin).currentValue();

			if(!predicate(value)) {
				ended = true;
			}

			return other;
		}

		typename CurrentType::value_type currentValue() {
			return value;
		}

	private:
		Predicate_t predicate;
		typename CurrentType::value_type value;
		bool ended;
	};

}

#endif
