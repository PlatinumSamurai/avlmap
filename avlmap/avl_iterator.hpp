// Copyright (c) 2024 PlatinumSamurai. All rights reserved.

#ifndef AVLMAP_AVLMAP_AVL_ITERATOR_HPP_
#define AVLMAP_AVLMAP_AVL_ITERATOR_HPP_

#include <memory>
#include <functional>
#include <utility>


template <typename Key,
          typename T,
          typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>
          >
class Avl;


template <typename Key, typename T, typename Compare = std::less<Key>>
class AvlIterator : public std::iterator<std::bidirectional_iterator_tag,
                                                            Node<Key, T>> {
 private:
     friend class Avl<Key, T, Compare>;
     Node<Key, T> *p;
     Compare cmp;
     bool start;
     bool end;
     int dist = 0;

     explicit AvlIterator(Node<Key, T> *, bool = false, bool = false);

     Node<Key, T>* NextElem(Node<Key, T> *);
     Node<Key, T>* PrevElem(Node<Key, T> *);

 public:
     AvlIterator(const AvlIterator<Key, T, Compare> &);

     AvlIterator& operator++();
     AvlIterator& operator--();
     AvlIterator operator++(int);
     AvlIterator operator--(int);
     AvlIterator<Key, T, Compare>& operator+=(unsigned);
     std::pair<const Key, T>& operator*() const;
     bool operator==(const AvlIterator &other) const;
     bool operator!=(const AvlIterator &other) const;
};


template <typename Key, typename T, typename Compare>
AvlIterator<Key, T, Compare>::AvlIterator(Node<Key, T> *node,
                                bool s, bool e) : p(node), start(s), end(e) {}


template <typename Key, typename T, typename Compare>
AvlIterator<Key, T, Compare>::AvlIterator(const AvlIterator<Key, T,
                    Compare> &it) : p(it.p), start(it.start), end(it.end) {}


template <typename Key, typename T, typename Compare>
AvlIterator<Key, T, Compare>& AvlIterator<Key, T, Compare>::operator++() {
    p = NextElem(p);

    return *this;
}


template <typename Key, typename T, typename Compare>
AvlIterator<Key, T, Compare>& AvlIterator<Key, T, Compare>::operator--() {
    p = PrevElem(p);

    return *this;
}


template <typename Key, typename T, typename Compare>
AvlIterator<Key, T, Compare> AvlIterator<Key, T, Compare>::operator++(int) {
    AvlIterator<Key, T, Compare> temp = *this;
    p = NextElem(p);

    return temp;
}


template <typename Key, typename T, typename Compare>
AvlIterator<Key, T, Compare> AvlIterator<Key, T, Compare>::operator--(int) {
    AvlIterator<Key, T, Compare> temp = *this;
    p = PrevElem(p);

    return temp;
}


template <typename Key, typename T, typename Compare>
AvlIterator<Key, T, Compare> operator+(const AvlIterator<Key, T,
                                                    Compare> &it, unsigned n) {
    AvlIterator<Key, T, Compare> temp = it;

    for (unsigned i = 0; i < n; ++i) {
        temp++;
    }

    return temp;
}


template <typename Key, typename T, typename Compare>
AvlIterator<Key, T, Compare>& AvlIterator<Key, T, Compare>::operator+=(
                                                                unsigned n) {
    for (unsigned i = 0; i < n; ++i) {
        (*this)++;
    }

    return *this;
}


template <typename Key, typename T, typename Compare>
std::pair<const Key, T>& AvlIterator<Key, T, Compare>::operator*() const {
    return *(p->pair);
}


template <typename Key, typename T, typename Compare>
bool AvlIterator<Key, T, Compare>::operator!=(const AvlIterator<Key, T,
                                                    Compare> &other) const {
    return std::tie(p, end) != std::tie(other.p, other.end);
}


template <typename Key, typename T, typename Compare>
bool AvlIterator<Key, T, Compare>::operator==(const AvlIterator &other) const {
    return std::tie(p, end) == std::tie(other.p, other.end);
}


template <typename Key, typename T, typename Compare>
Node<Key, T>* AvlIterator<Key, T, Compare>::NextElem(Node<Key, T> *node) {
    if (!node) {
        return nullptr;
    }
    if (start) {
        start = false;
    }
    if (p->pair->first == node->pair->first) {
       if (node->right) {
           return NextElem(node->right);
       } else if (node->prev) {
           return NextElem(node->prev);
       }
    } else if (cmp(p->pair->first, node->pair->first)) {
        if (node->left && cmp(p->pair->first, node->left->pair->first)) {
            return NextElem(node->left);
        } else {
            return node;
        }
    } else {
        if (node->prev) {
            return NextElem(node->prev);
        } else {
            end = true;
            return p;
        }
    }

    return nullptr;
}


template <typename Key, typename T, typename Compare>
Node<Key, T>* AvlIterator<Key, T, Compare>::PrevElem(Node<Key, T> *node) {
    if (start) {
        return p;
    }
    if (end) {
        end = false;
        return p;
    }
    if (p->pair->first == node->pair->first) {
        if (node->left) {
            return PrevElem(node->left);
        } else if (node->prev) {
            return PrevElem(node->prev);
        }
    } else if (cmp(node->pair->first, p->pair->first)) {
        if (node->right && cmp(node->right->pair->first, p->pair->first)) {
            return PrevElem(node->right);
        } else {
            return node;
        }
    } else {
        if (node->prev) {
            return PrevElem(node->prev);
        } else {
            start = true;
            return p;
        }
    }

    return nullptr;
}

#endif  // AVLMAP_AVLMAP_AVL_ITERATOR_HPP_

