// Copyright (c) 2024 PlatinumSamurai. All rights reserved.

#ifndef AVLMAP_AVLMAP_AVL_HPP_
#define AVLMAP_AVLMAP_AVL_HPP_

#include <memory>
#include <utility>
#include "node.hpp"
#include "avl_iterator.hpp"
#include "../format/format.hpp"


template <typename Key, typename T, typename Compare, typename Allocator>
class Avl {
 private:
    Node<Key, T> *root;
    Compare cmp;
    Allocator alloc;

    void UpdateHeight(Node<Key, T> *);
    int GetHeight(const Node<Key, T> *) const;
    int HeightDiff(const Node<Key, T> *) const;
    size_t Count(const Node<Key, T> *) const;

    Node<Key, T>* LeftRot(Node<Key, T> *);
    Node<Key, T>* RightRot(Node<Key, T> *);
    Node<Key, T>* Rebalance(Node<Key, T> *);
    Node<Key, T>* MinElem(Node<Key, T> *) const;
    Node<Key, T>* MaxElem(Node<Key, T> *) const;
    Node<Key, T>* RemoveElem(Node<Key, T> *);
    Node<Key, T>* Erase(Node<Key, T> *, const Key &);
    void Clear(Node<Key, T> *);
    bool Contains(Node<Key, T> *, const Key &) const;
    AvlIterator<Key, T, Compare> Find(Node<Key, T> *, const Key &);
    void UpdateParents(Node <Key, T> *);


 public:
    typedef AvlIterator<Key, T, Compare> iterator;
    typedef std::reverse_iterator<AvlIterator<Key, T, Compare>> r_iterator;
    typedef const AvlIterator<Key, T, Compare> c_iterator;
    typedef const std::reverse_iterator<AvlIterator<Key, T, Compare>>
                                                                cr_iterator;
    Avl();
    Avl(const Key &, T &&);
    Avl(std::initializer_list<std::pair<const Key, T>>);
    Avl(const Avl &);
    ~Avl();

    std::pair<AvlIterator<Key, T, Compare>, bool>
                                    insert(const std::pair<const Key, T> &);
    size_t erase(const Key &k);
    auto erase(auto pos) -> decltype(pos);
    bool empty() const;
    size_t size() const;
    void clear();
    bool contains(const Key &k) const;
    AvlIterator<Key, T, Compare> find(const Key &k);
    T& at(const Key &);
    T& operator[](const Key &);
    T& operator[](const Key &&);
    T& operator=(const Avl &);

    template <typename K, typename Value, typename Comp, typename Alloc>
    friend std::ostream& operator<<(std::ostream &out,
                                        const Avl<K, Value, Comp, Alloc> &avl);
    void printNode(std::ostream &out,
                                const Node<Key, T> *node, int offset) const;

    iterator begin();
    iterator end();
    r_iterator rbegin();
    r_iterator rend();
    c_iterator begin() const;
    c_iterator end() const;
    cr_iterator rbegin() const;
    cr_iterator rend() const;
};


template <typename Key, typename T, typename Compare, typename Allocator>
AvlIterator<Key, T, Compare> Avl<Key, T, Compare, Allocator>::begin() {
    return AvlIterator<Key, T, Compare>(MinElem(root), true, false);
}


template <typename Key, typename T, typename Compare, typename Allocator>
AvlIterator<Key, T, Compare> Avl<Key, T, Compare, Allocator>::end() {
    return AvlIterator<Key, T, Compare>(MaxElem(root), false, true);
}


template <typename Key, typename T, typename Compare, typename Allocator>
std::reverse_iterator<AvlIterator<Key, T,
                        Compare>> Avl<Key, T, Compare, Allocator>::rbegin() {
    return std::reverse_iterator<AvlIterator<Key, T, Compare>>
                (AvlIterator<Key, T, Compare>(MaxElem(root), false, true));
}


template <typename Key, typename T, typename Compare, typename Allocator>
std::reverse_iterator<AvlIterator<Key, T,
                            Compare>> Avl<Key, T, Compare, Allocator>::rend() {
    return std::reverse_iterator<AvlIterator<Key, T, Compare>>
                (AvlIterator<Key, T, Compare>(MinElem(root), true, false));
}


template <typename Key, typename T, typename Compare, typename Allocator>
Avl<Key, T, Compare, Allocator>::c_iterator
                            Avl<Key, T, Compare, Allocator>::begin() const {
    return c_iterator(MinElem(root), true, false);
}


template <typename Key, typename T, typename Compare, typename Allocator>
Avl<Key, T, Compare, Allocator>::c_iterator
                            Avl<Key, T, Compare, Allocator>::end() const {
    return c_iterator(MaxElem(root), false, true);
}


