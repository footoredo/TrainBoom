#ifndef TRAINBOOM_UTIL_MAP_HPP
#define TRAINBOOM_UTIL_MAP_HPP

#include "stupid_ptr.hpp"
#include "pair.hpp"
#include "exception.hpp"

namespace TrainBoom {

namespace util {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
private:
	struct Node;
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */

	class const_iterator;
	class iterator {
		friend class map;
	private:
		stupid_ptr<Node> p;
		const map *who;

	public:
		constexpr iterator(): p(nullptr), who(nullptr) {
		}
		iterator(const stupid_ptr<Node>& p, const map *who): p(p), who(who) {
		}
		iterator(const iterator &other): p(other.p), who(other.who) {
		}

		iterator& operator=(const iterator &other) {
			if (this != &other) {
				p = other.p;
				who = other.who;
			}
			return *this;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			if (!p) throw invalid_iterator();
			iterator tmp(*this);
			p = p->next_p;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
			if (!p) throw invalid_iterator();
			p = p->next_p;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			if (!who || p == who->min_p) throw invalid_iterator();
			iterator tmp(*this);
			if (!p) p = who->max_p;
			else p = p->prev_p;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
			if (!who || p == who->min_p) throw invalid_iterator();
			else if (!p) p = who->max_p;
			else p = p->prev_p;
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
			return *(p->value);
		}
		bool operator==(const iterator &rhs) const {
			return p == rhs.p && who == rhs.who;
		}
		bool operator==(const const_iterator &rhs) const {
			return p == rhs.p && who == rhs.who;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return p != rhs.p || who != rhs.who;
		}
		bool operator!=(const const_iterator &rhs) const {
			return p != rhs.p || who != rhs.who;
		}

		/**
		 * for the support of it->first.
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		stupid_ptr<value_type> operator->() const noexcept {
			return p->value;
		}
	};
	class const_iterator {
		friend class map;
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			stupid_ptr<Node> p;
			const map *who;

		public:
			constexpr const_iterator(): p(nullptr), who(nullptr) {
			}
			const_iterator(const stupid_ptr<Node>& p, const map *who): p(p), who(who) {}
			// const_iterator(const const_iterator &other): p(other.p), who(who) {
			// }
			const_iterator(const iterator &other): p(other.p), who(who) {
			}

			const_iterator& operator=(const const_iterator &other) {
				if (this != &other) {
					p = other.p;
					who = other.who;
				}
				return *this;
			}

			const_iterator operator++(int) {
				if (!p) throw invalid_iterator();
				const_iterator tmp(*this);
				p = p->next_p;
				return tmp;
			}
			/**
			 * TODO ++iter
			 */
			const_iterator & operator++() {
				if (!p) throw invalid_iterator();
				p = p->next_p;
				return *this;
			}
			/**
			 * TODO iter--
			 */
			const_iterator operator--(int) {
				if (!who || p == who->min_p) throw invalid_iterator();
				const_iterator tmp(*this);
				if (!p) p = who->max_p;
				else p = p->prev_p;
				return tmp;
			}
			/**
			 * TODO --iter
			 */
			const_iterator & operator--() {
				if (!who || p == who->min_p) throw invalid_iterator();
				else if (!p) p = who->max_p;
				else p = p->prev_p;
				return *this;
			}

			const value_type & operator*() const {
				return *(p->value);
			}
			bool operator==(const iterator &rhs) const {
				return p == rhs.p && who == rhs.who;
			}
			bool operator==(const const_iterator &rhs) const {
				return p == rhs.p && who == rhs.who;
			}
			/**
			 * some other operator for iterator.
			 */
			bool operator!=(const iterator &rhs) const {
				return p != rhs.p || who != rhs.who;
			}
			bool operator!=(const const_iterator &rhs) const {
				return p != rhs.p || who != rhs.who;
			}

