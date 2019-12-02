#include "PageRank.h"


PageRank::PageRank(){

}

PageRank::~PageRank(){

}

PageRank::PageRank(char * f, float damping, int path_length){
    this->d = damping;
    this->k = path_length;

    this->g.readGraph(f);
    this->w = g.getNumNodes();
}


void PageRank::doPageRankEstimate(int threads){
    
    std::vector<int> nodes = g.getKeys();
    omp_set_num_threads(threads);

    int n = nodes.size();//How many random walks we will do
    
    #pragma omp for schedule(dynamic) shared(n, nodes, visit_counter)
    for(int i = 0; i < n; i++)
    {
        int source = nodes.at(i);

        //Check for existence of source in the map. Either add it to map or increment its visit count.
        #pragma  omp critical
        visitNode(source);

        //Random walk of length k
        for(int j = 0; j < this->k; j++)
        {
            //Generate random number for damping
            float r = rand();
            if(r < this->d)
            {
                //Randomly jump to another node
                source = nodes.at(int(rand() % g.getNumNodes()));//Go to the node at the random index
                
                #pragma omp critical
                visitNode(source);
            }
            else{
                std::vector<int> adj = g.getAdjacencyList(source);
                source = adj.at(int(rand() * adj.size()));//Go to the node at the random index
                #pragma omp critical
                visitNode(source);
            }
        }
    }
//TO DO: 1) make random numbers thread safe
//       2) verify all Graph functions are thread safe
//       3) get omp syntax right
}


void PageRank::setDamping(float d){
    this->d = d;
}

void PageRank::setWalkLength(int k){
    this->k = k;
}

void PageRank::setGraph(char * f){
    this->g.clearTable();
    this->g.readGraph(f);
}

void PageRank::visitNode(int node){
    auto found = visit_counter.find(node);
    if(found == visit_counter.end())
    {
        visit_counter.insert({node,1});
    }
    else{
        found->second++;
    }
}