#ifndef TRAINBOOM_STUPID_PTR_HPP
#define TRAINBOOM_STUPID_PTR_HPP

#include <cstddef>
#include <functional>

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

	constexpr stupid_ptr();
	stupid_ptr(T *_ptr);
	T* operator->() const;
    T& operator*() const;
	stupid_ptr(const stupid_ptr<T>& other);
	~stupid_ptr();
	stupid_ptr<T>& operator=(T *_ptr);
	stupid_ptr<T>& operator=(const stupid_ptr<T>& other);
	bool operator==(T* other_ptr) const;
	bool operator!=(T* other_ptr) const;
	operator bool() const;

	friend void swap(stupid_ptr<T>& A, stupid_ptr<T>& B);

private:
	T *ptr;
	size_t *ref_counter;

	void inc();  // Increase ref_counter.
	void dec();  // Decrease ref_counter.
};

}

#endif
