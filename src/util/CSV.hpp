/*By ZidaneAndMessi*/
#ifndef TRAINBOOM_CSV_HPP
#define TRAINBOOM_CSV_HPP

#include <fstream>
#include <string>
#include "exception.hpp"

namespace TrainBoom {

namespace util {

namespace CSV {

using namespace std;

class CSV
{
private:
	int n;
	class line
	{
	public:
		int n;
		std::string *s;
		line()
		{
			n=0;
			s=NULL;
		}
		~line()
		{
			delete[] s;
		}
		line& operator=(const line &that) noexcept
		{
			if(this==&that)return *this;
			delete []s;
			n=that.n;
			s=new string[n];
			for(int i=0;i<n;i++)
				s[i]=that.s[i];
			return *this;
		}
		line& operator=(const line &&that) noexcept
		{
			if(this==&that)return *this;
			delete []s;
			n=that.n;
			s=new string[n];
			for(int i=0;i<n;i++)
				s[i]=that.s[i];
			return *this;
		}
		void split(const string &t)
		{
			n=0,s=NULL;
			int len=t.size();
			for(int i=0;i<len;i++)
				if(t[i]==',')
					n++;
			n++;
			s=new string[n];
			int j=0,tot=0;
			for(int i=0;i<len;i++)
				if(t[i]==',')
				{
					s[tot++]=t.substr(j,i-j);
					j=i+1;
				}
			s[tot++]=t.substr(j,len-j);
		}
	};
	line *a;
public:
	CSV()
	{
		n=0;
		a=NULL;
	}
	~CSV()
	{
		delete[] a;
	}
	CSV& operator=(const CSV &that) noexcept
	{
		if(this==&that)return *this;
		delete []a;
		n=that.n;
		a=new line[n];
		for(int i=0;i<n;i++)
			a[i]=that.a[i];
		return *this;
	}
	CSV& operator=(const CSV &&that) noexcept
	{
		if(this==&that)return *this;
		delete []a;
		n=that.n;
		a=new line[n];
		for(int i=0;i<n;i++)
			a[i]=that.a[i];
		return *this;
	}
	int size()
	{
		return n;
	}
	int size(int x)
	{
		if(x<=0||x>n)throw index_out_of_bound();
		return a[x-1].n;
	}
	string data(int x,int y)
	{
		if(x<=0||x>n)throw index_out_of_bound();
		if(y<=0||y>a[x-1].n)throw index_out_of_bound();
		return a[x-1].s[y-1];
	}
	void load(const string &filename)
	{
		n=0;
		a=NULL;
		ifstream fin(filename);
		if(!fin) {throw open_file_error();return;}
		string s;
		while(getline(fin,s))n++;
		a=new line[n];
		fin.clear();
		fin.seekg(0);
		for(int i=0;i<n;i++)
		{
			getline(fin,s);
			a[i].split(s);
		}
		fin.close();
	}
};

}	// CSV

}	// util

}	// TrainBoom


#endif
