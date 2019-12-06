#include "ReadGraphs.h"

Graph::Graph(){

}

Graph::~Graph(){

}

//Need to modify for use with set
void Graph::readGraph(char * f){
    std::ifstream in_file;
    in_file.open(f);

    std::string str;
    if(in_file.is_open())
    {
        while(!in_file.eof())
        {
            std::getline(in_file,str);

            if(str.compare(0,1,"#") != 0 && str.compare(0,1,"\0") != 0)
            {   
                std::size_t tab = str.find("\t");
                
                if(tab == std::string::npos){//Handle case of facebook graph
                    tab = str.find(" ");
                }

                std::size_t nl = str.find("\n");
                std::string source = str.substr(0,tab);
                std::string dest = str.substr(tab+1,tab+1 - nl);
                //std::cout << "String: "<< source << " " << dest << std::endl;
                int s = stoi(source);
                int d = stoi(dest);
                auto itr = this->adjacency_table.find(s);
                
                if(itr != this->adjacency_table.end())
                {
                    itr->second.push_back(d);
                }
                else
                {
                    std::vector<int> v;
                    v.push_back(d);
                    this->adjacency_table.insert({s, v});
                } 
            }
        }
    }
    in_file.close();
}


void Graph::printGraph(){
    for(auto itr = this->adjacency_table.begin(); itr != this->adjacency_table.end(); itr++)
    {
        std::cout << itr->first << " ";
        for(auto vitr = itr->second.begin(); vitr != itr->second.end(); vitr++)
        {
            std::cout << *vitr << " ";
        }
        std::cout << std::endl;
    }
}


void Graph::clearTable(){
    this->adjacency_table.clear();
}

int Graph::getNumNodes(){
    return this->adjacency_table.size();
}

std::vector<int> Graph::getNodes(){
    std::vector<int> v;
    for(auto it = this->adjacency_table.begin(); it != this->adjacency_table.end(); it++)
    {
        v.push_back(it->first);
    }
    return v;
}


int Graph::getLargestNode(){
    int largest = 0;
    for(auto it = this->adjacency_table.begin(); it != this->adjacency_table.end(); it++)
    {
        if(it->first > largest){
            largest = it->first;
        }
    }
    return largest;
}


std::vector<int> & Graph::getAdjacencyList(int node_name){
    auto it = adjacency_table.find(node_name);
    if(it != adjacency_table.end()){
        return it->second;
    }
    else{
        std::vector<int> * v = new std::vector<int>;
        return *v;
    }
}

//Modify for use with unordered_set
//Source node included in random edge
int Graph::getRandomEdge(int source, double random){
    auto it = adjacency_table.find(source);
    
    if(it != adjacency_table.end()){
        int walk_to_index = int(random * (it->second.size() + 1));
    
        if(walk_to_index < it->second.size()){
            return it->second.at(walk_to_index);
        }
        else{
            return source;
        }
    }
    else{// Node is a dead end, return itself
        return source;
    }
}



