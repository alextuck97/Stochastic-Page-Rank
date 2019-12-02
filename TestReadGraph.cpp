#include "ReadGraphs.h"

int main(int argc,char *argv[])
{
    char * f = argv[1];    
    Graph g;
    g.readGraph(f);
    g.printGraph();

    return 0;
}