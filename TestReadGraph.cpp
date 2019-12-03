#include "ReadGraphs.h"

// Basic test to ensure grpahs are
// properly read. Outputs the adjacency list
// of each node. Run it with "sample.txt" 
// for easy inspection.
int main(int argc,char *argv[])
{
    char * f = argv[1];    
    Graph g;
    g.readGraph(f);
    g.printGraph();

    return 0;
}