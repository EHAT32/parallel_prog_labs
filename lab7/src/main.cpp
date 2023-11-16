#include <iostream>
#include <oneapi/tbb/flow_graph.h>
#include <string>
#include <tbb/tbb.h>
#include <tuple>

void createGraph(tbb::flow::graph& graph){
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
    tbb::flow::function_node<int> finalNode(graph, tbb::flow::unlimited,
    [](int) {std::cout << "Nodes finished working" << std::endl;});
    //finaEdge
    tbb::flow::make_edge(joinNode, finalNode);
}

// void createGraph(tbb::flow::graph& g){
//     // Function node that prints "Hello"
//     tbb::flow::function_node<int> hello_node(g, tbb::flow::unlimited, [](int) {
//         std::cout << "Hello" << std::endl;
//     });

//     // Function node that prints "World"
//     tbb::flow::function_node<int> world_node(g, tbb::flow::unlimited, [](int) {
//         std::cout << "World" << std::endl;
//     });

//     // Join node
//     tbb::flow::join_node<std::tuple<int, int>> join(g);

//     // Output edges from hello_node and world_node to the join node
//     tbb::flow::make_edge(hello_node, tbb::flow::input_port<0>(join));
//     tbb::flow::make_edge(world_node, tbb::flow::input_port<1>(join));

//     // Function node that prints "Goodbye"
//     tbb::flow::function_node<std::tuple<int, int>> goodbye_node(g, tbb::flow::unlimited, [](const std::tuple<int, int>&) {
//         std::cout << "Goodbye" << std::endl;
//     });

//     // Output edge from the join node to the goodbye_node
//     tbb::flow::make_edge(join, goodbye_node);

// }

int main(){
    tbb::flow::graph g;
    g.wait_for_all();
    return 0;
}