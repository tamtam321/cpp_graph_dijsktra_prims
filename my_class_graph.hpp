#ifndef CODE_MY_CLASS_GRAPH_HPP
#define CODE_MY_CLASS_GRAPH_HPP

#include <iostream>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <utility>
#include <tuple>
#include <vector>

#include <exception>

class InvalidFileName : public std::exception
{
public:
    const char *what() const noexcept { return "Invalid filename !"; }
};

class myGraph
{
    struct Node
    {
        char value;
        std::map<char, int> adj;

        Node() = default;
        Node(char vrtx);
        Node(char vrtx, std::map<char, int> adjacency);
    };

    std::map<char, Node*> vertexes;

public:
    myGraph() = default;
    ~myGraph() = default;
    myGraph(std::string file_name);

    void updateAdjacency(Node *p);
    bool isEmpty();
    void addVertex(char vrtx, std::map<char, int> adjacency);
};

myGraph::Node::Node(char vrtx)
{
    this->value = vrtx;
}

myGraph::Node::Node(char vrtx, std::map<char, int> adjacency)
{
    this->value = vrtx;
    this->adj = adjacency;
}

myGraph::myGraph(std::string file_name)
{
    std::ifstream f_in(file_name);

    if (f_in.fail())
    {
        throw InvalidFileName();
    }

    std::string str_vertex;
    std::string str_adjacent;
    std::string str_distance;
    std::stringstream ss;
    std::stringstream ss2;

    while (!f_in.eof())
    {
        Node *p = new Node();

        std::getline(f_in, str_vertex);
        std::getline(f_in, str_adjacent);
        std::getline(f_in, str_distance);

        p->value = str_vertex[0];

        ss << str_adjacent;
        ss2 << str_distance;

        for (char ch_adj; ss >> ch_adj;)
        {
            int adj_dis;
            ss2 >> adj_dis;
            p->adj.insert(std::pair<char, int>(ch_adj, adj_dis));

            if (ss.peek() == ',' && ss2.peek() == ',')
            {
                ss.ignore();
                ss2.ignore();
            }
        }

        ss.clear();
        ss2.clear();

        vertexes[p->value] = p;
    }
}

void myGraph::updateAdjacency(Node *p)
{
    for (auto [v_key, v_val] : p->adj)
    {
        vertexes[v_key]->adj.insert(std::pair<char, int>(p->value, v_val));
    }
}

bool myGraph::isEmpty()
{
    return vertexes.empty();
}

void myGraph::addVertex(char vrtx, std::map<char, int> adjacency)
{
    if (isEmpty())
    {
        Node *p = new Node(vrtx);
        vertexes[p->value] = p;
        return;
    }
    else
    {
        Node *p = new Node(vrtx, adjacency);
        updateAdjacency(p);
        vertexes[p->value] = p;
    }
}

#endif //CODE_MY_CLASS_GRAPH_HPP
