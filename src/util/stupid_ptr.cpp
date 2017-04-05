#include "stupid_ptr.hpp"

#include <cstddef>
#include <functional>

using namespace util;

template<typename T, class Deleter = single_deleter<T>>

constexpr stupid_ptr::stupid_ptr(): ptr(nullptr), ref_counter(nullptr) {}
stupid_ptr::stupid_ptr(T *_ptr): ptr(_ptr), ref_counter(nullptr) {
	if (ptr) {
		ref_counter = new size_t(1);
	}
}
stupid_ptr::stupid_ptr(const stupid_ptr<T>& other):
	ptr(other.ptr), ref_counter(other.ref_counter) {
		if (ref_counter) {
			inc();
		}
}
stupid_ptr<T>& stupid_ptr::operator=(T *_ptr) {
    if (ptr == _ptr) {
        throw multiple_reference();
    }
	if (ref_counter) {
		dec();
	}
	ptr = _ptr;
	if (ptr) {
	 	ref_counter = new size_t(1);
	}
	return *this;
}
stupid_ptr<T>& stupid_ptr::operator=(const stupid_ptr<T>& other) {
	if (ref_counter) {
		dec();
	}
	ptr = other.ptr;
	ref_counter = other.ref_counter;
	if (ref_counter) {
		inc();
	}
	return *this;
}

T* stupid_ptr::operator->() const {
	return ptr;
}
T& stupid_ptr::operator*() const {
	return *ptr;
}

stupid_ptr::~stupid_ptr() {
	if (ref_counter) {
		dec();
	}
}

bool stupid_ptr::operator==(T* other_ptr) const {
	return ptr == other_ptr;
}
bool stupid_ptr::operator!=(T* other_ptr) const {
	return ptr != other_ptr;
}
operator stupid_ptr::bool() const {
	return ptr;
}
friend void stupid_ptr::swap(stupid_ptr<T>& A, stupid_ptr<T>& B) {
	using std::swap;
	swap(A.ptr, B.ptr);
	swap(A.ref_counter, B.ref_counter);
}

void stupid_ptr::inc() {
	++ *ref_counter;
}
void stupid_ptr::dec() {
	-- *ref_counter;
	if (*ref_counter == 0) {
		Deleter()(ptr);
		delete ref_counter;
		ref_counter = nullptr;
	}
}
