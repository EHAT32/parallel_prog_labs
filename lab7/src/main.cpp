#include <iostream>
#include <oneapi/tbb/flow_graph.h>
#include <string>
#include <tbb/tbb.h>
#include <tuple>

void createGraph(tbb::flow::graph& graph){
    //upperNode
    tbb::flow::function_node<int, int> upperNode(graph, tbb::flow::unlimited,
    [](const int& n) -> int {std::cout << "Upper node received " << n << std::endl;
    return n;});
    //lowerNode
    tbb::flow::function_node<int, int> lowerNode(graph, tbb::flow::unlimited,
    [](const int& n) -> int {std::cout << "Lower node received " << n << std::endl;
    return n;});
    //joinNode
    tbb::flow::join_node<std::tuple<int, int>, tbb::flow::queueing> joinNode(graph);
    //edges
    tbb::flow::make_edge(upperNode, tbb::flow::input_port<0>(joinNode));
    tbb::flow::make_edge(lowerNode, tbb::flow::input_port<1>(joinNode));
    
}

int main(){
    tbb::flow::graph g;

    return 0;
}