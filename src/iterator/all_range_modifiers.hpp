#ifndef RANGE_MODIFIERS

#include <utility>

namespace rust {
	template<class OriginRange>
		class FilteredRange;

	template<class OriginRange>
	class CycledRange;

	template<class OriginRange>
	class TookRange;

	template<class OriginRange>
	class MappedRange;

	template<class OriginRange>
	class InspectedRange;

	template<class OriginRange, class OtherRange>
	class ZippedRange;

}

#define RANGE_MODIFIERS \
		FilteredRange<CurrentType> \
		filter(std::function<bool(typename CurrentType::value_type)> predicate) { \
			return FilteredRange<CurrentType>(*this, predicate); \
		} \
		CycledRange<CurrentType> \
		cycle() { \
			return CycledRange<CurrentType>(*this); \
		} \
		TookRange<CurrentType> \
		take(size_t size) { \
			return TookRange<CurrentType>(*this, size); \
		} \
		MappedRange<CurrentType> \
		map(std::function<typename CurrentType::value_type(typename CurrentType::value_type)> map) { \
			return MappedRange<CurrentType>(*this, map); \
		} \
		InspectedRange<CurrentType> \
		inspect(std::function<void(typename CurrentType::value_type)> inspectFunc) { \
			return InspectedRange<CurrentType>(*this, inspectFunc); \
		} \
		template<typename MyOtherRange> \
		ZippedRange<CurrentType, MyOtherRange> \
		zip(MyOtherRange range) { \
			return ZippedRange<CurrentType, MyOtherRange>(*this, range); \
		}
		
#endif
