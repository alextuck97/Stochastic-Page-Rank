#ifndef PAGE_RANK_H
#define PAGE_RANK_H

#include <unordered_map>
#include "ReadGraphs.h"
#include <cstdlib>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <assert.h>
#include <climits>

class PageRank
{

public:

    PageRank();
    PageRank(char * f, float damping, int walk_length);
    ~PageRank();

    void doPageRankEstimate(int threads, int n = 0);

    void setDamping(float d);
    void setWalkLength(int k);
    void setGraph(char * f);

    std::vector<std::pair<int,int>> getTopKPages(int k);
    
    void RunTests();

private:

    Graph g;

    void TestChooseRandomNeighbor();
    void TestTopKPages();
    void TestThreadRandomness(int threads, int n);

    float d; //Damping
    int k; //Walk length

    int * visits;

};

#endif