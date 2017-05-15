#include <iostream>
#include <cstdio>
#include "util/CSV.hpp"
using namespace std;
using namespace trainBoom;
int main(int argc, char *argv[])
{
    using util::CSV::CSV;
	CSV a,b;
	b.load("test/test.csv");
	a=b;
	cout<<a.size()<<' '<<a.size(2)<<endl;
	cout<<a.data(569,1)<<endl;
    std::string tmp = a.data(2,8);
    for (int i = 0; i < (int)tmp.length(); ++ i)
        std::cout << (int)tmp[i] << " ";
    std::cout << std::endl;
	return 0;
}