template <typename Key, typename T, typename Compare, typename Allocator>
Avl<Key, T, Compare, Allocator>::cr_iterator
                            Avl<Key, T, Compare, Allocator>::rbegin() const {
    return cr_iterator(AvlIterator<const Key, const T, Compare>
                                            (MaxElem(root), false, true));
}


template <typename Key, typename T, typename Compare, typename Allocator>
Avl<Key, T, Compare, Allocator>::cr_iterator
                            Avl<Key, T, Compare, Allocator>::rend() const {
    return cr_iterator(AvlIterator<const Key, const T, Compare>
                                            (MinElem(root), true, false));
}


template <typename Key, typename T, typename Compare, typename Allocator>
Node<Key, T>* Avl<Key, T, Compare, Allocator>::LeftRot(Node<Key, T> *node) {
    Node<Key, T> *temp = node->right;
    node->right = temp->left;
    if (node->right) {
        node->right->prev = node;
    }

    temp->left = node;
    temp->prev = node->prev;
    node->prev = temp;

    UpdateHeight(node);
    UpdateHeight(temp);

    return temp;
}


template <typename Key, typename T, typename Compare, typename Allocator>
Node<Key, T>* Avl<Key, T, Compare, Allocator>::RightRot(Node<Key, T> *node) {
    Node<Key, T> *temp = node->left;
    node->left = temp->right;
    if (node->left) {
        node->left->prev = node;
    }

    temp->right = node;
    temp->prev = node->prev;
    node->prev = temp;

    UpdateHeight(node);
    UpdateHeight(temp);

    return temp;
}


template <typename Key, typename T, typename Compare, typename Allocator>
Node<Key, T>* Avl<Key, T, Compare, Allocator>::Rebalance(Node<Key, T> *node) {
    bool side;

    if (!node) {
        return node;
    }

    if (node->prev) {
        if (node->prev->left &&
                    node->prev->left->pair->first == node->pair->first) {
            side = true;
        } else {
            side = false;
        }
    }

    UpdateHeight(node);
    if (HeightDiff(node) == 2) {
        if (HeightDiff(node->right) < 0) {
            node->right = RightRot(node->right);
        }

        node = LeftRot(node);
        if (node->prev) {
            (side ? node->prev->left : node->prev->right) = node;
        } else {
            root = node;
        }
    } else if (HeightDiff(node) == -2) {
        if (HeightDiff(node->left) > 0) {
            node->left = LeftRot(node->left);
        }

        node = RightRot(node);

        if (node->prev) {
            (side ? node->prev->left : node->prev->right) = node;
        } else {
            root = node;
        }
    } else {
        Rebalance(node->prev);
    }

    return node;
}


template <typename Key, typename T, typename Compare, typename Allocator>
Node<Key, T>* Avl<Key, T, Compare, Allocator>::MinElem(Node<Key, T> *node)
                                                                        const {
    return node && node->left ? MinElem(node->left) : node;
}


template <typename Key, typename T, typename Compare, typename Allocator>
Node<Key, T>* Avl<Key, T, Compare, Allocator>::MaxElem(Node<Key, T> *node)
                                                                        const {
    return node && node->right ? MaxElem(node->right) : node;
}


template <typename Key, typename T, typename Compare, typename Allocator>
Node<Key, T>* Avl<Key, T, Compare, Allocator>::RemoveElem(Node<Key, T> *node) {
    if (!node->left) {
        return node->right;
    }
    node->left = RemoveElem(node->left);
    if (node->left) {
        node->left->prev = node;
    }

    return Rebalance(node);
}


template <typename Key, typename T, typename Compare, typename Allocator>
Node<Key, T>* Avl<Key, T, Compare, Allocator>::Erase(Node<Key, T> *node,
                                                                const Key &k) {
    if (!node) {
        return nullptr;
    }

    if (k == node->pair->first) {
        Node<Key, T> *rsubtree = node->right;
        Node<Key, T> *lsubtree = node->left;
        Node<Key, T> *prev = node->prev;
        bool side;

        if (prev && prev->left &&
                                prev->left->pair->first == node->pair->first) {
            side = true;
        } else {
            side = false;
        }
        std::destroy_n(node->pair, 1);
        alloc.deallocate(node->pair, 1);
        delete node;

        if (!rsubtree) {
            if (lsubtree && lsubtree->prev) {
                lsubtree->prev = prev;
                (side ? lsubtree->prev->left :
                                        lsubtree->prev->right) = lsubtree;
            }
            return lsubtree;
        }

        Node<Key, T> *rmin = MinElem(rsubtree);
        rsubtree->prev = nullptr;
        rmin->right = RemoveElem(rsubtree);
        rmin->left = lsubtree;
        if (rmin->left) {
            rmin->left->prev = rmin;
        }
        if (rmin->right) {
            rmin->right->prev = rmin;
        }
        rmin->prev = prev;

        if (prev) {
            (side ? rmin->prev->left : rmin->prev->right) = rmin;
        }

        return Rebalance(rmin);
    } else if (cmp(k, node->pair->first)) {
        node->left = Erase(node->left, k);
    } else {
        node->right = Erase(node->right, k);
    }


    return Rebalance(node);
}


