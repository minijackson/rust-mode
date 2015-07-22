#ifndef RANGE_MODIFIERS

#include <utility>

namespace rust {
	template<
		class OriginRange,
		class T,
		class Distance,
		class Pointer,
		class Reference
	> class FilteredRange;

	template<
		class OriginRange,
		class T,
		class Distance,
		class Pointer,
		class Reference
	> class CycledRange;

	template<
		class OriginRange,
		class T,
		class Distance,
		class Pointer,
		class Reference
	> class TookRange;

	template<
		class OriginRange,
		class T,
		class Distance,
		class Pointer,
		class Reference
	> class MappedRange;

	template<
		class OriginRange,
		class T,
		class Distance,
		class Pointer,
		class Reference
	> class InspectedRange;

	template<
		class OriginRange,
		class OtherRange,
		class T,
		class Distance,
		class Pointer,
		class Reference
	> class ZippedRange;

}

#define RANGE_MODIFIERS \
		FilteredRange<CurrentType, T, Distance, Pointer, Reference> \
		filter(std::function<bool(T)> predicate) { \
			return FilteredRange<CurrentType, T, Distance, Pointer, Reference>(*this, predicate); \
		} \
		CycledRange<CurrentType, T, Distance, Pointer, Reference> \
		cycle() { \
			return CycledRange<CurrentType, T, Distance, Pointer, Reference>(*this); \
		} \
		TookRange<CurrentType, T, Distance, Pointer, Reference> \
		take(size_t size) { \
			return TookRange<CurrentType, T, Distance, Pointer, Reference>(*this, size); \
		} \
		MappedRange<CurrentType, T, Distance, Pointer, Reference> \
		map(std::function<T(T)> map) { \
			return MappedRange<CurrentType, T, Distance, Pointer, Reference>(*this, map); \
		} \
		InspectedRange<CurrentType, T, Distance, Pointer, Reference> \
		inspect(std::function<void(T)> inspectFunc) { \
			return InspectedRange<CurrentType, T, Distance, Pointer, Reference>(*this, inspectFunc); \
		} \
		template<typename MyOtherRange> \
		ZippedRange<CurrentType, MyOtherRange, std::pair<T, typename MyOtherRange::value_type>, Distance, Pointer, Reference> \
		zip(MyOtherRange range) { \
			return ZippedRange<CurrentType, MyOtherRange, std::pair<T, typename MyOtherRange::value_type>, Distance, Pointer, Reference>(*this, range); \
		}
		
#endif
