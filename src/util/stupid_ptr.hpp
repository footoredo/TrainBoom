#ifndef TRAINBOOM_STUPID_PTR_HPP
#define TRAINBOOM_STUPID_PTR_HPP

#include <cstddef>
#include <functional>
#include "exception.hpp"

namespace util {

class brain_is_fucked : public exception {
public:
	brain_is_fucked(): exception(
		"brain_is_fucked",
		"My brain is fucked ^%*^^&*^*&^&$^%#^#%$") {}
};

template<typename T>
struct single_deleter {
	void operator() (T*& ptr) {
		delete ptr;
		ptr = nullptr;
	}
};

template<typename T>
struct array_deleter {
	void operator() (T*& ptr) {
		delete[] ptr;
		ptr = nullptr;
	}
};

template<typename T, class Deleter = single_deleter<T>>
class stupid_ptr {
	friend void swap(stupid_ptr& A, stupid_ptr& B) {
		stupid_ptr tmp(std::move(A));
		A = std::move(B);
		B = std::move(tmp);
	}
public:

	class multiple_reference : public exception {
	public:
		multiple_reference(): exception(
			"multiple_reference",
			"You declared muliple reference to a same object without \
				using stupid_ptr!!!") {}
	};

	class make_copy_to_empty_ptr : public exception {
	public:
		make_copy_to_empty_ptr(): exception(
			"make_copy_to_empty_ptr",
			"You made a copy to a empty stupid_ptr!!!") {}
	};

	constexpr stupid_ptr () noexcept : ptr(nullptr), ref_counter(nullptr) {}
	constexpr stupid_ptr (nullptr_t) noexcept : ptr(nullptr), ref_counter(nullptr) {}
	explicit stupid_ptr (T *ptr) : ptr(ptr) {
		ref_counter = new size_t(1);
	}
	stupid_ptr (const stupid_ptr<T, Deleter>& that) noexcept
		: ptr(that.ptr), ref_counter(that.ref_counter) {
			if (ref_counter) {
				inc();
			}
	}

	~stupid_ptr () {
		if (ref_counter) {
			dec();
		}
	}

	stupid_ptr<T, Deleter>& operator= (const stupid_ptr<T, Deleter>& that) noexcept {
		if (this != &that) {
			if (ref_counter) {
				dec();
			}
			ptr = that.ptr;
			ref_counter = that.ref_counter;
			if (ref_counter) {
				inc();
			}
		}
		return *this;
	}
	stupid_ptr<T, Deleter>& operator= (stupid_ptr<T, Deleter>&& that) noexcept {
		if (this != &that) {
			if (ref_counter) {
				dec();
			}
			ptr = that.ptr;
			ref_counter = that.ref_counter;
			that.ptr = nullptr;
			that.ref_counter = nullptr;
		}
		return *this;
	}

	T& operator* () const {
		return *ptr;
	}
	T* operator-> () const {
		return ptr;
	}
	T& operator[] (size_t pos) const {
		return ptr[pos];
	}

	bool operator== (const stupid_ptr<T>& other_ptr) const {
		return ptr == other_ptr.ptr;
	}
	bool operator!= (const stupid_ptr<T>& other_ptr) const {
		return ptr != other_ptr.ptr;
	}
	bool operator< (const stupid_ptr<T>& other_ptr) const {
		return ptr < other_ptr.ptr;
	}
	bool operator> (const stupid_ptr<T>& other_ptr) const {
		return ptr > other_ptr.ptr;
	}
	bool operator<= (const stupid_ptr<T>& other_ptr) const {
		return ptr <= other_ptr.ptr;
	}
	bool operator>= (const stupid_ptr<T>& other_ptr) const {
		return ptr >= other_ptr.ptr;
	}
	operator bool () const {
		return ptr;
	}

private:
	T *ptr;
	size_t *ref_counter;

	void inc() {
		++ *ref_counter;
	}
	void dec() {
		-- *ref_counter;
		if (*ref_counter == 0) {
			Deleter()(ptr);
			delete ref_counter;
			ref_counter = nullptr;
		}
	}
};

}

#endif
