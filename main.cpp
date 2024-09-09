#include <iostream>
#include <string>
#include <fstream>

#include "avlmap/avl.hpp"


int main() {
    Avl<int, std::string> tree({{5, "hello"}, {3, "bye"}, {6, "me"}, {4, "you"},
                                {12, "ok"}, {9, "micky"}, {7, "john"}, {10, "jimmy"},
                                {15, "russel"}, {16, "joe"}, {17, "magnus"}});

    if (tree.contains(12)) {
        tree.erase(tree.find(15));
    }

    std::cout << tree;

    tree.clear();

    return 0;
}

