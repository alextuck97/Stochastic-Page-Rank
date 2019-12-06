#include "PageRank.h"

/*********
 * Argument 1: The graph file
 *          2: Damping factor
 *          3: Walk length
 * *******/
int main(int argc,char *argv[])
{
    char * f = argv[1]; 
    float d = atof(argv[2]);
    int w = atoi(argv[3]);

    PageRank p = PageRank(f,d,w);

    p.doPageRankEstimate(8);
    std::cout << "K:" << w << " d:" << d << std::endl;
    std::vector<std::pair<int,int>> topK = p.getTopKPages(5);

    for(int i = 0; i < 5; i++){
        std::cout << topK.at(i).first << " Visits: " << topK.at(i).second << std::endl;
    }
    std::cout << std::endl;

    

}