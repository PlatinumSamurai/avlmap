// Copyright (c) 2024 PlatinumSamurai. All rights reserved.

#ifndef AVLMAP_AVLMAP_NODE_HPP_
#define AVLMAP_AVLMAP_NODE_HPP_

#include <utility>


template <typename Key, typename T>
struct Node {
    std::pair<const Key, T> *pair;
    Node *prev;
    Node *left;
    Node *right;
    int height;

    Node() {
        pair = nullptr;
        prev = nullptr;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
    ~Node() {
        prev = nullptr;
        left = nullptr;
        right = nullptr;
    }
};


template <typename Key, typename T>
bool operator==(const Node<Key, T> &lhs, const Node<Key, T> &rhs) {
    return *(lhs.pair) == *(rhs.pair);
}


#endif  // AVLMAP_AVLMAP_NODE_HPP_

