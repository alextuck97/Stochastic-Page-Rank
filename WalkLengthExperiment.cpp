#include "PageRank.h"

/*********
 * Time how long it takes to do PageRank on a graph given the arguments
 * Argument 1: The graph file
 *          2: Number of threads
 *          3: Damping factor
 *          4: Walk length
 * Output: prints the walk length, damping factor, and time it took to run
 * *******/
int main(int argc,char *argv[])
{

    char * f = argv[1]; 
    int threads = atoi(argv[2]);
    std::cout << "Input threads " << threads << std::endl;
    float d = atof(argv[3]);
    int w = atoi(argv[4]);
    
    PageRank p = PageRank(f, d, w);

    double t1 = omp_get_wtime();
    p.doPageRankEstimate(threads);
    double t2 = omp_get_wtime();

    std::cout << "K:" << w << " d:" << d << " T:" << t2 - t1;
    std::cout << std::endl;

    //std::vector<std::pair<int,int>> topK = p.getTopKPages(5);

    //for(int i = 0; i < 5; i++){
    //    std::cout << topK.at(i).first << " Visits: " << topK.at(i).second << std::endl;
    //}
    std::cout << std::endl;

}