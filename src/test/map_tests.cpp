#include <iostream>
#include "util/map.hpp"

using namespace TrainBoom;
using namespace util;

class Integer {
public:
	static int counter;
	int val;

	Integer(int val) : val(val) {
		counter++;
	}

	Integer(const Integer &rhs) {
		val = rhs.val;
		counter++;
	}

	Integer& operator = (const Integer &rhs) {
        throw;
        return *this;
		// assert(false);
	}

	operator int() const {
		return val;
	}

	void test() const {

	}

	~Integer() {
		// std::cout << "DIE!" << std::endl;
		// std::cout << val << " dies" << std::endl;
		counter--;
	}
};

int Integer::counter = 0;

void print_all(const map<Integer, int>& M) {
    std::cout << "---" << std::endl;
    std::cout << "size: " << M.size() << std::endl;
    for (map<Integer, int>::iterator it = M.begin(); it != M.end(); ++ it) {
        std::cout << "(" << it->first << ", " << it->second << ")" << " ";
    }
    std::cout << std::endl << "---" << std::endl;
}

void test() {
    map<Integer, int> M;
    auto t = M.insert(make_pair(Integer(3), 3)); std::cout << t.first->first << " " << t.second << std::endl;
    std::cout << "min: " << M.begin()->first << std::endl;
    print_all(M);
    t = M.insert(make_pair(Integer(5), 6)); std::cout << t.first->first << " " << t.second << std::endl;
    std::cout << "min: " << M.begin()->first << std::endl;
    print_all(M);

    std::cout << M[Integer(5)] << std::endl;
    print_all(M);
    M[Integer(6)] = 7;

    print_all(M);

    t = M.insert(make_pair(Integer(2), 400)); std::cout << t.first->first << " " << t.second << std::endl;
    std::cout << "min: " << M.begin()->first << std::endl;
    print_all(M);



    map<Integer, int> MM(M);
    std::cout << "new min: " << M.begin()->first << std::endl;
    print_all(MM);
    print_all(M);

    std::cout << std::endl << std::endl;
    M.erase(t.first);
    print_all(M);

    M.erase(M.begin());
    print_all(M);


    std::cout << std::endl << std::endl;
    map<Integer, int>::iterator tmp = M.end();
    -- tmp;
    std::cout << "!!" << std::endl;
    std::cout << tmp->first << " " << tmp->second << std::endl;
}

int main() {
    test();
    std::cout << Integer::counter << std::endl;
}
