#include "ReadGraphs.h"

Graph::Graph(){

}

Graph::~Graph(){

}

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

std::vector<int> Graph::getAdjacencyList(int node_name){
    auto it = adjacency_table.find(node_name);
    if(it != adjacency_table.end()){
        return it->second;
    }
    else{
        std::vector<int> v;
        return v;
    }
}



