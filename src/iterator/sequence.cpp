#include "sequence.hpp"

using namespace rust;

SequenceIterator::SequenceIterator()
	: value(1) {}

SequenceIterator::SequenceIterator(int value)
	: value(value) {}

SequenceIterator::reference SequenceIterator::operator*() {
	return value;
}

bool SequenceIterator::operator==(SequenceIterator other) {
	return value == other.value;
}

bool SequenceIterator::operator!=(SequenceIterator other) {
	return value != other.value;
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

SequenceIterator::reference SequenceIterator::operator[](difference_type i) {
	return *(*this + i);
}

bool SequenceIterator::operator<(SequenceIterator other) {
	return value < other.value;
}

bool SequenceIterator::operator>(SequenceIterator other) {
	return value > other.value;
}

bool SequenceIterator::operator<=(SequenceIterator other) {
	return value <= other.value;
}

bool SequenceIterator::operator>=(SequenceIterator other) {
	return value >= other.value;
}

Sequence::Sequence()
	: Iterator<SequenceIterator>(SequenceIterator(1)) {}

Sequence::Sequence(int begin)
	: Iterator<SequenceIterator>(SequenceIterator(begin)) {}

Sequence::Sequence(int begin, int end)
	: Iterator<SequenceIterator>(SequenceIterator(begin), SequenceIterator(end)) {}
