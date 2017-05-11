#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cassert>
#include "../BinaryFile.hpp"
using namespace std;
using namespace trainBoom;
struct foo{
	int a,b,c;
	char t[100];
	string s;
	foo():a(0),b(0),c(0){}
	foo(int _a,int _b,int _c):a(_a),b(_b),c(_c){
		int i,tp=rand()%40;
		for (i=0;i<tp;++i)
		{
			t[i]=(char)(rand()%26+65);
			s.push_back((char)(rand()%26+65));
		}
		t[tp]='\0';
	}
	void print()
	{
		cout<<a<<' '<<b<<' '<<c<<' '<<t<<' '<<s<<endl;
	}
};
int main()
{
	BinaryFile asd("t.dat");
	asd.Write(std::string("first"));
	asd.Write(std::string("second"));
	asd.Write(std::string("third"));
	std::string bbuf[3];
	for (int i=0;i<3;++i)
	{
		asd.Read(bbuf[i]);
		cout<<bbuf[i]<<endl;
	}
	asd.Close();

	BinaryFile asdas("ttt.dat");
	asdas.Write(std::string("123546611123"));
	std::string tmpstr; asdas.Read(tmpstr);
	std::cout << tmpstr << std::endl;
	assert(tmpstr == "123546611123");
	asdas.Close();

	int w=5;
	BinaryFile tt("hhg.dat");
	tt.Write(w);
	tt.Read(w);
	cout<<w<<endl;

	srand(time(0));
	foo a(3,6,4),b;
	BinaryFile tmp("hhh.dat");
	a.print();
	tmp.Write(a.a);
	tmp.Write(a.b);
	tmp.Write(a.t);
	tmp.Write(a.s);
	tmp.Write(a.c);
	tmp.Close();

	BinaryFile rd("hhh.dat");
	rd.Read(b.a);
	rd.Read(b.b);
	rd.Read(b.t);
	rd.Read(b.s);
	rd.Read(b.c);
	b.print();

//	s.Write(&str,sizeof(str));		//not ok
//	s.Read(&buf,sizeof(buf));		//not ok

	char cstr[1100],cbuf[1100];
	for (int i=0;i<1000;++i)
	{
		cstr[i]=(char)(rand()%60+65);
	}
	cstr[1000]='\0';
	cout<<cstr<<endl<<endl;
	BinaryFile cw("str.dat");
	cw.Write(cstr);
	cw.Close();
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
