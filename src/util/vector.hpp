#ifndef TRAINBOOM_UTIL_VECTOR_HPP
#define TRAINBOOM_UTIL_VECTOR_HPP

#include "exception.hpp"

#include <climits>
#include <cstddef>
#include <vector>

namespace TrainBoom {

namespace util {
/*
	template<typename T>
	class vector {
		private:
			int curLength;
			int maxSize;
			T * data;
		public:

			class const_iterator;
			class iterator {
				private:

					vector * v;
					int curPos;
				public:

					iterator(vector * q, int p = 0):v(q), curPos(p) {
					}

					iterator operator+(const int &n) const {

						iterator tmp = *this;
						tmp.curPos += n;

						return tmp;
					}
					iterator operator-(const int &n) const {

						iterator tmp = *this;
						tmp.curPos -= n;

						return tmp;
					}
					// return the distance between two iterator,
					// if these two iterators points to different vectors, throw invalid_iterator.
					int operator-(const iterator &rhs) const {

						if(v != rhs.v) throw invalid_iterator();
						else if(rhs.curPos > curPos) return rhs.curPos - curPos;
						else return curPos - rhs.curPos;
					}
					iterator operator+=(const int &n) {

						iterator tmp = *this;
						tmp.curPos += n;

						return tmp;
					}
					iterator operator-=(const int &n) {
						iterator tmp = *this;
						tmp.curPos -= n;

						return tmp;
					}

					iterator operator++(int) {
						iterator tmp = * this;
						++curPos;
						return tmp;
					}

					iterator& operator++() {
						++curPos;
						return * this;
					}

					iterator operator--(int) {
						iterator tmp = * this;
						--curPos;
						return tmp;
					}

					iterator& operator--() {
						--curPos;
						return * this;
					}

					T& operator*() const {
						return v->data[curPos];
					}

					bool operator==(const iterator &rhs) const {
						return(v == rhs.v && curPos == rhs.curPos);
					}
					bool operator==(const const_iterator &rhs) const {
						return(v == rhs.v && curPos == rhs.curPos);
					}

					bool operator!=(const iterator &rhs) const {
						return(v != rhs.v || curPos != rhs.curPos);
					}
					bool operator!=(const const_iterator &rhs) const {
						return(v != rhs.v || curPos != rhs.curPos);
					}
			};

			class const_iterator {
				private:

					const vector * v;
					int curPos;
				public:

					const_iterator(const vector * q, int p = 0):v(q), curPos(p) {
					}
					const_iterator operator+(const int &n) const {
						const_iterator tmp = *this;
						tmp.curPos += n;

						return tmp;
					}
					const_iterator operator-(const int &n) const {

						const_iterator tmp = *this;
						tmp.curPos -= n;

						return tmp;
					}

					int operator-(const const_iterator &rhs) const {

						if(v != rhs.v) throw invalid_iterator();
						else if(rhs.curPos > curPos) return rhs.curPos - curPos;
						else return curPos - rhs.curPos;
					}
					const_iterator operator+=(const int &n) {

						const_iterator tmp = *this;
						tmp.curPos += n;

						return tmp;
					}
					const_iterator operator-=(const int &n) {

						const_iterator tmp = *this;
						tmp.curPos -= n;

						return tmp;
					}

					const_iterator operator++(int) {
						const_iterator tmp = * this;
						++curPos;
						return tmp;
					}

					const_iterator& operator++() {
						++curPos;
						return * this;
					}

					const_iterator operator--(int) {
						const_iterator tmp = * this;
						--curPos;
						return tmp;
					}

					const_iterator& operator--() {
						--curPos;
						return * this;
					}

					const T& operator*() const {
						return v->data[curPos];
					}

					bool operator==(const iterator &rhs) const {
						return(v == rhs.v && curPos == rhs.curPos);
					}
					bool operator==(const const_iterator &rhs) const {
						return(v == rhs.v && curPos == rhs.curPos);
					}

					bool operator!=(const iterator &rhs) const {

						return(v != rhs.v || curPos != rhs.curPos);
					}
					bool operator!=(const const_iterator &rhs) const {

						return(v != rhs.v || curPos != rhs.curPos);
					}
			};

			void doubleSpace() {
				T * tmp = data;
				data = (T*)::operator new(2 * maxSize * sizeof(T));
				for(int i = 0; i < curLength; ++i)
					data[i] = tmp[i];
				maxSize *= 2;
				delete tmp;
			}
			vector(int iniSize = 10): curLength(0), maxSize(iniSize) {
				data = (T*):: operator new(maxSize * sizeof(T));
			}
			vector(const vector &other) {
				maxSize = other.maxSize;
				curLength = other.curLength;
				data = (T*):: operator new(maxSize * sizeof(T));
				for(int i = 0; i < curLength; ++i) {
					data[i] = other.data[i];
				}
			}

			~vector() {
                // std::cout << "1" << std::endl;
				for(int i = maxSize - 1; i>= 0; --i){
					T * p = data + i;
					p -> ~T();
				}
                //
				::operator delete(data);
                // std::cout << "2" << std::endl;
			}

			vector &operator=(const vector &other) {
				if(&other == this) return * this;
				for(int i = maxSize - 1; i >= 0; --i){
					T * p = data + i;
					p -> ~T();
				}
				::operator delete(data);
				maxSize = other.maxSize;
				curLength = other.curLength;
				data = (T*):: operator new(maxSize * sizeof(T));
				for(int i = 0; i < curLength; ++i)
				 	data[i] = other.data[i];
				return * this;
			}

			T & at(const size_t &pos) {
				if(pos >= curLength || pos < 0) throw index_out_of_bound();
				return data[pos];

			}
			const T & at(const size_t &pos) const {
				if(pos >= curLength || pos < 0) throw index_out_of_bound();
				return data[pos];
			}

			T & operator[](const size_t &pos) {
				if(pos >= curLength || pos < 0) throw index_out_of_bound();
				return data[pos];
			}
			const T & operator[](const size_t &pos) const {
				if(pos >= size_t(curLength) || pos < 0) throw index_out_of_bound();
				return data[pos];
			}

			const T & front() const {
				if(curLength == 0) throw container_is_empty();
				return data[0];
			}

			const T & back() const {
				if(curLength == 0) throw container_is_empty();
				return data[curLength - 1];
			}

			iterator begin() {
				iterator tmp(this, 0);
				return tmp;
			}
			const_iterator cbegin() const {
				const_iterator tmp(this, 0);
				return tmp;
			}

			iterator end() {
				iterator tmp(this, curLength);
				return tmp;
			}
			const_iterator cend() const {
				const_iterator tmp(this, curLength);
				return tmp;
			}

			bool empty() const {
				return curLength == 0;
			}

			size_t size() const {
				return curLength;
			}

			size_t capacity() const {
				return maxSize;
			}

			void clear() {
				curLength = 0;
			}

			iterator insert(iterator pos, const T &value) {
				if(curLength == maxSize) doubleSpace();
				iterator tp = end();
				for(; tp != pos; --tp)
					* tp = *(tp - 1);

				* tp = value;
				++curLength;
				return tp;
			}

			iterator insert(const size_t &ind, const T &value) {

				if(ind > curLength) throw index_out_of_bound();
				if(curLength == maxSize) doubleSpace();
				iterator tp(this, ind);
				for(int i = curLength; i > ind; --i)
					data[i] = data[i-1];
				data[ind] = value;
				++curLength;
				return tp;
			}

			iterator erase(iterator pos) {
				iterator tp = pos;
				while(pos != end()) {
					* pos = * (pos + 1);
					++pos;
				}
				--curLength;
				return tp;
			}

			iterator erase(const size_t &ind) {
				int tmp = ind;
				while(tmp != curLength) {
					data[tmp] = data[tmp + 1];
					++tmp;
				}
				iterator tp(this, ind);
				--curLength;
				return tp;
			}

			void push_back(const T &value) {
				if(curLength == maxSize) doubleSpace();
				void* ptr = data + (curLength++);
				new (ptr) T(value);
			}

			void pop_back() {
				if(curLength == 0) throw container_is_empty();
				--curLength;
			}

	};*/

using std::vector;

} // util

} // TrainBoom

#endif
