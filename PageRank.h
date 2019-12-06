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
    std::unordered_map<int,int> visit_counter;

    void visitNode(int node);
    
    void lockAndVisit(int node);

    void countWalk(std::vector<int> w);

    int chooseRandomNeighbor(int source_node, double random_number);
    
    void TestVisitNode();
    void TestChooseRandomNeighbor();
    void TestTopKPages();
    void TestThreadRandomness(int threads, int n);

    float d; //Damping
    int k; //Walk length
    int w; //walks

    int * visits;

    omp_lock_t * locks;
    int num_locks;

};

#endif