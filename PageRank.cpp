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

    this->visits = new int[this->g.getLargestNode() + 1]();//Plus 1 for the case where the graph starts in node 1
    //this->num_locks = w / 1000;
    //this->locks = new omp_lock_t[w / 1000];
    
}

// Input: n, how many source nodes there are. Default is each node is a source.
void PageRank::doPageRankEstimate(int threads, int n){
    
    std::vector<int> nodes = g.getNodes();
    
    omp_set_num_threads(threads);
    
    //for(int i = 0; i < num_locks; i++){
    //    omp_init_lock(&locks[i]);
    //}

    //Default number of random walks is one for each node in the graph
    if(n == 0)
        n = nodes.size();
    
    #pragma omp parallel for schedule(dynamic, 1000) shared(n, nodes)
    for(int i = 0; i < n; i++)
    {
        int source = nodes.at(i % nodes.size());//Cycle through nodes in case want to walk beyond the total number
        
        double random_number;
        struct drand48_data drand_buffer;

        // Seed a new random number generator for each walk
        int seed = (omp_get_thread_num() + 1) * i + time(NULL);
        srand48_r(seed, &drand_buffer);
        
        //Check for existence of source in the map. Either add it to map or increment its visit count.
        //#pragma  omp critical
        //{
        //omp_set_lock(&locks[source / 1000]);
        //    visitNode(source);
        //omp_unset_lock(&locks[source / 1000]);
        //}
        //Random walk of length k
        //std::vector<int> walk;
        #pragma omp atomic
            visits[source]++;
        //walk.push_back(source);
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
                // = chooseRandomNeighbor(source, random_number);
                source = g.getRandomEdge(source, random_number);
            }
            //walk.push_back(source);
            #pragma omp atomic
                visits[source]++;
            //#pragma omp critical
            //{//omp_set_lock(&locks[source / 1000]);
            //    visitNode(source);
            //}//omp_unset_lock(&locks[source / 1000]);
        }
        //#pragma omp critical
        //{
        //    countWalk(walk);
        //}
    }

    //for(int i = 0; i < num_locks; i++){
    //    omp_destroy_lock(&locks[i]);
    //}
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

// Randomly choose a neighbor of the source node
// Thread safe, does not perform writes.
int PageRank::chooseRandomNeighbor(int source_node, double random_number){
    std::vector<int> adj = g.getAdjacencyList(source_node);

    int walk_to_index = int(random_number * (adj.size() + 1));
    int new_node = source_node;
    if(walk_to_index < adj.size()){
        new_node = adj.at(walk_to_index);//Go to the node at the random index
    } // If source is not updated, then we stay on current node

    return new_node;
}

// Increment a counter for a node. Does not check if the node exists
// in the graph or not. Must be sync'd.
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

//Scan through the nodes, pick the largest and smallest. Initialize an array of zeros with that size.
//Then, only use atomic

void PageRank::countWalk(std::vector<int> w){
    for(auto it = w.begin(); it != w.end(); it++){
        visitNode(*it);
    }
}

void PageRank::lockAndVisit(int node){
    int lock = node / 1000;

}


// Return the k page-visit pairs with the highest visit counts
std::vector<std::pair<int,int>> PageRank::getTopKPages(int k){
    
    std::vector<std::pair<int,int>> topK;

    int range = g.getLargestNode() + 1;
    
    int prev_largest = INT_MAX;
    for(int i = 0; i < k; i++){
        std::pair<int,int> iLargest = std::make_pair(0,0);
        
        
        for(int j = 0; j < range; j++){
            if(visits[j] > iLargest.second && visits[j] < prev_largest)
            {
                iLargest.first = j;
                iLargest.second = visits[j];
            }
        }
        //visit_counter_copy.erase(iLargest.first);
        prev_largest = iLargest.second;
        topK.push_back(iLargest);
    }

    return topK;
}


/****************************************

PageRank class tests

***************************************/
void PageRank::RunTests(){
    this->TestVisitNode();
    this->TestChooseRandomNeighbor();
    this->TestTopKPages();
    //this->TestThreadRandomness(4, 12);
}

// Ensure visit counts are properly updated 
void PageRank::TestVisitNode(){
    this->visitNode(4);
    this->visitNode(2);
    this->visitNode(1);
    this->visitNode(1);
    

    assert(visit_counter.find(4)->second == 1);
    assert(visit_counter.find(2)->second == 1);
    assert(visit_counter.find(1)->second == 2);
    // Node 50 does not exist
    assert(visit_counter.find(50) == visit_counter.end());

    std::cout << "Passed visit node tests \n";
}

// Simulate picking a random neighbor node.
// Visually inspect the output to see if it looks random
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
        //std::cout << this->chooseRandomNeighbor(source_node, random_number) << "  ";// << random_number;  
        std::cout << g.getRandomEdge(source_node, random_number) << "  ";
    }
}


void PageRank::TestTopKPages(){
    omp_set_num_threads(4);
    int walks = 200;

    this->doPageRankEstimate(4);
    std::vector<std::pair<int,int>> topK = this->getTopKPages(5);

    std::cout << "\nTesting get top K pages \n";
    for(int i = 0; i < 5; i++){
        std::cout << topK.at(i).first << " Visits: " << topK.at(i).second << std::endl;
    }
}

// Visually inspect that each thread produces unique values
void PageRank::TestThreadRandomness(int threads, int n){
    omp_set_num_threads(threads);

    //Generate 10 sequences of random numbers
   
    #pragma omp parallel for schedule(dynamic) shared(n)
    for(int i = 0; i < n; i++)
    {
        double random_number;
        struct drand48_data drand_buffer;

        // Seed a new random number generator for each walk
        int seed = (omp_get_thread_num() + 1) * i + time(NULL);
        srand48_r(seed, &drand_buffer);

        double randos[5];
        for(int j = 0; j < 5; j++){
            drand48_r(&drand_buffer, &random_number);
            randos[j] = random_number;
        }
        
        #pragma omp critical
        {
            std::cout << "Thread " << omp_get_thread_num();
            for(int k = 0; k < 5; k++)
            {
                std::cout << " " << randos[k] << " ";
            }
            std::cout << std::endl;
        }
    }
}