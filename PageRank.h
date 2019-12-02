#ifndef PAGE_RANK_H
#define PAGE_RANK_H

#include <map>
#include "ReadGraphs.h"
#include <cstdlib>
#include <omp.h>

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

private:

    Graph g;
    std::map<int,int> visit_counter;

    void visitNode(int node);

    float d; //Damping
    int k; //Walk length
    int w; //walks

}





#endif