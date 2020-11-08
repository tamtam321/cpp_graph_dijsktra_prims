#include "my_class_graph.hpp"

#include <iostream>

int main()
{
/// Test 1: Build up graph from txt file, add & remove vertex.
    std::cout << "\n(Test 1): Build up graph from txt file, add & remove vertex." << std::endl;

    myGraph mg1("graph_02.txt");

    mg1.addVertex('w', {{'h', 1}, {'c', 2}, {'d', 3}});

    mg1.removeVertex('w');
    mg1.removeVertex('f');

    mg1.addVertex('f', {{'c', 4}, {'d', 14}, {'e', 10}, {'g', 2}});

/// Test 2: Build up graph from adding vertex part 1.
    std::cout << "\n(Test 2): Build up graph from adding vertex part 1." << std::endl;

    myGraph mg2;

    mg2.addVertex('a', {{'b', 3}, {'d', 6}, {'j', 9}});
    mg2.addVertex('b', {{'a', 3}});
    mg2.addVertex('d', {{'a', 6}});
    mg2.addVertex('j', {{'a', 9}});

/// Test 3: Build up graph from adding vertex part 2.
    std::cout << "\n(Test 3): Build up graph from adding vertex part 2." << std::endl;

    myGraph mg3;

    mg3.addVertex('b', {{}});
    mg3.addVertex('d', {{'b', 4}});
    mg3.addVertex('j', {{'d', 5}, {'b', 12}});
    mg3.addVertex('a', {{'b', 3}, {'d', 6}, {'j', 9}});

/// Test 4: FAIL -> tried to connect d to j but j was not exist in that time.
    std::cout << "\n(Test 4): FAIL -> tried to connect d to j but j was not exist in that time." << std::endl;

    myGraph mg4;

    mg4.addVertex('b', {{'d', 4}});
    mg4.addVertex('d', {{'j', 4}});
    mg4.addVertex('j', {{'b', 5}});
    mg4.addVertex('a', {{'b', 3}, {'d', 6}, {'j', 9}});

/// Test 5: Remove vertex & consider articulation points
    std::cout << "\n(Test 5): Remove vertex & consider articulation points" << std::endl;

    myGraph mg5;

    mg5.addVertex('a', {});
    mg5.addVertex('b', {{'a', 2}});
    mg5.addVertex('c', {});
    mg5.addVertex('d', {{'c', 1}});
    mg5.addVertex('f', {});
    mg5.addVertex('e', {{'f', 5}});
    mg5.addVertex('h', {});
    mg5.addVertex('g', {{'h', 8}});
    mg5.addVertex('i', {{'b', 4}, {'d', 6}, {'e', 3}, {'g', 9}});

    // This is an articulation point. There will be an error message.
    mg5.removeVertex('i');

/// Test 6: Remove vertex & consider articulation points part 2
/// (Itt exception-t kell, hogy dobjon, de mivel emiatt nem halad a teszt, ezért kikommenteltem.)
//    std::cout << "\n(Test 6): Remove vertex & consider articulation points part 2" << std::endl;
//
//    myGraph mg6;
//
//    mg6.addVertex('a', {});
//    mg6.addVertex('b', {{'a', 2}});
//    mg6.addVertex('c', {});
//    mg6.addVertex('d', {{'c', 1}});
//    mg6.addVertex('f', {});
//    mg6.addVertex('e', {{'f', 5}});
//    mg6.addVertex('h', {});
//    mg6.addVertex('g', {{'h', 8}});
//    mg6.addVertex('i', {{'b', 4}, {'d', 6}, {'e', 3}, {'g', 9}});
//
//    mg6.removeVertex('a');
//    mg6.removeVertex('b');
//    mg6.removeVertex('f');
//    mg6.removeVertex('e');
//    mg6.removeVertex('h');
//    mg6.removeVertex('g');
//    mg6.removeVertex('c');
//    mg6.removeVertex('d');
//    mg6.removeVertex('i');
//
//    mg6.addVertex('i', {});
//
//    mg6.removeVertex('i');
//
//    // Vertex is empty, you unable to delete anything. There will be an error message.
//    mg6.removeVertex('i');

/// Test 7: Remove vertex & consider articulation points part 3
/// (Itt exception-t kell, hogy dobjon, de mivel emiatt nem halad a teszt, ezért kikommenteltem!)
//    std::cout << "\n(Test 7): Remove vertex & consider articulation points part 3" << std::endl;
//
//    myGraph mg7("graph_02.txt");
//
//    mg7.addVertex('w', {{'h', 1}, {'c', 2}, {'d', 3}});
//
//    mg7.removeVertex('w');
//    mg7.removeVertex('f');
//
//    mg7.addVertex('f', {{'c', 4}, {'d', 14}, {'e', 10}, {'g', 2}});
//
//    mg7.removeVertex('c');
//    mg7.removeVertex('a');
//    mg7.removeVertex('b');
//    mg7.removeVertex('h');
//    mg7.removeVertex('i');
//    mg7.removeVertex('g');
//    mg7.removeVertex('f');
//    mg7.removeVertex('d');
//    mg7.removeVertex('e');
//
//    // Itt már üres a gráf és jelezni-e kéne felénk.
//    mg7.removeVertex('f');

/// Test 8: Dijkstra Shortest Path part 1
    std::cout << "\n(Test 8): Dijkstra Shortest Path part 1" << std::endl;

    myGraph mg8("graph_02.txt");

    mg8.dijkstra('a');

/// Test 9: Dijkstra Shortest Path part 2
    std::cout << "\n(Test 9): Dijkstra Shortest Path part 2" << std::endl;

    myGraph mg9("graph_01.txt");

    mg9.dijkstra('h');

/// Test 10: Dijkstra Shortest Path After Remove part 1
    std::cout << "\n(Test 10): Dijkstra Shortest Path After Remove part 1" << std::endl;

    myGraph mg10("graph_02.txt");

    mg10.dijkstra('a');

    mg10.removeVertex('c');

    mg10.dijkstra('w');

/// Test 11: Dijkstra Shortest Path After Remove part 2
/// (Itt exception-t kell, hogy dobjon, de mivel emiatt nem halad a teszt, ezért kikommenteltem.)
//    std::cout << "\n(Test 11): Dijkstra Shortest Path After Remove part 2" << std::endl;
//
//    myGraph mg11("graph_02.txt");
//
//    mg11.dijkstra('a');
//
//    mg11.removeVertex('c');
//    mg11.removeVertex('d');
//    mg11.removeVertex('i');
//    mg11.removeVertex('e');
//    mg11.removeVertex('f');
//    mg11.removeVertex('g');
//    mg11.removeVertex('b');
//    mg11.removeVertex('h');
//    mg11.removeVertex('a');
//
//    mg11.dijkstra('h');

/// Test 12: Prim's Minimum Spanning Tree part 1
    std::cout << "\n(Test 12): Prim's Minimum Spanning Tree part 1" << std::endl;

    myGraph mg12("graph_02.txt");

    mg12.prim('b');

/// Test 13: Prim's Minimum Spanning Tree part 2
    std::cout << "\n(Test 13): Prim's Minimum Spanning Tree part 2" << std::endl;

    myGraph mg13("graph_03.txt");

    mg13.prim('b');

/// Test 14: Prim's Minimum Spanning Tree part 3
    std::cout << "\n(Test 14): Prim's Minimum Spanning Tree part 3" << std::endl;

    myGraph mg14("graph_01.txt");

    mg14.prim('g');

/// Test 15: Prim's Minimum Spanning Tree After Remove part 1
    std::cout << "\n(Test 15): Prim's Minimum Spanning Tree After Remove part 1" << std::endl;

    myGraph mg15("graph_01.txt");

    mg15.prim('b');
    mg15.prim('a');

    mg15.removeVertex('j');
    mg15.removeVertex('h');
    mg15.removeVertex('g');
    mg15.removeVertex('d');

    mg15.prim('c');

/// Test 16: Prim's Minimum Spanning Tree After Remove part 2
    std::cout << "\n(Test 16): Prim's Minimum Spanning Tree After Remove part 2" << std::endl;

    myGraph mg16("graph_01.txt");

    mg16.prim('b');
    mg16.prim('a');

    mg16.removeVertex('j');
    mg16.removeVertex('h');
    mg16.removeVertex('g');
    mg16.removeVertex('d');

    mg16.prim('c');

    // g is not exist -> error
    mg16.removeVertex('g');

    mg16.removeVertex('i');
    mg16.removeVertex('a');
    mg16.removeVertex('b');
    mg16.removeVertex('c');
    mg16.removeVertex('e');
    mg16.removeVertex('f');

    /// (Itt exception-t kell, hogy dobjon, de mivel emiatt nem halad a teszt, ezért kikommenteltem.)
    // now we have an empty graph
    // deletion can't be allowed
//    mg16.removeVertex('a');

/// Test 17: Check Dijkstra & Prim After Adding Vertex part 1
    std::cout << "\n(Test 17): Check Dijkstra & Prim After Adding Vertex part 1" << std::endl;

    myGraph mg17;

    mg17.addVertex('a', {});
    mg17.addVertex('b', {{'a', 2}});
    mg17.addVertex('c', {});
    mg17.addVertex('d', {{'c', 1}});
    mg17.addVertex('f', {});
    mg17.addVertex('e', {{'f', 5}});
    mg17.addVertex('h', {});
    mg17.addVertex('g', {{'h', 8}});
    mg17.addVertex('i', {{'b', 4}, {'d', 6}, {'e', 3}, {'g', 9}});

    mg17.prim('i');
    mg17.dijkstra('i');

/// Test 18: Check Dijkstra & Prim After Adding Vertex part 2
    std::cout << "\n(Test 18): Check Dijkstra & Prim After Adding Vertex part 2" << std::endl;

    myGraph mg18;

    mg18.addVertex('a', {});
    mg18.addVertex('d', {{'a', 4}});
    mg18.addVertex('e', {{'a', 8}, {'d', 11}});
    mg18.addVertex('b', {{'e', 7}});
    mg18.addVertex('h', {{'d', 8}, {'b', 2}});
    mg18.addVertex('i', {{'e', 1}, {'b', 6}});
    mg18.addVertex('f', {{'i', 2}, {'h', 4}});
    mg18.addVertex('c', {{'h', 7}, {'f', 14}});
    mg18.addVertex('g', {{'c', 9}, {'f', 10}});

    mg18.prim('a');
    mg18.dijkstra('a');

    return 0;
}
