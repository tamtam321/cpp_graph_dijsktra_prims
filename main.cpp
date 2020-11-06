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
/// Test 1: Build up graph from txt file, add & remove vertex.
//    myGraph mg1("graph_02.txt");
//    mg1.addVertex('w', {{'h', 1}, {'c', 2}, {'d', 3}});
//    mg1.removeVertex('w');
//    mg1.removeVertex('f');
//    mg1.addVertex('f', {{'c', 4}, {'d', 14}, {'e', 10}, {'g', 2}});

/// Test 2: Build up graph from adding vertex.
//    myGraph mg3;
//    mg3.addVertex('a', {{'b', 3}, {'d', 6}, {'j', 9}});
//    mg3.addVertex('b', {{'a', 3}});
//    mg3.addVertex('d', {{'a', 6}});
//    mg3.addVertex('j', {{'a', 9}});

//    mg3.addVertex('b', {{}});
//    mg3.addVertex('d', {{'b', 4}});
//    mg3.addVertex('j', {{'d', 5}, {'b', 12}});
//    mg3.addVertex('a', {{'b', 3}, {'d', 6}, {'j', 9}});

/// Test 3: FAIL -> tried to connect d to j but j was not exist.
//    mg3.addVertex('b', {{'d', 4}});
//    mg3.addVertex('d', {{'j', 4}});
//    mg3.addVertex('j', {{'b', 5}});
//    mg3.addVertex('a', {{'b', 3}, {'d', 6}, {'j', 9}});

/// Test 4: Remove vertex & consider articulation points
//    myGraph mg3;
//    mg3.addVertex('a', {});
//    mg3.addVertex('b', {{'a', 2}});
//    mg3.addVertex('c', {});
//    mg3.addVertex('d', {{'c', 1}});
//    mg3.addVertex('f', {});
//    mg3.addVertex('e', {{'f', 5}});
//    mg3.addVertex('h', {});
//    mg3.addVertex('g', {{'h', 8}});
//    mg3.addVertex('i', {{'b', 4}, {'d', 6}, {'e', 3}, {'g', 9}});
//    mg3.removeVertex('i');

/// Test 5: Remove vertex & consider articulation points part 2
//    myGraph mg3;
//    mg3.addVertex('a', {});
//    mg3.addVertex('b', {{'a', 2}});
//    mg3.addVertex('c', {});
//    mg3.addVertex('d', {{'c', 1}});
//    mg3.addVertex('f', {});
//    mg3.addVertex('e', {{'f', 5}});
//    mg3.addVertex('h', {});
//    mg3.addVertex('g', {{'h', 8}});
//    mg3.addVertex('i', {{'b', 4}, {'d', 6}, {'e', 3}, {'g', 9}});
//    mg3.removeVertex('a');
//    mg3.removeVertex('b');
//    mg3.removeVertex('f');
//    mg3.removeVertex('e');
//    mg3.removeVertex('h');
//    mg3.removeVertex('g');
//    mg3.removeVertex('c');
//    mg3.removeVertex('d');
//    mg3.removeVertex('i');
//
//    mg3.addVertex('i', {});
//
//    mg3.removeVertex('i');
//    mg3.removeVertex('i');



    return 0;
}
