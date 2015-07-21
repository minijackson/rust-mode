#ifndef RANGE_MODIFIERS

namespace rust {
	template<
		class OriginRange,
		class iterator,
		class Category,
		class T,
		class Distance,
		class Pointer,
		class Reference
	> class FilteredRange;

	template<
		class OriginRange,
		class iterator,
		class Category,
		class T,
		class Distance,
		class Pointer,
		class Reference
	> class CycledRange;

	template<
		class OriginRange,
		class iterator,
		class Category,
		class T,
		class Distance,
		class Pointer,
		class Reference
	> class TookRange;

	template<
		class OriginRange,
		class iterator,
		class Category,
		class T,
		class Distance,
		class Pointer,
		class Reference
	> class MappedRange;

	template<
		class OriginRange,
		class iterator,
		class Category,
		class T,
		class Distance,
		class Pointer,
		class Reference
	> class InspectedRange;

}

#define RANGE_MODIFIERS \
		FilteredRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference> \
		filter(std::function<bool(T)> predicate) { \
			return FilteredRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>(*this, predicate); \
		} \
		CycledRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference> \
		cycle() { \
			return CycledRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>(*this); \
		} \
		TookRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference> \
		take(size_t size) { \
			return TookRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>(*this, size); \
		} \
		MappedRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference> \
		map(std::function<T(T)> map) { \
			return MappedRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>(*this, map); \
		} \
		InspectedRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference> \
		inspect(std::function<void(T)> inspectFunc) { \
			return InspectedRange<CurrentType, iterator, Category, T, Distance, Pointer, Reference>(*this, inspectFunc); \
		}
		
#endif
