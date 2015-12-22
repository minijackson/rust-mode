#include "sequence.hpp"

using namespace rust;

SequenceIterator::SequenceIterator()
      : infinite(true) {}

SequenceIterator::SequenceIterator(int value)
	: value(value) {}

SequenceIterator::reference SequenceIterator::operator*() {
	if(infinite) {
		throw InfiniteRangeException();
	} else {
		return value;
	}
}

bool SequenceIterator::operator==(SequenceIterator other) {
	if(infinite || other.infinite) {
		return false;
	} else {
		return value == other.value;
	}
}

bool SequenceIterator::operator!=(SequenceIterator other) {
	if(infinite || other.infinite) {
		return true;
	} else {
		return value != other.value;
	}
}

SequenceIterator& SequenceIterator::operator++() {
	++value;
	return *this;
}

SequenceIterator SequenceIterator::operator++(int) {
	SequenceIterator other = *this;
	++value;
	return other;
}

SequenceIterator& SequenceIterator::operator--() {
	--value;
	return *this;
}

SequenceIterator SequenceIterator::operator--(int) {
	SequenceIterator other = *this;
	--value;
	return other;
}

SequenceIterator& SequenceIterator::operator+=(difference_type i) {
	value += i;
	return *this;
}

SequenceIterator SequenceIterator::operator+(difference_type i) {
	SequenceIterator other = *this;
	return other += i;
}

SequenceIterator& SequenceIterator::operator-=(difference_type i) {
	value -= i;
	return *this;
}

SequenceIterator SequenceIterator::operator-(difference_type i) {
	SequenceIterator other = *this;
	return other -= i;
}

SequenceIterator::difference_type SequenceIterator::operator-(SequenceIterator other) {
	return value - other.value;
}

SequenceIterator::value_type SequenceIterator::operator[](difference_type i) {
	return *(*this + i);
}

bool SequenceIterator::operator<(SequenceIterator other) {
	if((infinite && other.infinite) || infinite) {
		return false;
	} else if(other.infinite) {
		return true;
	} else {
		return value < other.value;
	}
}

bool SequenceIterator::operator>(SequenceIterator other) {
	if((infinite && other.infinite) || other.infinite) {
		return false;
	} else if(infinite) {
		return true;
	} else {
		return value > other.value;
	}
}

bool SequenceIterator::operator<=(SequenceIterator other) {
	if((infinite && other.infinite) || other.infinite) {
		return true;
	} else if(infinite) {
		return false;
	} else {
		return value <= other.value;
	}
}

bool SequenceIterator::operator>=(SequenceIterator other) {
	if((infinite && other.infinite) || infinite) {
		return true;
	} else if(other.infinite) {
		return false;
	} else {
		return value >= other.value;
	}
}

Sequence::Sequence()
	: Iterator<SequenceIterator>(SequenceIterator(0)) {}

Sequence::Sequence(int begin)
	: Iterator<SequenceIterator>(SequenceIterator(begin)) {}

Sequence::Sequence(int begin, int end)
	: Iterator<SequenceIterator>(SequenceIterator(begin), SequenceIterator(end)) {}