template <typename Key, typename T, typename Compare, typename Allocator>
void Avl<Key, T, Compare, Allocator>::Clear(Node<Key, T> *node) {
    if (!node) {
        return;
    }

    Clear(node->left);
    Clear(node->right);

    std::destroy_n(node->pair, 1);
    alloc.deallocate(node->pair, 1);
    delete node;
}


template <typename Key, typename T, typename Compare, typename Allocator>
bool Avl<Key, T, Compare, Allocator>::Contains(Node<Key, T> *node,
                                                        const Key &k) const {
     if (!node) {
        return false;
     }

     if (k == node->pair->first) {
         return true;
     } else if (cmp(k, node->pair->first)) {
         return Contains(node->left, k);
     } else {
         return Contains(node->right, k);
     }

     return false;
}


template <typename Key, typename T, typename Compare, typename Allocator>
AvlIterator<Key, T, Compare>
        Avl<Key, T, Compare, Allocator>::Find(Node<Key, T> *node,
                                                                const Key &k) {
    if (!node) {
        return end();
    }

    if (k == node->pair->first) {
        return AvlIterator<Key, T, Compare>(node, false, false);
    } else if (cmp(k, node->pair->first)) {
        return Find(node->left, k);
    } else {
        return Find(node->right, k);
    }

    return end();
}


template <typename Key, typename T, typename Compare, typename Allocator>
Avl<Key, T, Compare, Allocator>::Avl() {
    root = nullptr;
}


template <typename Key, typename T, typename Compare, typename Allocator>
Avl<Key, T, Compare, Allocator>::Avl(const Key &k, T &&val) {
    root = new Node<Key, T>();
    root->pair = alloc.allocate(1);
    new(root->pair)std::pair<const Key, T>(k, val);
}


template <typename Key, typename T, typename Compare, typename Allocator>
Avl<Key, T, Compare, Allocator>::Avl(
                        std::initializer_list<std::pair<const Key, T>> init) {
    auto it = init.begin();

    root = new Node<Key, T>();
    root->pair = alloc.allocate(1);
    new(root->pair)std::pair<const Key, T>((*it).first, (*it).second);
    it++;

    while (it != init.end()) {
        insert(*it);
        ++it;
    }
}


template <typename Key, typename T, typename Compare, typename Allocator>
std::pair<AvlIterator<Key, T, Compare>, bool> Avl<Key, T, Compare,
                    Allocator>::insert(const std::pair<const Key, T> &pair) {
    Node<Key, T> *temp = new Node<Key, T>();
    Node<Key, T> *subtree = root;

    temp->pair = alloc.allocate(1);
    new(temp->pair)std::pair<const Key, T>(pair.first, pair.second);

    if (!subtree) {
        root = temp;
        return std::make_pair(AvlIterator<Key, T, Compare>(root, true, true),
                                                                         true);
    }
    while (true) {
        if (subtree->pair->first == temp->pair->first) {
            std::destroy_n(temp->pair, 1);
            alloc.deallocate(temp->pair, 1);
            delete temp;
            return std::make_pair(AvlIterator<Key, T, Compare>(subtree),
                                                                        false);
        } else if (cmp(subtree->pair->first, temp->pair->first)) {
            if (subtree->right) {
                subtree = subtree->right;
            } else {
                subtree->right = temp;
                subtree->right->prev = subtree;
                Rebalance(subtree);
                return std::make_pair(AvlIterator<Key, T, Compare>(temp),
                                                                         true);
            }
        } else {
            if (subtree->left) {
                subtree = subtree->left;
            } else {
                subtree->left = temp;
                subtree->left->prev = subtree;
                Rebalance(subtree);
                return std::make_pair(AvlIterator<Key, T, Compare>(temp),
                                                                         true);
            }
        }
    }
}


template <typename Key, typename T, typename Compare, typename Allocator>
size_t Avl<Key, T, Compare, Allocator>::erase(const Key &k) {
    auto it = find(k);
    if (it != this->end()) {
        root = Erase(root, k);
    }

    return 1;
}


