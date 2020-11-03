//#include "my_class_graph.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include <exception>

class InvalidFileName : public std::exception
{
public:
    const char *what() const noexcept { return "Invalid filename !"; }
};

int main()
{
    std::ifstream f_in("graph_01.txt");

    if (f_in.fail())
    {
        throw InvalidFileName();
    }

    std::vector<std::string> vertexes;
    std::vector<char> adjacents;
    std::vector<int> distances;

    std::string str_vertex;
    std::string str_adjacent;
    std::string str_distance;
    std::stringstream ss;
    std::stringstream ss2;

    while (!f_in.eof())
    {
        std::getline(f_in, str_vertex);
        std::getline(f_in, str_adjacent);
        std::getline(f_in, str_distance);

        vertexes.push_back(str_vertex);

        ss << str_adjacent;
        ss2 << str_distance;

        for (char ch_adj; ss >> ch_adj;)
        {
            int adj_dis;
            ss2 >> adj_dis;

            adjacents.push_back(ch_adj);
            distances.push_back(adj_dis);

            if (ss.peek() == ',' && ss2.peek() == ',')
            {
                ss.ignore();
                ss2.ignore();
            }
        }

        ss.clear();
        ss2.clear();
    }

//    for (std::string str : vertexes)
//    {
//        std::cout << str << std::endl;
//    }

    for (char str : adjacents)
    {
        std::cout << str << std::endl;
    }

//    for (int str : distances)
//    {
//        std::cout << str << std::endl;
//    }

    return 0;
}
