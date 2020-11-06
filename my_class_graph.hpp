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
#include <set>

#include <exception>

class InvalidFileName : public std::exception
{
public:
    const char *what() const noexcept { return "Invalid filename!"; }
};

class UnderFlowException : public std::exception {
public:
    const char *what() const noexcept { return "Graph is empty, you can't remove any node!"; }
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

    bool isEmpty();
    void update_adjacency_after_insert(Node *p);
    void update_adjacency_after_remove(Node *p);
    void DFS(std::set<char> &visited, std::set<char> &articulationPoints, Node *vertex,
             std::map<char, int> &visitedTime, std::map<char, int> &lowTime,
             std::map<char, char> &parent, int &time);
    bool is_articulation_point(Node *p);

public:
    myGraph() = default;
    ~myGraph();
    myGraph(std::string file_name);

    myGraph(const myGraph &mg) = delete;
    myGraph &operator=(const myGraph &mg) = delete;
    myGraph &operator=(myGraph &&mg) = delete;
    myGraph(myGraph &&mg) = delete;

    void addVertex(char vrtx, std::map<char, int> adjacency);
    void removeVertex(char vrtx);
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

myGraph::~myGraph()
{
    std::map<char, Node*>::iterator it;
    Node *tmp;

    for (it = vertexes.begin(); it != vertexes.end(); it++)
    {
        tmp = it->second;
        delete tmp;
    }
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

void myGraph::update_adjacency_after_insert(Node *p)
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
        update_adjacency_after_insert(p);
        vertexes[p->value] = p;
    }
}

void myGraph::update_adjacency_after_remove(Node *p)
{
    for (auto [v_key, v_val] : p->adj)
    {
        vertexes[v_key]->adj.erase(p->value);
    }
}

void myGraph::removeVertex(char vrtx)
{
    if (vertexes.empty())
    {
        throw UnderFlowException();
    }

    Node *p = vertexes[vrtx];

    if (is_articulation_point(p))
    {
        try
        {
            throw vrtx;
        }
        catch (char ex)
        {
            std::string s(1, ex);
            std::cout << "Can't remove \"" + s + "\" node, because\n"
                         "this is an articulation point!\n"
                         "Deletion is allowed, if no other nodes or subtrees are isolated after remove." << std::endl;
        }
    }
    else
    {
        update_adjacency_after_remove(p);
        vertexes.erase(p->value);
        delete p;
    }
}

void myGraph::DFS(std::set<char> &visited, std::set<char> &articulationPoints,
                  Node *vertex, std::map<char, int> &visitedTime,
                  std::map<char, int> &lowTime, std::map<char, char> &parent, int &time)
{
    visited.insert(vertex->value);
    visitedTime.insert({vertex->value, time});
    lowTime.insert({vertex->value, time});
    time++;
    int childCount = 0;
    bool isArticulationPoint = false;

    for (auto [_adj, distance] : vertex->adj)
    {
        if (parent.find(vertex->value) != parent.end() && _adj == parent[vertex->value])
        {
            continue;
        }

        if (!visited.contains(_adj))
        {
            parent.insert({_adj, vertex->value});
            childCount++;
            DFS(visited, articulationPoints, vertexes[_adj], visitedTime, lowTime, parent, time);

            if (visitedTime[vertex->value] <= lowTime[_adj])
            {
                isArticulationPoint = true;
            }
            else
            {
                lowTime[vertex->value] = std::min(lowTime[vertex->value], lowTime[_adj]);
            }
        }
        else
        {
            lowTime[vertex->value] = std::min(lowTime[vertex->value], lowTime[_adj]);
        }
    }

    if ((parent.find(vertex->value) == parent.end() && childCount >= 2) ||
    (parent.find(vertex->value) != parent.end() && isArticulationPoint))
    {
        articulationPoints.insert(vertex->value);
    }
}

bool myGraph::is_articulation_point(Node *p)
{
    bool sol = false;
    int time = 0;
    std::set<char> visited;
    std::set<char> articulationPoints;
    std::map<char, int> visitedTime;
    std::map<char, int> lowTime;
    std::map<char, char> parent;
    Node *startVertex = p;

    DFS(visited, articulationPoints, startVertex, visitedTime, lowTime, parent, time);

    if (articulationPoints.contains(p->value))
    {
        sol = true;
    }

//    for (char ch : articulationPoints)
//    {
//        std::cout << ch << std::endl;
//    }

    return sol;
}

#endif //CODE_MY_CLASS_GRAPH_HPP
