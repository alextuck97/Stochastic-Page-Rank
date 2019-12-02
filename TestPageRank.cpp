#include "PageRank.h"



int main(int argc,char *argv[])
{
    char * f = argv[1]; 
    
    float d = atof(argv[2]);
    int walk_length = atoi(argv[3]);   
    
    PageRank pagerank{f,d,walk_length};
    pagerank.RunTests();

    return 0;
}