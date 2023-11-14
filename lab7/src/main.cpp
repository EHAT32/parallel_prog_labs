#include <iostream>
#include <oneapi/tbb/flow_graph.h>
#include <string>
#include <tbb/tbb.h>

void createGraph(tbb::flow::graph& graph){
    //nodes
    tbb::flow::function_node<int, std::string> upperNode(graph, tbb::flow::unlimited,
    [](const int& n) -> std::string {std::cout << "Upper node received " << n << std::endl;
    return std::to_string(n);});

    tbb::flow::function_node<int, std::string> lowerNode(graph, tbb::flow::unlimited,
    [](const int& n) -> std::string {std::cout << "Lower node received " << n << std::endl;
    return std::to_string(n);});
}

int main(){
    tbb::flow::graph g;

    return 0;
}