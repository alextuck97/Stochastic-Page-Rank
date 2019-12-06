#include "PageRank.h"

/*********
 * Argument 1: The graph file
 *          2: Number of threads
 *          3: Damping factor
 * *******/
int main(int argc,char *argv[])
{

    char * f = argv[1]; 
    int threads = atoi(argv[2]);
    std::cout << "Input threads " << threads << std::endl;
    float d = atof(argv[3]);
    int w = atoi(argv[4]);
    int walk_lengths[10] = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, 512000};

   

    PageRank p = PageRank(f, d, 0);

    //for(int i = 0; i < 10; i++){
    p.setWalkLength(w);
    double t1 = omp_get_wtime();
    p.doPageRankEstimate(threads);
    double t2 = omp_get_wtime();
    std::cout << t2 - t1;
    //}
    std::cout << std::endl;

    std::vector<std::pair<int,int>> topK = p.getTopKPages(5);

    std::cout << "\nTesting get top K pages \n";
    for(int i = 0; i < 5; i++){
        std::cout << topK.at(i).first << " Visits: " << topK.at(i).second << std::endl;
    }

}