			/**
			 * for the support of it->first.
			 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
			 */
			stupid_ptr<value_type> operator->() const noexcept {
				return p->value;
			}
	};
	/**
	 * TODO two constructors
	 */
	constexpr map(): root(nullptr), min_p(nullptr), max_p(nullptr), _size(0) {
	}
	map(const map &other): _size(other._size) {
		root = copy_tree(other.root);
		// std::cout << "incopy " << (bool)(root->child[0]) << " " << (bool)(root->child[1]) << " " << (bool)(other.root->child[0]) << " " << (bool)(other.root->child[1]) << std::endl;
		min_p = find_min(root); max_p = find_max(root);
		list_link_all();
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
		if (this != &other) {
		// std::cout << "in = " << (bool)(other.root->child[0]) << " " << (bool)(other.root->child[1]) << std::endl;
			delete_tree(root);
			root = copy_tree(other.root);
			// std::cout << "in = " << (bool)(root->child[0]) << " " << (bool)(root->child[1]) << " " << (bool)(other.root->child[0]) << " " << (bool)(other.root->child[1]) << std::endl;
			min_p = find_min(root); max_p = find_max(root);
			list_link_all();
			_size = other._size;
		}
		return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map() {
		clear();
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */


	T & at(const Key &key) {
		stupid_ptr<Node> p = _find(key);
		if (!p) throw index_out_of_bound();
		else {
			return p->value->second;
		}
	}
	const T & at(const Key &key) const {
		stupid_ptr<Node> p = _find(key);
		if (!p) throw index_out_of_bound();
		else {
			return p->value->second;
		}
	}
	/**
	 * TODO
	 * access specified element
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
		stupid_ptr<Node> p = _find(key);
		if (!p) {
			p = insert(value_type(key, T())).first.p;
		}
		return p->value->second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
		stupid_ptr<Node> p = _find(key);
		if (!p) throw index_out_of_bound();
		return p->value->second;
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() const {
		return iterator(min_p, this);
	}
	const_iterator cbegin() const {
		return const_iterator(min_p, this);
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() const {
		return iterator(nullptr, this);
	}
	const_iterator cend() const {
		return const_iterator(nullptr, this);
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
		return _size == 0;
	}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {
		return _size;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		delete_tree(root);
		root = stupid_ptr<Node>(nullptr);
		min_p = stupid_ptr<Node>(nullptr);
		max_p = stupid_ptr<Node>(nullptr);
		_size = 0;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion),
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
		auto tmp = _insert(value);
		if (tmp.second) {
			++ _size;
			if (!min_p || Compare()(value.first, min_p->value->first))
				min_p = tmp.first;
			if (!max_p || Compare()(max_p->value->first, value.first))
				max_p = tmp.first;
			list_link(find_prev(tmp.first), tmp.first);
			list_link(tmp.first, find_next(tmp.first));
			// std::cout << "insert " << value.first << std::endl;
			// std::cout << "max_p: " << max_p->value->first << std::endl;
			// list_link_all();
			// std::cout << "inside size: " << count_size(root) << std::endl;
		}
		return pair<iterator, bool>(iterator(tmp.first, this), tmp.second);
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
		if (pos == this->end() || pos.who != this) {
			throw invalid_iterator();
		}
		else {
			-- _size;
			// std::cout << "deleting " << pos->first << std::endl;
			// std::cout << "before: " << min_p->value->first << std::endl;
			// std::cout << pos.p << " " << max_p << std::endl;
			// std::cout << (pos.p == max_p) << std::endl;
			// std::cout << "counter: " << pos.p.get_counter() << std::endl;
			if (pos.p == min_p)
				min_p = pos.p->next_p;
			if (pos.p == max_p)
				max_p = pos.p->prev_p;
			// std::cout << (bool)(min_p) << std::endl;
			// std::cout << "after: " << min_p->value->first << std::endl;

			// std::cout << "counter: " << pos.p.get_counter() << std::endl;
			list_break(pos.p);
			// std::cout << "counter: " << pos.p.get_counter() << std::endl;
			// std::cout << (bool)(pos.p->child[0]) << " " << (bool)(pos.p->child[1]) << std::endl;
			_delete(pos.p);
			// std::cout << "counter: " << pos.p.get_counter() << std::endl;
		}
	}
	/**
	 * Returns the number of elements with key
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	iterator find(const Key &key) {
		return iterator(_find(key), this);
	}
	size_t count(const Key &key) const {
		return (bool)_find(key);
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	const_iterator find(const Key &key) const {
		return const_iterator(_find(key), this);
	}

	size_t countsize() const {
		return _countsize(root);
	}

private:
	static const bool BLACK = 0, RED = 1;
	struct Node {
		stupid_ptr<value_type> value;
		stupid_ptr<Node> child[2];
		stupid_ptr<Node> parent;
		stupid_ptr<Node> prev_p, next_p;
		// map *who;
		bool color;
		Node (const value_type& value)
			: value(new value_type(value)),
				child({nullptr, nullptr}),
				parent(nullptr),
				prev_p(nullptr), next_p(nullptr),
				// who(who),
				color(RED) {}
		Node (const stupid_ptr<Node>& other)
			: value(new value_type(*other->value)),
				child({nullptr, nullptr}),
				parent(nullptr),
				prev_p(nullptr), next_p(nullptr),
				// who(who),
				color(other->color) {}
		friend stupid_ptr<Node> grandparent(const stupid_ptr<Node>& u) {
			if (u->parent) return u->parent->parent;
			else return nullptr;
		}
		friend stupid_ptr<Node> uncle(const stupid_ptr<Node>& u) {
			if (u->parent) {
				return u->parent->child[u->parent->child[0] == u];
			}
			else return nullptr;
		}
		friend void list_link(const stupid_ptr<Node>& x, const stupid_ptr<Node>& y) {
			if (x) x->next_p = y;
			if (y) y->prev_p = x;
		}
		friend void list_break(const stupid_ptr<Node>& x) {
			if (x) {
				list_link(x->prev_p, x->next_p);
				x->prev_p = nullptr;
				x->next_p = nullptr;
			}
		}
	};

	class no_such_child : public exception {
	};

	size_t _countsize(const stupid_ptr<Node>& u) const {
		if (!u) return 0;
		else return 1 + _countsize(u->child[0]) + _countsize(u->child[1]);
	}

	void link(const stupid_ptr<Node>& x, const stupid_ptr<Node>& y, bool direction) {
		// std::cout << "link " << (x ? x->value->first : -1) << " " << (y ? y->value->first : -1) << std::endl;
		if (x) x->child[direction] = y;
		else root = y;
		if (y) y->parent = x;
	}

	void rotate(const stupid_ptr<Node>& x, bool direction) {  // child[direction] replace it.
		stupid_ptr<Node> y(x->child[direction]);
		if (!y) throw no_such_child();
		stupid_ptr<Node> B(y->child[!direction]);
		stupid_ptr<Node> p(x->parent);
		bool pdirection = p ? (x == p->child[1]) : false;

		link(x, B, direction);
		link(y, x, !direction);
		link(p, y, pdirection);
	}

	stupid_ptr<Node> root, min_p, max_p;
	size_t _size;

	void delete_tree(const stupid_ptr<Node>& u) {
		if (!u) return;
		u->prev_p = u->next_p = nullptr;
		u->parent = nullptr;
		delete_tree(u->child[0]);
		delete_tree(u->child[1]);
		u->child[0] = u->child[1] = nullptr;
	}

	size_t count_size(stupid_ptr<Node> u) {
		if (!u) return 0;
		else return 1 + count_size(u->child[0]) + count_size(u->child[1]);
	}

	stupid_ptr<Node> list_DFS(stupid_ptr<Node> prev, const stupid_ptr<Node>& cur) {
		if (!cur) return prev;
		prev = list_DFS(prev, cur->child[0]);
		list_link(prev, cur);
		return list_DFS(cur, cur->child[1]);
	}

	void list_link_all() {
		list_link(list_DFS(stupid_ptr<Node>(nullptr), root), stupid_ptr<Node>(nullptr));
	}

	stupid_ptr<Node> copy_tree(const stupid_ptr<Node>& v) {
		if (!v) return stupid_ptr<Node>(nullptr);
		else {
			stupid_ptr<Node> u(new Node(v));
			link(u, copy_tree(v->child[0]), 0);
			link(u, copy_tree(v->child[1]), 1);
			return u;
		}
	}

	stupid_ptr<Node> find_min(const stupid_ptr<Node>& start) {
		if (!start) return stupid_ptr<Node>(nullptr);
		else {
			stupid_ptr<Node> u(start);
			while (u->child[0]) u = u->child[0];
			return u;
		}
	}

	stupid_ptr<Node> find_max(const stupid_ptr<Node>& start) {
		if (!start) return stupid_ptr<Node>(nullptr);
		else {
			stupid_ptr<Node> u(start);
			while (u->child[1]) u = u->child[1];
			return u;
		}
	}

	stupid_ptr<Node> find_prev(const stupid_ptr<Node>& u) {
		const Key& key = u->value->first;
		stupid_ptr<Node> cur(root), ans(nullptr);
		while (cur) {
			if (Compare()(cur->value->first, key)) {
				if (!ans || Compare()(ans->value->first, cur->value->first)) {
					ans = cur;
				}
				cur = cur->child[1];
			}
			else {
				cur = cur->child[0];
			}
		}
		return ans;
	}

	stupid_ptr<Node> find_next(const stupid_ptr<Node>& u) const {
		const Key& key = u->value->first;
		stupid_ptr<Node> cur(root), ans(nullptr);
		while (cur) {
			if (Compare()(key, cur->value->first)) {
				if (!ans || Compare()(cur->value->first, ans->value->first)) {
					ans = cur;
				}
				cur = cur->child[0];
			}
			else {
				cur = cur->child[1];
			}
		}
		return ans;
	}

	stupid_ptr<Node> _find(const Key& key) const {
		stupid_ptr<Node> u(root);
		while (u) {
			if (Compare()(key, u->value->first)) {
				u = u->child[0];
			}
			else if (Compare()(u->value->first, key)) {
				u = u->child[1];
			}
			else {
				return u;
			}
		}
		return stupid_ptr<Node>(nullptr);
	}

	pair<stupid_ptr<Node>, bool> _insert(const value_type& value) {
		stupid_ptr<Node> n(new Node(value));
		if (!root) {
			root = n;
			maintain_insert(n);
			return make_pair(n, true);
		}
		else {
			stupid_ptr<Node> u(root);
			while (true) {
				if (Compare()(value.first, u->value->first)) {
					if (!u->child[0]) {
						link(u, n, 0);
						maintain_insert(n);
						return make_pair(n, true);
					}
					else {
						u = u->child[0];
					}
				}
				else if (Compare()(u->value->first, value.first)) {
					if (!u->child[1]) {
						link(u, n, 1);
						maintain_insert(n);
						// std::cerr << "success!" << std::endl;
						return make_pair(n, true);
					}
					else {
						u = u->child[1];
					}
				}
				else {
					return make_pair(u, false);
				}
			}
		}
	}

	void maintain_insert(stupid_ptr<Node> n) {
		// return;
		while (n) {
			if (!n->parent) {
				n->color = BLACK;
				break;
			}
			else {
				if (n->parent->color == BLACK) {
					break;
				}
				else {
					stupid_ptr<Node> p(n->parent), gp(p->parent);
					bool gpdirection = p == gp->child[1];
					stupid_ptr<Node> uncle(gp->child[!gpdirection]);
					if (uncle && uncle->color == RED) {
						p->color = BLACK;
						uncle->color = BLACK;
						gp->color = RED;
						n = gp;  // continue
					}
					else {
						bool pdirection = n == p->child[1];
						if (pdirection != gpdirection) {
							rotate(p, pdirection);
							swap(n, p);
							pdirection ^= 1;
						}
						p->color = BLACK;
						gp->color = RED;
						rotate(gp, gpdirection);
						break;
					}
				}
			}
		}
	}

	void _delete(stupid_ptr<Node> u) {
		if (u->child[0] && u->child[1]) {
			stupid_ptr<Node> v = find_max(u->child[0]);
			// if ((int)u->value->first == 2017) {
			// 	std::cout << v->value->first << std::endl;
			// 	swap(u->value, v->value);
			// 	std::cout << v->value->first << std::endl;
			// 	throw;
			// }
			stupid_ptr<Node> pu(u->parent), pv(v->parent);
			bool du = pu ? u == pu->child[1] : false;
			bool dv = v == pv->child[1];

			stupid_ptr<Node> lu(u->child[0]), ru(u->child[1]),
				lv(v->child[0]), rv(v->child[1]);

			if (v == u->child[0]) {
				link(u, lv, 0); link(u, rv, 1);
				link(v, u, 0); link(v, ru, 1);
				link(pu, v, du);
			}
			else {
				link(u, lv, 0); link(u, rv, 1);
				link(pv, u, dv);
				link(v, lu, 0); link(v, ru, 1);
				link(pu, v, du);
			}

			std::swap(u->color, v->color);
		}

		stupid_ptr<Node> child(u->child[!!u->child[1]]),
			p(u->parent);
		// std::cout << "father: " << p->value->first << std::endl;
		// std::cout << "is empty ? " << (bool)child << std::endl;
		link(p, child, p ? u == p->child[1] : false);
		if (u->color == BLACK) {
			if (child && child->color == RED) {
				child->color = BLACK;
			}
			else {
				u = child;
				while (true) {
					// std::cout << "deleting " << u->value->first << std::endl;
					if (!p)
						break;
					else {
						bool pdirection = u == p->child[1];
						stupid_ptr<Node> s(p->child[!pdirection]);
						if (s->color == RED) {
							p->color = RED;
							s->color = BLACK;
							rotate(p, !pdirection);
							s = p->child[!pdirection];
						}
						if (p->color == BLACK &&
							s->color == BLACK &&
							(!s->child[0] || s->child[0]->color == BLACK) &&
							(!s->child[1] || s->child[1]->color == BLACK)) {
								s->color = RED;
								u = p;
								p = p->parent;
								continue;
						}
						else {
							if (p->color == RED &&
								s->color == BLACK &&
								(!s->child[0] || s->child[0]->color == BLACK) &&
								(!s->child[1] || s->child[1]->color == BLACK)) {
									s->color = RED;
									p->color = BLACK;
									break;
							}
							else {
								if (s->color == BLACK) {
									if ((s->child[pdirection] && s->child[pdirection]->color == RED) &&
										(!s->child[!pdirection] || s->child[!pdirection]->color == BLACK)) {
											s->color = RED;
											s->child[pdirection]->color = BLACK;
											rotate(s, pdirection);
											s = s->parent;
									}
								}
								s->color = p->color;
								p->color = BLACK;
								s->child[!pdirection]->color = BLACK;
								rotate(p, !pdirection);
								break;
							}
						}
					}
				}
			}
		}
	}

};

}	// util

}	// TrainBoom

#endif
