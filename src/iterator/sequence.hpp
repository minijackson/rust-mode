#ifndef RUST_SEQUENCE_HPP
#define RUST_SEQUENCE_HPP

#include "iterator.hpp"

#include <iterator>

namespace rust {

	class SequenceIterator : public std::iterator<std::random_access_iterator_tag, int> {
	public:
		SequenceIterator();
		SequenceIterator(int value);

		reference operator*();

		bool operator==(SequenceIterator other);
		bool operator!=(SequenceIterator other);

		SequenceIterator& operator++();
		SequenceIterator operator++(int);
		SequenceIterator& operator--();
		SequenceIterator operator--(int);

		SequenceIterator& operator+=(difference_type i);
		SequenceIterator operator+(difference_type i);
		SequenceIterator& operator-=(difference_type i);
		SequenceIterator operator-(difference_type i);

		difference_type operator-(SequenceIterator other);

		reference operator[](difference_type i);

		bool operator<(SequenceIterator other);
		bool operator>(SequenceIterator other);
		bool operator<=(SequenceIterator other);
		bool operator>=(SequenceIterator other);

	private:
		int value;
	};

	class Sequence : public Iterator<SequenceIterator> {
		typedef Sequence CurrentType;
	public:
		Sequence();
		Sequence(int begin);
		Sequence(int begin, int end);
	};
}

#endif
