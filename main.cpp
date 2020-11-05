#include "my_class_graph.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <utility>

#include <exception>

int main()
{
/// Test 1
//    myGraph mg1("graph_02.txt");
//    mg1.addVertex('w', {{'h', 1}, {'c', 2}, {'d', 3}});
//    mg1.removeVertex('w');
//    mg1.removeVertex('f');
//    mg1.addVertex('f', {{'c', 4}, {'d', 14}, {'e', 10}, {'g', 2}});

/// Test 2
//    std::map<char, std::vector<int>> map1;
//    map1 = {{'a', {1}},{'b', {2}},{'c', {3}}};
//    map1['d'] = {4};
//    map1['a'].push_back(5);
//    map1['e'].push_back(6);
//    map1.insert(std::pair<char, std::vector<int>>('w', {9}));

/// Test 3
//    myGraph mg3;
//    mg3.addVertex('a', {{'b', 3}, {'d', 6}, {'j', 9}});
//    mg3.addVertex('b', {{'a', 3}});
//    mg3.addVertex('d', {{'a', 6}});
//    mg3.addVertex('j', {{'a', 9}});

//    mg3.addVertex('b', {{}});
//    mg3.addVertex('d', {{'b', 4}});
//    mg3.addVertex('j', {{'d', 5}, {'b', 12}});
//    mg3.addVertex('a', {{'b', 3}, {'d', 6}, {'j', 9}});

/// FAIL
//    mg3.addVertex('b', {{'d', 4}});
//    mg3.addVertex('d', {{'j', 4}});
//    mg3.addVertex('j', {{'b', 5}});
//    mg3.addVertex('a', {{'b', 3}, {'d', 6}, {'j', 9}});

/// Test 4
    myGraph mg3;
    mg3.addVertex('a', {});
    mg3.addVertex('b', {{'a', 2}});
    mg3.addVertex('c', {});
    mg3.addVertex('d', {{'c', 1}});
    mg3.addVertex('f', {});
    mg3.addVertex('e', {{'f', 5}});
    mg3.addVertex('h', {});
    mg3.addVertex('g', {{'h', 8}});
    mg3.addVertex('i', {{'b', 4}, {'d', 6}, {'e', 3}, {'g', 9}});
    mg3.removeVertex('i');

    return 0;
}
