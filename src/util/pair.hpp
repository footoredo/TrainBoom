#ifndef TRAINBOOM_UTIL_PAIR_HPP
#define TRAINBOOM_UTIL_PAIR_HPP

namespace TrainBoom {

namespace util {

template<class T1, class T2>
class pair {
public:
	T1 first;
	T2 second;
	constexpr pair() : first(), second() {}
	pair(const pair &other) = default;
	pair(pair &&other) = default;
	pair& operator=(const pair& other) {
		if (this != &other) {
			first = other.first;
			second = other.second;
		}
		return *this;
	}
	pair(const T1 &x, const T2 &y) : first(x), second(y) {}
	template<class U1, class U2>
	pair(U1 &&x, U2 &&y) : first(x), second(y) {}
	template<class U1, class U2>
	pair(const pair<U1, U2> &other) : first(other.first), second(other.second) {}
	template<class U1, class U2>
	pair(pair<U1, U2> &&other) : first(other.first), second(other.second) {}

};

template<class U1, class U2>
pair<U1, U2> make_pair(const U1& x, const U2& y) {
	return pair<U1, U2>(x, y);
}

}	// util

}	// TrainBoom

#endif
