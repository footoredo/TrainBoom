#ifndef TRAINBOOM_STUPID_PTR_HPP
#define TRAINBOOM_STUPID_PTR_HPP

#include <cstddef>
#include <functional>
#include <iostream>
#include "exception.hpp"

namespace TrainBoom {

namespace util {

class brain_is_fucked : public exception {
public:
	brain_is_fucked(): exception(
		"brain_is_fucked",
		"My brain is fucked ^%*^^&*^*&^&$^%#^#%$") {}
};
//
// template<typename T>
// struct single_deleter {
// 	void operator() (T*& ptr) {
// 		delete ptr;
// 		ptr = nullptr;
// 	}
// };
//
// template<typename T>
// struct array_deleter {
// 	void operator() (T[]& ptr) {
// 		delete[] ptr;
// 		ptr = nullptr;
// 	}
// };

template<class ptrT>
class stupid_base {
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

	constexpr stupid_base () noexcept : ptr(nullptr), ref_counter(nullptr) {}
	constexpr stupid_base (nullptr_t) noexcept : ptr(nullptr), ref_counter(nullptr) {}
	stupid_base (ptrT ptr) : ptr(ptr) {
		ref_counter = new size_t(1);
	}
	stupid_base (const stupid_base<ptrT>& that) noexcept
		: ptr(that.ptr), ref_counter(that.ref_counter) {
			if (ref_counter) {
				inc();
			}
	}

	virtual ~stupid_base () {

	};

	stupid_base<ptrT>& operator= (const stupid_base<ptrT>& that) noexcept {
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
	stupid_base<ptrT>& operator= (stupid_base<ptrT>&& that) noexcept {
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

	bool operator== (const stupid_base<ptrT>& other_ptr) const {
		return ptr == other_ptr.ptr;
	}
	bool operator!= (const stupid_base<ptrT>& other_ptr) const {
		return ptr != other_ptr.ptr;
	}
	bool operator< (const stupid_base<ptrT>& other_ptr) const {
		return ptr < other_ptr.ptr;
	}
	bool operator> (const stupid_base<ptrT>& other_ptr) const {
		return ptr > other_ptr.ptr;
	}
	bool operator<= (const stupid_base<ptrT>& other_ptr) const {
		return ptr <= other_ptr.ptr;
	}
	bool operator>= (const stupid_base<ptrT>& other_ptr) const {
		return ptr >= other_ptr.ptr;
	}
	operator bool () const {
		return ptr;
	}

protected:
	ptrT ptr;
	size_t *ref_counter;
	void inc() {
		++ *ref_counter;
	}
	void dec() {
		// std::cout << "before: " << *ref_counter << std::endl;
		-- *ref_counter;
		if (*ref_counter == 0) {
			free_memory();
			delete ref_counter;
			ref_counter = nullptr;
		}
	}

private:
	virtual void free_memory() = 0;
};

template<class T>
class stupid_ptr : public stupid_base<T*> {
	friend void swap(stupid_ptr& A, stupid_ptr& B) {
		stupid_ptr tmp(std::move(A));
		A = std::move(B);
		B = std::move(tmp);
	}

public:
	constexpr stupid_ptr() noexcept : stupid_base<T*>()  {}
	constexpr stupid_ptr(nullptr_t) noexcept : stupid_base<T*>(nullptr) {}
	stupid_ptr(T* ptr): stupid_base<T*>(ptr) {}

	T& operator* () const {
		return *(this->ptr);
	}
	T* operator-> () const {
		return this->ptr;
	}

	virtual ~stupid_ptr () {
		if (this->ref_counter) {
			this->dec();
		}
	}

private:
	virtual void free_memory() {
		// std::cout << "!!! ptr" << std::endl;
		delete this->ptr;
		this->ptr = nullptr;
	}
};

template<class T>
class stupid_array : public stupid_base<T*> {
	friend void swap(stupid_array& A, stupid_array& B) {
		stupid_array tmp(std::move(A));
		A = std::move(B);
		B = std::move(tmp);
	}
public:
	class index_out_of_range : public exception {
	public:
		index_out_of_range(): exception(
			"index_out_of_range",
			"You index is out of range!!") {}
	};

	constexpr stupid_array() noexcept : stupid_base<T*>() {}
	constexpr stupid_array(nullptr_t) noexcept : stupid_base<T*>(nullptr) {}
	stupid_array(T* ptr, size_t n): stupid_base<T*>(ptr), n(n) {}
	stupid_array (const stupid_array<T>& that) noexcept
		: stupid_base<T*>(that), n(that.n) {}

	virtual ~stupid_array () {
		if (this->ref_counter) {
			this->dec();
		}
	}

	stupid_array<T>& operator= (const stupid_array<T>& that) noexcept {
		if (this != &that) {
			if (this->ref_counter) {
				this->dec();
			}
			this->ptr = that.ptr;
			this->ref_counter = that.ref_counter;
			n = that.n;
			if (this->ref_counter) {
				this->inc();
			}
		}
		return *this;
	}
	stupid_array<T>& operator= (stupid_array<T>&& that) noexcept {
		if (this != &that) {
			if (this->ref_counter) {
				this->dec();
			}
			this->ptr = that.ptr;
			this->ref_counter = that.ref_counter;
			n = that.n;
			that.n = 0;
			that.ptr = nullptr;
			that.ref_counter = nullptr;
		}
		return *this;
	}

	T& operator[] (size_t pos) const {
		if (pos >= n) {
			throw index_out_of_range();
		}
		return this->ptr[pos];
	}

	size_t size() const {
		return n;
	}

private:
	size_t n;
	virtual void free_memory() {
		// std::cout << "!!! array" << std::endl;
		// for (size_t i = 0; i < n; ++ i) {
		// 	std::cout << i << " " << (this->ptr + i) - this->ptr << std::endl;
		// 	delete (this->ptr + i);
		// }
		delete [] this->ptr;
		this->ptr = nullptr;
		n = 0;
	}
};

template <class T, class... Args>
stupid_ptr<T> make_stupid(Args&&... args) {
    return stupid_ptr<T>(new T(args...));
}

template <class T>
stupid_ptr<T> make_stupid() {
    return stupid_ptr<T>(new T());
}

}	// util

using util::stupid_ptr;
using util::stupid_array;
using util::make_stupid;

}	// TrainBoom

#endif
