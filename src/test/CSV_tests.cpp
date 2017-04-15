#include <iostream>
#include <cstdio>
#include "util/CSV.hpp"
using namespace std;
using namespace TrainBoom;
int main(int argc, char *argv[])
{
    using util::CSV::CSV;
	CSV a,b;
	b.load("test/test.csv");
	a=b;
	cout<<a.size()<<' '<<a.size(2)<<endl;
	cout<<a.data(569,1)<<endl;
	return 0;
}
