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
    
    std::vector<int> nodes = g.getNodes();
    omp_set_num_threads(threads);

    int n = nodes.size();//How many random walks we will do
    
    #pragma omp parallel for schedule(dynamic) shared(n, nodes)
    for(int i = 0; i < n; i++)
    {
        int source = nodes.at(i);
        
        double random_number;
        struct drand48_data drand_buffer;

        // Seed a new random number generator for each walk
        int seed = omp_get_thread_num() + i + time(NULL);
        srand48_r(seed, &drand_buffer);
        
        //Check for existence of source in the map. Either add it to map or increment its visit count.
        #pragma  omp critical
        visitNode(source);

        //Random walk of length k
        for(int j = 0; j < this->k; j++)
        {
            //Generate random number for damping
            drand48_r(&drand_buffer, &random_number);
            if(random_number < this->d)
            {
                //Randomly jump to another node
                drand48_r(&drand_buffer, &random_number);
                source = nodes.at(int(random_number * g.getNumNodes()));//Go to the node at the random index
            }
            else{
                drand48_r(&drand_buffer, &random_number);
                source = chooseRandomNeighbor(source, random_number);
            }

            #pragma omp critical
                visitNode(source);
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


int PageRank::chooseRandomNeighbor(int source_node, double random_number){
    std::vector<int> adj = g.getAdjacencyList(source_node);

    int walk_to_index = int(random_number * (adj.size() + 1));

    if(walk_to_index < adj.size()){
        source_node = adj.at(walk_to_index);//Go to the node at the random index
    } // If source is not updated, then we stay on current node

    return source_node;
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


void PageRank::RunTests(){
    this->TestVisitNode();
}

void PageRank::TestVisitNode(){
    this->visitNode(4);
    this->visitNode(2);
    this->visitNode(1);
    this->visitNode(1);
    this->visitNode(50);

    assert(visit_counter.find(4)->second == 1);
    assert(visit_counter.find(2)->second == 1);
    assert(visit_counter.find(2)->second == 2);
    assert(visit_counter.find(50) == visit_counter.end());
}

void PageRank::TestChooseRandomNeighbor(){
    
    struct drand48_data drand_buffer;
    int seed = time(NULL);
    srand48_r(seed, &drand_buffer);

    double random_number;

    int source_node = 0;

    for(int i = 0; i < 100; i++)
    {
        if (i % 10 == 0)//Increment source from 0 to 9
        {
            std::cout << std::endl;
            source_node = i / 10;
            std::cout << "Source " << source_node << " : ";
        }

        drand48_r(&drand_buffer, &random_number);
        
        //Visually inspect the output to check if its random
        std::cout << this->chooseRandomNeighbor(source_node, random_number) << " ";
        
    }
    
}