#include <iostream>
#include <oneapi/tbb/flow_graph.h>
#include <string>
#include <tbb/tbb.h>
#include <tuple>
#include <utility>

void createGraph(){
    tbb::flow::graph graph;
    //upperNode
    tbb::flow::function_node<int, int> upperNode(graph, tbb::flow::unlimited,
    [](const int& n) -> int {std::cout << "Upper node received " << n << std::endl;
    return 2 * n;});
    //lowerNode
    tbb::flow::function_node<int, int> lowerNode(graph, tbb::flow::unlimited,
    [](const int& n) -> int {std::cout << "Lower node received " << n << std::endl;
    return 4 * n;});
    //joinNode
    tbb::flow::join_node<std::tuple<int, int>, tbb::flow::queueing> joinNode(graph);
    //edges
    tbb::flow::make_edge(upperNode, tbb::flow::input_port<0>(joinNode));
    tbb::flow::make_edge(lowerNode, tbb::flow::input_port<1>(joinNode));
    //finalNode
    tbb::flow::function_node<std::tuple<int, int>, int> finalNode(graph, tbb::flow::unlimited,
    [](const std::tuple<int, int>& tup) {
        auto [f, s] = tup; //c++ 17
        std::cout << "Nodes finished working: 2 * n1 + 4 * n2 = " << f + s << std::endl; return 0;
    });
    //finalEdge
    tbb::flow::make_edge(joinNode, finalNode);
    upperNode.try_put(1);
    lowerNode.try_put(2);
    graph.wait_for_all();
}
int main(){
    createGraph();
    return 0;
}