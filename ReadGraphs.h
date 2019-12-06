#ifndef READ_GRAPHS_H
#define READ_GRAPHS_H

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>


class Graph
{
public:
    Graph();
    ~Graph();

    void readGraph(char * f);

    std::vector<int> & getAdjacencyList(int node_name);

    void printGraph();

    int getNumNodes();

    void clearTable();

    std::vector<int> getNodes();

    int getLargestNode();
    int getRandomEdge(int source, double random);


private:
    //std::vector<Node> adjacency_list;
    //std::unordered_map<int,std::vector<int> > adjacency_table;
    std::unordered_map<int,std::vector<int> > adjacency_table;
};




#endif