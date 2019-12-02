#ifndef PAGE_RANK_H
#define PAGE_RANK_H

#include <map>
#include "ReadGraphs.h"
#include <cstdlib>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <assert.h>

class PageRank
{

public:

    PageRank();
    PageRank(char * f, float damping, int walk_length);
    ~PageRank();

    void doPageRankEstimate(int threads);

    void setDamping(float d);
    void setWalkLength(int k);
    void setGraph(char * f);

    void RunTests();

private:

    Graph g;
    std::map<int,int> visit_counter;

    void visitNode(int node);
    void TestVisitNode();
    
    int chooseRandomNeighbor(int source_node, double random_number);
    void TestChooseRandomNeighbor();

    float d; //Damping
    int k; //Walk length
    int w; //walks

}

#endif