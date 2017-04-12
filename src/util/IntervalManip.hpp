#ifndef TRAINBOOM_UTIL_INTERVALMANIP_HPP
#define TRAINBOOM_UTIL_INTERVALMANIP_HPP

#include <iostream>
#include <cassert>
#include "exception.hpp"
#include "stupid_ptr.hpp"

namespace util {

class index_out_of_range : public exception {
public:
    index_out_of_range() : exception(
        "index_out_of_range",
        "Your index is out of range!!!") {}
};

template<
    class T,
    class M,
    class Modifier,
    class MergerT,
    class MergerM
> class IntervalManip {
private:
    size_t n;
    struct Node {
        T ans;
        size_t l, r;
        M tag;
        bool tagged;
        stupid_ptr<Node> ch[2];

        Node(size_t l, size_t r): ans(T()), l(l), r(r),
            tag(M()), tagged(false), ch({nullptr, nullptr}) {}

        void modify(const M& m) {
            if (!tagged) {
                tagged = true;
            }
            tag = MergerM()(tag, m);
            ans = Modifier()(ans, m, l, r);
        }

        void push() {
            if (tagged) {
                if (ch[0]) ch[0]->modify(tag);
                if (ch[1]) ch[1]->modify(tag);
                tag = M();
                tagged = false;
            }
        }

        void update() {
            if (ch[0] && ch[1]) {
                ans = MergerT()(ch[0]->ans, ch[1]->ans);
            }
        }
    };

    stupid_ptr<Node> root;

    stupid_ptr<Node> build(size_t l, size_t r, const stupid_ptr<T, array_deleter<T>>& data) {
        stupid_ptr<Node> u(new Node(l, r));
        if (l != r) {
            size_t mid((l + r) / 2);
            u->ch[0] = build(l, mid, data);
            u->ch[1] = build(mid + 1, r, data);
            u->update();
        }
        else {
            u->ans = data[l];
        }
        return u;
    }

    T query(const stupid_ptr<Node>& u, size_t l, size_t r) {
        if (l <= u->l && r >= u->r) {
            return u->ans;
        }
        else {
            u->push();
            size_t mid((u->l + u->r) / 2);
            if (l <= mid) {
                if (r > mid) {
                    return MergerM()(query(u->ch[0], l, r), query(u->ch[1], l, r));
                }
                else {
                    return query(u->ch[0], l, r);
                }
            }
            else {
                assert(r > mid);
                return query(u->ch[1], l, r);
            }
        }
    }

    void modify(const stupid_ptr<Node>& u, size_t l, size_t r, const M& m) {
        if (l <= u->l && r >= u->r) {
            u->modify(m);
        }
        else {
            u->push();
            size_t mid((u->l + u->r) / 2);
            if (l <= mid) {
                modify(u->ch[0], l, r, m);
            }
            if (r > mid) {
                modify(u->ch[1], l, r, m);
            }
            u->update();
        }
    }

public:
    IntervalManip(const stupid_ptr<T, array_deleter<T>>& data, size_t n): n(n), root(build(0, n - 1, data)) {}

    void rebuild(const stupid_ptr<T, array_deleter<T>>& data, size_t _n) {
        n = _n;
        root = build(0, n - 1, data);
    }

    T query(size_t l, size_t r) {
        if (r >= n || l > r)
            throw index_out_of_range();
        else
            return query(root, l, r);
    }

    void modify(size_t l, size_t r, const M& m) {
        if (r >= n || l > r)
            throw index_out_of_range();
        else
            modify(root, l, r, m);
    }
};

}

#endif
