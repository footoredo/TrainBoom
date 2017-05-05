#ifndef TRAINBOOM_UTIL_INTERVALMANIP_HPP
#define TRAINBOOM_UTIL_INTERVALMANIP_HPP

#include <iostream>
#include <cassert>
#include "exception.hpp"
#include "stupid_ptr.hpp"

namespace TrainBoom {

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
    stupid_array<stupid_ptr<T>> data;
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
                // std::cout <<  "!!!!" << std::endl;
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

    stupid_ptr<Node> build(size_t l, size_t r) {
        stupid_ptr<Node> u(new Node(l, r));
        if (l != r) {
            size_t mid((l + r) / 2);
            u->ch[0] = build(l, mid);
            u->ch[1] = build(mid + 1, r);
            u->update();
        }
        else {
            u->ans = *(data[l]);
            // std::cout << l << " " << data[l] << " " << u->ans << std::endl;
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
                    return MergerT()(query(u->ch[0], l, r), query(u->ch[1], l, r));
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

    void forceApply(const stupid_ptr<Node>& u) {
        if (u->l != u->r) {
            u->push();
            forceApply(u->ch[0]);
            forceApply(u->ch[1]);
        }
        else {
            // std::cout << u->l << " " << u->ans << std::endl;
            *(data[u->l]) = u->ans;
        }
    }

public:
    IntervalManip(const stupid_array<stupid_ptr<T>>& data, size_t n):
        data(data), n(n), root(build(0, n - 1)) {}

    void rebuild(const stupid_array<stupid_ptr<T>>& _data, size_t _n) {
        data = _data;
        n = _n;
        root = build(0, n - 1);
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

    void forceApply() {
        forceApply(root);
    }
};

}   // util

}   // TrainBoom

#endif
