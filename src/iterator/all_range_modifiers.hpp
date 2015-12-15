#ifndef RANGE_MODIFIERS

#include <utility>
#include <type_traits>
#include <vector>
#include <functional>

namespace rust {
	template <class OriginRange>
	class FilteredRange;

	template <class OriginRange>
	class CycledRange;

	template <class OriginRange>
	class TookRange;

	template <class OriginRange>
	class MappedRange;

	template <class OriginRange>
	class InspectedRange;

	template <class OriginRange, class OtherRange>
	class ZippedRange;

	template <class OriginRange, class OtherRange>
	class ChainedRange;

	template <class OriginRange>
	class TookWhileRange;
}

#define RANGE_MODIFIERS                                                                            \
                                                                                                   \
	template <typename FilterFunctor>                                                              \
	FilteredRange<CurrentType> filter(FilterFunctor&& predicate) {                                 \
		static_assert(                                                                             \
		  std::is_convertible<FilterFunctor,                                                       \
		                      std::function<bool(typename CurrentType::value_type)>>::value,       \
		  ".filter() needs a function of type bool(value_type) as first argument");                \
		return FilteredRange<CurrentType>(*this, std::forward<FilterFunctor>(predicate));          \
	}                                                                                              \
                                                                                                   \
	CycledRange<CurrentType> cycle() {                                                             \
		return CycledRange<CurrentType>(*this);                                                    \
	}                                                                                              \
                                                                                                   \
	TookRange<CurrentType> take(size_t size) {                                                     \
		return TookRange<CurrentType>(*this, size);                                                \
	}                                                                                              \
                                                                                                   \
	template <typename MapFunctor>                                                                 \
	MappedRange<CurrentType> map(MapFunctor&& map) {                                               \
		static_assert(                                                                             \
		  std::is_convertible<MapFunctor&&, std::function<typename CurrentType::value_type(        \
		                                      typename CurrentType::value_type)>>::value,          \
		  ".map() needs a function of type value_type(value_type) as first argument");             \
		return MappedRange<CurrentType>(*this, std::forward<MapFunctor>(map));                     \
	}                                                                                              \
                                                                                                   \
	template <typename InspectFunctor>                                                             \
	InspectedRange<CurrentType> inspect(InspectFunctor&& inspectFunc) {                            \
		static_assert(                                                                             \
		  std::is_convertible<InspectFunctor,                                                      \
		                      std::function<void(typename CurrentType::value_type)>>::value,       \
		  ".inspect() needs a function of type void(value_type) as first argument");               \
		return InspectedRange<CurrentType>(*this, std::forward<InspectFunctor>(inspectFunc));      \
	}                                                                                              \
                                                                                                   \
	template <typename MyOtherRange>                                                               \
	ZippedRange<CurrentType, MyOtherRange> zip(MyOtherRange range) {                               \
		return ZippedRange<CurrentType, MyOtherRange>(*this, range);                               \
	}                                                                                              \
                                                                                                   \
	template <typename MyOtherRange>                                                               \
	ChainedRange<CurrentType, MyOtherRange> chain(MyOtherRange range) {                            \
		return ChainedRange<CurrentType, MyOtherRange>(*this, range);                              \
	}                                                                                              \
                                                                                                   \
	template <typename FirstContainer, typename SecondContainer>                                   \
	std::pair<FirstContainer, SecondContainer> unzip() {                                           \
		try {                                                                                      \
			size_t contSize = CurrentType::size();                                                 \
                                                                                                   \
			FirstContainer first(contSize);                                                        \
			SecondContainer second(contSize);                                                      \
                                                                                                   \
			auto firstIt = first.begin(), firstEnd = first.end();                                  \
			auto secondIt = second.begin(), secondEnd = second.end();                              \
                                                                                                   \
			while(firstIt != firstEnd || secondIt != secondEnd) {                                  \
				std::tie(*firstIt, *firstEnd) = CurrentType::currentValue();                       \
				++firstIt;                                                                         \
				++secondIt;                                                                        \
			}                                                                                      \
                                                                                                   \
			return std::make_pair(first, second);                                                  \
                                                                                                   \
		} catch(UnknownValueException) {                                                           \
			using first_type = typename CurrentType::value_type::first_type;                       \
			using second_type = typename CurrentType::value_type::second_type;                     \
                                                                                                   \
			std::vector<first_type> firstTemp;                                                     \
			std::vector<second_type> secondTemp;                                                   \
			auto firstInserter = std::back_inserter(firstTemp);                                    \
			auto secondInserter = std::back_inserter(secondTemp);                                  \
                                                                                                   \
			while(!CurrentType::hasEnded()) {                                                      \
				std::tie(*firstInserter, *secondInserter) = CurrentType::currentValue();           \
				++(*this);                                                                         \
			}                                                                                      \
                                                                                                   \
			if(std::is_same<FirstContainer, std::vector<first_type>>::value &&                     \
			   std::is_same<SecondContainer, std::vector<second_type>>::value) {                   \
				return std::make_pair(firstTemp, secondTemp);                                      \
			} else if(std::is_same<FirstContainer, std::vector<first_type>>::value) {              \
				SecondContainer secondCont(secondTemp.size());                                     \
				std::copy(secondTemp.begin(), secondTemp.end(), secondCont.begin());               \
				return std::make_pair(firstTemp, secondCont);                                      \
			} else if(std::is_same<SecondContainer, std::vector<second_type>>::value) {            \
				FirstContainer firstCont(firstTemp.size());                                        \
				std::copy(firstTemp.begin(), firstTemp.end(), firstCont.begin());                  \
				return std::make_pair(firstCont, secondTemp);                                      \
			} else {                                                                               \
				FirstContainer firstCont(firstTemp.size());                                        \
				SecondContainer secondCont(secondTemp.size());                                     \
				std::copy(firstTemp.begin(), firstTemp.end(), firstCont.begin());                  \
				std::copy(secondTemp.begin(), secondTemp.end(), secondCont.begin());               \
				return std::make_pair(firstCont, secondCont);                                      \
			}                                                                                      \
		}                                                                                          \
	}                                                                                              \
                                                                                                   \
	template <typename FirstContainer, typename SecondContainer, typename PartitionFunctor>        \
	std::pair<FirstContainer, SecondContainer> partition(PartitionFunctor&& predicate) {           \
		static_assert(                                                                             \
		  std::is_convertible<PartitionFunctor,                                                    \
		                      std::function<bool(typename CurrentType::value_type)>>::value,       \
		  ".partition() needs a function of type bool(value_type) as first argument");             \
		/* WTF template here?! */                                                                  \
		FirstContainer first = this->filter(std::forward<PartitionFunctor>(predicate))             \
		                         .template collect<FirstContainer>();                              \
		SecondContainer second = this->filter([predicate](typename CurrentType::value_type el) {   \
			                             return !predicate(el);                                    \
			                         })                                                            \
		                           .template collect<SecondContainer>();                           \
		return std::make_pair(first, second);                                                      \
	}                                                                                              \
                                                                                                   \
	template <typename WhileFunctor>                                                               \
	TookWhileRange<CurrentType> take_while(WhileFunctor&& predicate) {                             \
		static_assert(                                                                             \
		  std::is_convertible<WhileFunctor,                                                        \
		                      std::function<bool(typename CurrentType::value_type)>>::value,       \
		  ".take_while() needs a function of type bool(value_type) as first argument");            \
		return TookWhileRange<CurrentType>(*this, std::forward<WhileFunctor>(predicate));          \
	}                                                                                              \
                                                                                                   \
	template <typename FoldFunctor>                                                                \
	typename CurrentType::value_type fold(typename CurrentType::value_type foldValue,              \
	                                      FoldFunctor&& foldFunc) {                                \
		static_assert(                                                                             \
		  std::is_convertible<FoldFunctor, std::function<typename CurrentType::value_type(         \
		                                     typename CurrentType::value_type,                     \
		                                     typename CurrentType::value_type)>>::value,           \
		  ".fold() needs a function of type value_type(value_type, value_type) as second "         \
		  "argument");                                                                             \
		std::vector<typename CurrentType::value_type> rangeValues =                                \
		  this->template collect<std::vector<typename CurrentType::value_type>>();                 \
		for(typename CurrentType::value_type rangeValue : rangeValues) {                           \
			foldValue = foldFunc(foldValue, rangeValue);                                           \
		}                                                                                          \
		return foldValue;                                                                          \
	}

#endif