template <typename Key, typename T, typename Compare, typename Allocator>
auto Avl<Key, T, Compare, Allocator>::erase(auto pos) -> decltype(pos) {
    if (find((*pos).first) != this->end()) {
        auto it = pos + 1;
        erase((*pos).first);

        return it;
    }

    return end();
}


template <typename Key, typename T, typename Compare, typename Allocator>
void Avl<Key, T, Compare, Allocator>::UpdateHeight(Node<Key, T> *node) {
    if (node->left) {
        UpdateHeight(node->left);
    }
    if (node->right) {
        UpdateHeight(node->right);
    }

    int leftHeight = GetHeight(node->left);
    int rightHeight = GetHeight(node->right);

    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}


template <typename Key, typename T, typename Compare, typename Allocator>
int Avl<Key, T, Compare, Allocator>::GetHeight(const Node<Key, T> *node) const {
    return (node ? node->height : 0);
}


template <typename Key, typename T, typename Compare, typename Allocator>
Avl<Key, T, Compare, Allocator>::~Avl() {}


template <typename Key, typename T, typename Compare, typename Allocator>
void Avl<Key, T, Compare, Allocator>::printNode(std::ostream &out,
                                const Node<Key, T> *node, int offset) const {
    if (!node) {
        return;
    }

    for (int i = 0; i < offset; ++i) {
        out << "\t";
    }
    out << format("{0} : {1}\n", node->pair->first, node->pair->second);
    if (node->right) {
        printNode(out, node->right, offset + 1);
    }
    if (node->left) {
        printNode(out, node->left, offset + 1);
    }
}


template <typename Key, typename T, typename Compare, typename Allocator>
int Avl<Key, T, Compare, Allocator>::HeightDiff(const Node<Key, T> *node)
                                                                    const {
    return GetHeight(node->right) - GetHeight(node->left);
}


template <typename Key, typename T, typename Compare, typename Allocator>
bool Avl<Key, T, Compare, Allocator>::empty() const {
    return (root ? false : true);
}


template <typename Key, typename T, typename Compare, typename Allocator>
size_t Avl<Key, T, Compare, Allocator>::size() const {
    return Count(root);
}


template <typename Key, typename T, typename Compare, typename Allocator>
size_t Avl<Key, T, Compare, Allocator>::Count(const Node<Key, T> *node) const {
    if (!node) {
        return 0;
    }

    return Count(node->left) + Count(node->right) + 1;
}


template <typename Key, typename T, typename Compare, typename Allocator>
void Avl<Key, T, Compare, Allocator>::clear() {
    Clear(root);
    root = nullptr;
}


template <typename Key, typename T, typename Compare, typename Allocator>
bool Avl<Key, T, Compare, Allocator>::contains(const Key &k) const {
    return Contains(root, k);
}


template <typename Key, typename T, typename Compare, typename Allocator>
AvlIterator<Key, T, Compare> Avl<Key, T, Compare, Allocator>::find(
                                                                const Key &k) {
    return Find(root, k);
}


template <typename Key, typename T, typename Compare, typename Allocator>
T& Avl<Key, T, Compare, Allocator>::at(const Key &k) {
    auto it = find(k);

    if (it == end()) {
        throw std::out_of_range("Avl::at");
    }

    return ((*it).second);
}


template <typename Key, typename T, typename Compare, typename Allocator>
T& Avl<Key, T, Compare, Allocator>::operator[](const Key &k) {
    auto it = find(k);

    if (it == end()) {
        auto pair = insert(std::make_pair(k, T()));
        return (*pair.first).second;
    }

    return (*it).second;
}


template <typename Key, typename T, typename Compare, typename Allocator>
T& Avl<Key, T, Compare, Allocator>::operator[](const Key &&k) {
    auto it = find(k);

    if (it == end()) {
        auto pair = insert(std::make_pair(k, T()));
        return (*pair.first).second;
    }

    return (*it).second;
}


template <typename Key, typename T, typename Compare, typename Allocator>
bool operator==(const Avl<Key, T, Compare, Allocator> &lhs,
                const Avl<Key, T, Compare, Allocator> &rhs) {
    if (lhs.size() == rhs.size()) {
            auto it1 = lhs.begin();
            auto it2 = rhs.begin();

            while (it1 != lhs.end()) {
                if (*it1 != *it2) {
                    return false;
                }
                it1++;
                it2++;
            }
    } else {
        return false;
    }

    return true;
}


template <typename Key, typename T, typename Compare, typename Allocator>
std::ostream& operator<<(std::ostream &out,
                                const Avl<Key, T, Compare, Allocator> &avl) {
    avl.printNode(out, avl.root, 0);

    return out;
}

#endif  // AVLMAP_AVLMAP_AVL_HPP_

