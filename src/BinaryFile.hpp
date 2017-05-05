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
			std::fstream file;
			std::string path;
			void ROpen()
			{
				if (file.is_open()) return;
				file.open(path, std::fstream::in|std::fstream::binary);
				if (!file.is_open()) throw error_opening_file("file not open");
			}
			void WOpen()
			{
				if (file.is_open()) return;
				file.open(path, std::fstream::out|std::fstream::binary|std::fstream::trunc);
				if (!file.is_open()) throw error_opening_file("file not open");
			}
			void Close()
			{
				if (file.is_open()) file.close();
			}
		public:
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
				path=pth;
				Close();
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
				file.write(reinterpret_cast<const char *>(obj),sz);
				if (file.rdstate())	Close(),throw error_writing_file("flag set: "+(char)(48+file.rdstate()));
				Close();
			}
			void Read(std::string &buf)
			{
				ROpen();
				while (!file.eof())
				{
				    char c;
					file.read(reinterpret_cast<char *>(&c),sizeof(char));
					if (file.eof()) break;
					if (file.rdstate()) Close(),throw error_reading_file("flag set: "+(char)(48+file.rdstate()));
					buf.push_back(c);
				}
				Close();
			}
			void Read(char *buf)
			{
				ROpen();
				int i=0;
				while (!file.eof())
				{
					char c;
					file.read(reinterpret_cast<char *>(&c),sizeof(char));
					if (file.eof()) break;
					if (file.rdstate()) Close(),throw error_reading_file("flag set: "+(char)(48+file.rdstate()));
					*(buf++)=c;
				}
				*buf='\0';
				Close();
			}
			void Read(void *buf,size_t sz)
			{
				ROpen();
				file.read(reinterpret_cast<char *>(buf),sz);
//				if (file.rdstate()) throw error_reading_file("flag set");
				Close();
			}
	};
}
#endif
