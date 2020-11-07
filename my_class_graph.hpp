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

# define INF 0x3f3f3f3f

#include <exception>

class InvalidFileName : public std::exception
{
public:
    const char *what() const noexcept { return "Invalid filename!"; }
};

class UnderFlowException : public std::exception
{
public:
    const char *what() const noexcept { return "Graph is empty, you can't remove any node!"; }
};

class EmptyGraph : public std::exception
{
public:
    const char *what() const noexcept { return "Graph is empty, can't do Dijkstra or Prim!"; }
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

    std::string dijkstra_path(std::map<char, char> parent, char from, char to);

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
    void dijkstra(char src);
    void prim(char src);
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
    else if(vertexes.find(vrtx) == vertexes.end())
    {
        try
        {
            throw vrtx;
        }
        catch (char ex)
        {
            std::string s(1, ex);
            std::cout << "The \"" + s + "\" node is not exist, you can't remove it!";
        }

        return;
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
                         "Deletion is allowed, if no other nodes or subtrees are isolated after remove.\n" << std::endl;
        }

        return;
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

    return sol;
}

std::string myGraph::dijkstra_path(std::map<char, char> parent, char from, char to)
{
    std::string path = "";

    path += to;
    std::string arrow = "--->";

    while (from != to)
    {
        path = parent[to] + arrow + path;
        to = parent[to];
    }

    return path;
}

void myGraph::dijkstra(char src)
{
    if (vertexes.empty())
    {
        throw EmptyGraph();
    }
    else if (vertexes.find(src) == vertexes.end())
    {
        std::string s(1, src);
        std::cout << "The \"" + s + "\" node is not exist, so it could not be the source of the Dijkstra's algorithm.\n";
        return;
    }

    std::map<char, bool> visited;
    std::map<char, char> parent;
    std::map<char, int> d;
    std::set<char> unvisited;
    int curr_min = INF;
    std::pair<char, int> curr_process_vertex = std::make_pair('!', INF);
    std::map<char, Node*> vertexes_copy = vertexes;

    for (auto [vertex, distance] : vertexes_copy)
    {
        d.insert(std::make_pair(vertex, INF));
        visited.insert(std::make_pair(vertex, false));
        unvisited.insert(vertex);
    }

    d[src] = 0;
    Node *curr_vertex = vertexes_copy[src];

    while (!unvisited.empty())
    {
        for (auto [adjacent, adj_distance] : curr_vertex->adj)
        {
            curr_min = std::min(d[curr_vertex->value] + adj_distance, d[adjacent]);

            if (curr_min < d[adjacent])
            {
                if (parent.find(adjacent) == parent.end())
                {
                    parent.insert(std::make_pair(adjacent, curr_vertex->value));
                    d[adjacent] = curr_min;
                }
                else
                {
                    parent[adjacent] = curr_vertex->value;
                    d[adjacent] = curr_min;
                }
            }
        }

        unvisited.erase(curr_vertex->value);
        visited[curr_vertex->value] = true;
        curr_process_vertex = std::make_pair('!', INF);

        for (auto [vertex, distance] : d)
        {
            if (visited[vertex] == false)
            {
                if (curr_process_vertex.second >= distance)
                {
                    curr_process_vertex = std::make_pair(vertex, distance);
                }
            }
        }

        curr_vertex = vertexes_copy[curr_process_vertex.first];
    }

    std::cout << "Dijkstra's Shortest Path:" << std::endl;
    std::cout << "Direction     Distance       Path" << std::endl;

    for (auto [vertex, distance] : d)
    {
        std::string path = dijkstra_path(parent, src, vertex);
        std::cout << src << "->" << vertex << "          " << distance << "              " << path << std::endl;
    }

    std::cout << std::endl;
}

void myGraph::prim(char src)
{
    if (vertexes.empty())
    {
        throw EmptyGraph();
    }
    else if (vertexes.find(src) == vertexes.end())
    {
        std::string s(1, src);
        std::cout << "The \"" + s + "\" node is not exist, so it could not be the source of the Dijkstra's algorithm.\n";
        return;
    }

    std::map<char, Node*> vertexes_copy = vertexes;
    std::map<char, char> parent;
    std::map<char, bool> visited;
    std::map<char, int> mst;
    std::set<char> unvisited;

    for (auto [vertex, distance] : vertexes_copy)
    {
        mst.insert(std::make_pair(vertex, INF));
        visited.insert(std::make_pair(vertex, false));
        parent.insert(std::make_pair(vertex, '!'));
        unvisited.insert(vertex);
    }

    mst[src] = 0;
    Node *curr_vertex = vertexes_copy[src];
    std::pair<char, int> curr_min_vertex = std::make_pair('!', INF);
    unvisited.erase(src);
    visited[curr_vertex->value] = true;
    parent[src] = src;

    while (!unvisited.empty())
    {
        for (auto [adjacent, adj_distance] : curr_vertex->adj)
        {
            if (visited[adjacent] == false)
            {
                if (adj_distance < mst[adjacent])
                {
                    mst[adjacent] = adj_distance;
                    parent[adjacent] = curr_vertex->value;
                }
            }
        }

        for (auto [vertex, distance] : mst)
        {
            if (curr_min_vertex.second > distance)
            {
                if (visited[vertex] == false)
                {
                    curr_min_vertex = std::make_pair(vertex, distance);
                }
            }
        }

        unvisited.erase(curr_min_vertex.first);
        visited[curr_min_vertex.first] = true;

        curr_vertex = vertexes_copy[curr_min_vertex.first];
        curr_min_vertex = std::make_pair('!', INF);
    }

    std::cout << "Prim's Minimum Spanning Tree:" << std::endl;
    std::cout << "Path" << "      " << "Weight" << std::endl;

    for (auto [vertex, distance] : mst)
    {
        std::cout << parent[vertex] << "-->" << vertex << "      " << distance << std::endl;
    }

    std::cout << std::endl;
}

#endif //CODE_MY_CLASS_GRAPH_HPP
