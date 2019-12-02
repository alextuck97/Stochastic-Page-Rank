#ifndef READ_GRAPHS_H
#define READ_GRAPHS_H

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <map>


class Graph
{
public:
    Graph();
    ~Graph();

    void readGraph(char * f);

    std::vector<int> getAdjacencyList(int node_name);

    void printGraph();

    int getNumNodes();

    void clearTable();

    std::vector<int> getNodes();

private:
    //std::vector<Node> adjacency_list;
    std::map<int,std::vector<int> > adjacency_table;
};




#endif