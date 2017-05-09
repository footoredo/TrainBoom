#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cassert>
#include "../BinaryFile.hpp"
using namespace std;
using namespace TrainBoom;
struct foo{
	int a,b,c;
	char t[100];
//	string s; //not ok
	foo(){
	}
	foo(int _a,int _b,int _c):a(_a),b(_b),c(_c){
		int i,tp=rand()%40;
		for (i=0;i<tp;++i)
		{
			t[i]=(char)(rand()%26+65);
		}
		t[tp]='\0';
	}
	void print()
	{
		cout<<a<<' '<<b<<' '<<c<<' '<<t<<' '<<endl;
	}
};
int main()
{	
	BinaryFile asd("t.dat");
	asd.Write(std::string("first"));
	asd.Write(std::string("second"));
	asd.Write(std::string("third"));
	std::string bbuf;
	asd.Read(bbuf);
	cout<<bbuf<<endl;
	asd.Close();
	
	BinaryFile asdas("ttt.dat");
	asdas.Write(std::string("123546611123"));
	std::string tmpstr; asdas.Read(tmpstr);
	std::cout << tmpstr << std::endl;
	assert(tmpstr == "123546611123");
	asdas.Close();
	
	int w=5;
	BinaryFile tt("hhh.dat");
	tt.Write(w);
	tt.Read(w);
	cout<<w<<endl;
	srand(time(0));
	foo a(3,6,4),c(7,9,2);
	foo b,d;
	BinaryFile tmp("hhh.dat");
	a.print();
	tmp.Write(&a,sizeof(a));
	BinaryFile rd("hhh.dat");
	rd.Read(&b,sizeof(b));
	b.print();
	c.print();
	tmp.Write(&c,sizeof(c));
	rd.Read(&d,sizeof(d));
	d.print();
	foo arr[3],arrr[3];
	for (int i=0;i<3;++i)
	{
		arr[i]=foo(rand()%30,rand()%30,rand()%30);
		arr[i].print();
	}
	BinaryFile test("hhh.dat");
	test.Write(arr,sizeof(arr));
	BinaryFile testr("hhh.dat");
	test.Read(arrr,sizeof(arrr));
	for (int i=0;i<3;++i)
	{
		arrr[i].print();
	}
	string str="test",buf;
	str+='\0';
	str+="apple";
	str+='\0';
	BinaryFile s("str.dat");
	s.Write(str);
	s.Read(str);
	s.Write(str);
	s.Close();
	BinaryFile r("str.dat");
	r.Read(buf);
	cout<<str<<endl<<buf<<endl<<buf.size()<<'/'<<str.size()<<endl;
	assert(!buf.compare(str));

//	s.Write(&str,sizeof(str));		//not ok
//	s.Read(&buf,sizeof(buf));		//not ok

	string astr[3]={"hello","","world"},abuf[3];
	s.ChangePath("str1.dat");
	for (int i=0;i<3;++i)
	{
		s.Write(astr[i]);
		s.Close();
		s.Read(abuf[i]);
		cout<<astr[i]<<' '<<abuf[i]<<endl;
		assert(abuf[i]==astr[i]);
	}

	char cstr[1100],cbuf[1100];
	for (int i=0;i<1000;++i)
	{
		cstr[i]=(char)(rand()%60+65);
	}
	cstr[1000]='\0';
	cout<<cstr<<endl<<endl;
	BinaryFile cw("str.dat");
	cw.Write(cstr);
	BinaryFile cr("str.dat");
	cr.Read(cbuf);
	cout<<cbuf<<endl<<strlen(cstr)<<'/'<<strlen(cbuf)<<endl;
	assert(!strcmp(cstr,cbuf));
	for (int i=0;i<1001;++i)
	{
		if (cstr[i]!=cbuf[i]) cout<<'!'<<i<<' '<<cstr[i]<<' '<<cbuf[i]<<endl;
	}
	return 0;
}
