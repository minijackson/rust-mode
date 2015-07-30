#ifndef RANGE_MODIFIERS

#include <utility>
#include <type_traits>
#include <vector>
#include <functional>

namespace rust {
template <class OriginRange> class FilteredRange;

template <class OriginRange> class CycledRange;

template <class OriginRange> class TookRange;

template <class OriginRange> class MappedRange;

template <class OriginRange> class InspectedRange;

template <class OriginRange, class OtherRange> class ZippedRange;

template <class OriginRange, class OtherRange> class ChainedRange;
}

#define RANGE_MODIFIERS                                                        \
	FilteredRange<CurrentType> filter(                                         \
	    std::function<bool(typename CurrentType::value_type)> predicate) {     \
		return FilteredRange<CurrentType>(*this, predicate);                   \
	}                                                                          \
	CycledRange<CurrentType> cycle() {                                         \
		return CycledRange<CurrentType>(*this);                                \
	}                                                                          \
	TookRange<CurrentType> take(size_t size) {                                 \
		return TookRange<CurrentType>(*this, size);                            \
	}                                                                          \
	MappedRange<CurrentType> map(                                              \
	    std::function<typename CurrentType::value_type(                        \
	        typename CurrentType::value_type)> map) {                          \
		return MappedRange<CurrentType>(*this, map);                           \
	}                                                                          \
	InspectedRange<CurrentType> inspect(                                       \
	    std::function<void(typename CurrentType::value_type)> inspectFunc) {   \
		return InspectedRange<CurrentType>(*this, inspectFunc);                \
	}                                                                          \
	template <typename MyOtherRange>                                           \
	ZippedRange<CurrentType, MyOtherRange> zip(MyOtherRange range) {           \
		return ZippedRange<CurrentType, MyOtherRange>(*this, range);           \
	}                                                                          \
	template <typename MyOtherRange>                                           \
	ChainedRange<CurrentType, MyOtherRange> chain(MyOtherRange range) {        \
		return ChainedRange<CurrentType, MyOtherRange>(*this, range);          \
	}                                                                          \
	template <typename FirstContainer, typename SecondContainer>               \
	std::pair<FirstContainer, SecondContainer> unzip() {                       \
		try {                                                                  \
			size_t contSize = CurrentType::size();                             \
                                                                               \
			FirstContainer first(contSize);                                    \
			SecondContainer second(contSize);                                  \
                                                                               \
			auto firstIt = first.begin(), firstEnd = first.end();              \
			auto secondIt = second.begin(), secondEnd = second.end();          \
                                                                               \
			while(firstIt != firstEnd || secondIt != secondEnd) {              \
				std::tie(*firstIt, *firstEnd) = CurrentType::currentValue();   \
				++firstIt;                                                     \
				++secondIt;                                                    \
			}                                                                  \
                                                                               \
			return std::make_pair(first, second);                              \
                                                                               \
		} catch(UnknownValueException) {                                       \
			using first_type = typename CurrentType::value_type::first_type;   \
			using second_type = typename CurrentType::value_type::second_type; \
                                                                               \
			std::vector<first_type> firstTemp;                                 \
			std::vector<second_type> secondTemp;                               \
			auto firstInserter = std::back_inserter(firstTemp);                \
			auto secondInserter = std::back_inserter(secondTemp);              \
                                                                               \
			while(!CurrentType::hasEnded()) {                                  \
				std::tie(*firstInserter, *secondInserter) =                    \
				    CurrentType::currentValue();                               \
				++(*this);                                                     \
			}                                                                  \
                                                                               \
			if(std::is_same<FirstContainer, std::vector<first_type>>::value && \
			   std::is_same<SecondContainer,                                   \
			                std::vector<second_type>>::value) {                \
				return std::make_pair(firstTemp, secondTemp);                  \
			} else if(std::is_same<FirstContainer,                             \
			                       std::vector<first_type>>::value) {          \
				SecondContainer secondCont(secondTemp.size());                 \
				std::copy(secondTemp.begin(), secondTemp.end(),                \
				          secondCont.begin());                                 \
				return std::make_pair(firstTemp, secondCont);                  \
			} else if(std::is_same<SecondContainer,                            \
			                       std::vector<second_type>>::value) {         \
				FirstContainer firstCont(firstTemp.size());                    \
				std::copy(firstTemp.begin(), firstTemp.end(),                  \
				          firstCont.begin());                                  \
				return std::make_pair(firstCont, secondTemp);                  \
			} else {                                                           \
				FirstContainer firstCont(firstTemp.size());                    \
				SecondContainer secondCont(secondTemp.size());                 \
				std::copy(firstTemp.begin(), firstTemp.end(),                  \
				          firstCont.begin());                                  \
				std::copy(secondTemp.begin(), secondTemp.end(),                \
				          secondCont.begin());                                 \
				return std::make_pair(firstCont, secondCont);                  \
			}                                                                  \
		}                                                                      \
	}                                                                          \
	template <typename FirstContainer, typename SecondContainer>               \
	std::pair<FirstContainer, SecondContainer> partition(                      \
	    std::function<bool(typename CurrentType::value_type)> predicate) {     \
		/* WTF template here?! */                                              \
		FirstContainer first =                                                 \
		    this->filter(predicate).template collect<FirstContainer>();        \
		SecondContainer second =                                               \
		    this->filter([predicate](typename CurrentType::value_type el) {    \
			return !predicate(el);                                             \
			}).template collect<SecondContainer>();                            \
		return std::make_pair(first, second);                                  \
	}

#endif
