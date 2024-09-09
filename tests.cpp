// Copyright (c) 2024 PlatinumSamurai. All rights reserved.

#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include "avlmap/avl.hpp"


TEST(avl_test, insert_test) {
    Avl<int, std::string> tree2(5, "hello");
    tree2.insert(std::make_pair(3, "bye"));
    tree2.insert(std::make_pair(6, "me"));
    tree2.insert(std::make_pair(4, "ou"));
    tree2.insert(std::make_pair(12, "ok"));
    tree2.insert(std::make_pair(9, "ol"));
    tree2.insert(std::make_pair(7, "oh"));
    tree2.insert(std::make_pair(10, "og"));
    tree2.insert(std::make_pair(15, "of"));
    tree2.insert(std::make_pair(16, "od"));
    tree2.insert(std::make_pair(17, "os"));

    std::stringstream str1;
    std::stringstream str2;
    std::ifstream fin("resources/insert_test.txt");

    str2 << fin.rdbuf();
    str1 << tree2;

    fin.close();

    ASSERT_EQ(str1.str(), str2.str());

    tree2.clear();
}


TEST(avl_test, erase_by_key_test) {
    Avl<int, std::string> tree({{5, "hello"}, {3, "bye"}, {6, "me"}, {4, "ou"},
                                {12, "ok"}, {9, "ol"}, {7, "oh"}, {10, "og"},
                                {15, "of"}, {16, "od"}, {17, "os"}});
    Avl<int, std::string> tree2({{10, "og"}, {15, "of"}, {5, "hello"},
                                 {6, "me"}, {4, "ou"}, {12, "ok"}, {7, "oh"},
                                 {16, "od"}});


    tree.erase(17);
    tree.erase(3);
    tree.erase(9);

    ASSERT_EQ(tree, tree2);

    tree.clear();
    tree2.clear();
}


TEST(avl_test, erase_by_iterator_test) {
    Avl<int, std::string> tree({{5, "hello"}, {3, "bye"}, {6, "me"}, {4, "ou"},
                                {12, "ok"}, {9, "ol"}, {7, "oh"}, {10, "og"},
                                {15, "of"}, {16, "od"}, {17, "os"}});
    Avl<int, std::string> tree2({{10, "og"}, {15, "of"}, {5, "hello"},
                                 {6, "me"}, {4, "ou"}, {12, "ok"}, {7, "oh"},
                                 {16, "od"}});

    for (int item : std::vector<int>{17, 3, 9}) {
        auto it = tree.begin();
        while (it != tree.end()) {
            if ((*it).first == item) {
                tree.erase(it);
                break;
            }
            it++;
        }
    }

    ASSERT_EQ(tree, tree2);

    tree.clear();
    tree2.clear();
}



TEST(avl_test, empty_test) {
    Avl<int, std::string> tree({{5, "hello"}, {3, "bye"}, {6, "me"},
                                {4, "ou"}});

    tree.erase(5);
    ASSERT_FALSE(tree.empty());

    tree.erase(tree.begin());
    ASSERT_FALSE(tree.empty());

    tree.erase(--(tree.end()));
    ASSERT_FALSE(tree.empty());

    tree.erase(4);
    ASSERT_TRUE(tree.empty());

    tree.clear();
}


TEST(avl_test, size_test) {
    Avl<int, std::string> tree;

    ASSERT_EQ(tree.size(), 0);

    tree.insert({3, "bye"});
    ASSERT_EQ(tree.size(), 1);

    tree.insert({3, "bye"});
    ASSERT_EQ(tree.size(), 1);

    tree.insert({4, "hello"});
    ASSERT_EQ(tree.size(), 2);

    tree.erase(3);
    ASSERT_EQ(tree.size(), 1);

    tree.erase(3);
    ASSERT_EQ(tree.size(), 1);

    tree.clear();
}


TEST(avl_test, clear_test) {
    Avl<int, std::string> tree({{5, "hello"}, {3, "bye"}, {6, "me"},
                                {4, "ou"}});

    ASSERT_EQ(tree.size(), 4);

    tree.clear();
    ASSERT_TRUE(tree.empty());

    tree.clear();
    ASSERT_EQ(tree.size(), 0);

    tree.clear();
}


TEST(avl_test, too_big_int_value_test) {
    Avl<int, std::string> tree({{5, "hello"}, {3, "bye"}, {6, "me"},
                                {4, "ou"}});

    ASSERT_TRUE(tree.contains(5));
    ASSERT_FALSE(tree.contains(10));

    tree.erase(5);
    ASSERT_FALSE(tree.contains(5));
    ASSERT_FALSE(tree.contains(10));

    tree.insert({10, "too"});
    ASSERT_TRUE(tree.contains(10));

    tree.clear();
}


TEST(avl_test, find_test) {
    Avl<int, std::string> tree({{5, "hello"}, {3, "bye"}, {6, "me"},
                                {4, "ou"}});

    auto it = tree.find(5);
    ASSERT_NE(it, tree.end());
    ASSERT_EQ((*it).second, "hello");
    ASSERT_EQ(tree.find(23), tree.end());

    tree.insert({23, "try"});
    it = tree.find(23);
    ASSERT_NE(it, tree.end());
    ASSERT_EQ((*it).second, "try");

    tree.clear();
}


TEST(avl_test, at_test) {
    Avl<int, std::string> tree({{5, "hello"}, {3, "bye"}, {6, "me"},
                                {4, "ou"}});

    ASSERT_NO_THROW(tree.at(5));
    tree.at(5) = "it's me";
    auto it = tree.find(5);
    ASSERT_EQ((*it).second, "it's me");

    ASSERT_THROW(tree.at(25), std::out_of_range);

    tree.clear();
}


TEST(avl_test, operator_square_parenthences_test) {
    Avl<int, std::string> tree;
    std::string str = "it's me";

    ASSERT_TRUE(tree.empty());

    tree[5] = "hello";
    ASSERT_FALSE(tree.empty());
    ASSERT_EQ(tree[5], "hello");

    tree[10] = std::move(str);
    ASSERT_EQ(tree[10], "it's me");
    ASSERT_TRUE(str.empty());

    tree.clear();
}


TEST(avl_test, use_of_stl_algorithms_test) {
    Avl<int, std::string> tree({{5, "hello"}, {3, "bye"}, {6, "me"}, {4, "ou"},
                                {12, "ok"}, {9, "ol"}, {7, "oh"}, {10, "og"},
                                {15, "of"}, {16, "od"}, {17, "os"},
                                {25, "hello"}});

    bool b = std::all_of(tree.rbegin(), tree.rend(),
                                [](auto item){return item.second != "Alexey";});
    ASSERT_TRUE(b);

    int count = std::count_if(tree.begin(), tree.end(),
                                [](auto item){return item.second == "hello";});
    ASSERT_EQ(count, 2);

    tree.clear();
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

