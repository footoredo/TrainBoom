#ifndef TRAINBOOM_BINOP_HPP
#define TRAINBOOM_BINOP_HPP

#include "exception.hpp"
#include <iostream>
#include <fstream>
#include <string>

namespace TrainBoom
{
	class BinaryFile
	{
		private:
			std::ifstream in;
			std::ofstream out;
			std::string path;
			void WClose()
			{
				if (out.is_open()) out.close();
			}
			void RClose()
			{
				if (in.is_open()) in.close();
			}
			void ROpen()
			{
				if (in.is_open()) return;
				in.open(path, std::fstream::in|std::fstream::binary);
				if (!in.is_open()) throw error_opening_file("file not open");
			}
			void WOpen()
			{
				if (out.is_open()) return;
				out.open(path, std::fstream::out|std::fstream::binary|std::fstream::trunc);
				if (!out.is_open()) throw error_opening_file("file not open");
			}
		public:
			void Open()
			{
				ROpen();
				WOpen();
			}
			void Close()
			{
				WClose();
				RClose();
			}
			class error_opening_file : public exception
			{
				public:
					error_opening_file(const std::string &det) : exception(
						    "error_opening_file",
						    det) {}
			};
			class error_writing_file : public exception
			{
				public:
					error_writing_file(const std::string &det) : exception(
						    "error_writing_file",
						    det) {}
			};
			class error_reading_file : public exception
			{
				public:
					error_reading_file(const std::string &det) : exception(
						    "error_reading_file",
						    det) {}
			};
			BinaryFile(const std::string &_p):path(_p)
			{
//				Open();
			}
			~BinaryFile()
			{
//				Close(); automatically closed upon destroying
			}
			void ChangePath(const std::string &pth)
			{
				Close();
				path=pth;
//				Open();
			}
			void Write(const std::string &obj)
			{
				Write(obj.c_str(),sizeof(char)*obj.size());
			}
			void Write(const char *obj)
			{
				Write(obj,strlen(obj));
			}
			void Write(const void *obj,size_t sz)
			{
				WOpen();
				out.write(reinterpret_cast<const char *>(obj),sz);
				if (out.rdstate())	throw error_writing_file("flag set: "+(char)(48+out.rdstate()));
//				Close();
			}
			void Read(std::string &buf)
			{
				WClose();
				ROpen();
				while (!in.eof())
				{
				    char c;
					in.read(reinterpret_cast<char *>(&c),sizeof(char));
					if (in.eof()) break;
					if (in.rdstate()) throw error_reading_file("flag set: "+(char)(48+in.rdstate()));
					buf.push_back(c);
				}
//				Close();
			}
			void Read(char *buf)
			{
				WClose();
				ROpen();
				while (!in.eof())
				{
					char c;
					in.read(reinterpret_cast<char *>(&c),sizeof(char));
					if (in.eof()) break;
					if (in.rdstate()) Close(),throw error_reading_file("flag set: "+(char)(48+in.rdstate()));
					*(buf++)=c;
				}
				*buf='\0';
//				Close();
			}
			void Read(void *buf,size_t sz)
			{
				WClose();
				ROpen();
				in.read(reinterpret_cast<char *>(buf),sz);
//				if (file.rdstate()) throw error_reading_file("flag set");
//				Close();
			}
			template<typename T>
			void Write(const T& obj)
			{
				Write(&obj,sizeof(obj));
			}
			template<typename T>
			void Read(T& obj)
			{
				Read(&obj,sizeof(obj));
			}
	};
}
#endif
