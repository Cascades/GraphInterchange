#include <algorithm>
#include <cassert>
#include <iostream>

#include <graph_interchange/graph_algos/traversals.h>

#include <graph_impls/adjacency_list_graph.h>
#include <graph_impls/edge_set_graph.h>

template<typename GraphT, typename NodeT, typename NodeItr, typename ConnItr>
void runTest(const gic::NodeFuncs<GraphT, NodeT, NodeItr, ConnItr>& funcs, std::string_view name)
{
  std::cout << "Testing: " << name << std::endl;
  GraphT ng = funcs.createGraph();

  NodeT* node0 = funcs.addNode(&ng, "Node 0");
  NodeT* node1 = funcs.addNode(&ng, "Node 1");
  NodeT* node2 = funcs.addNode(&ng, "Node 2");

  NodeT* node3 = funcs.addNode(&ng, "Node 3");
  NodeT* node4 = funcs.addNode(&ng, "Node 4");
  NodeT* node5 = funcs.addNode(&ng, "Node 5");

  funcs.connect(node0, node1);
  funcs.connect(node1, node2);

  funcs.connect(node3, node4);
  funcs.connect(node4, node5);

  {
    std::vector<const NodeT*> traversedOrder;
    traversedOrder.reserve(6);
    gic::alg::DepthFirstPre(&funcs, &ng, [&traversedOrder](const NodeT* n) { traversedOrder.emplace_back(n); });
    const std::vector<const NodeT*> expectedPreOrder{ node0, node1, node2, node3, node4, node5 };
    assert(std::equal(traversedOrder.cbegin(), traversedOrder.cend(), expectedPreOrder.cbegin(), expectedPreOrder.cend()));
  }

  {
    std::vector<const NodeT*> traversedOrder;
    traversedOrder.reserve(6);
    gic::alg::DepthFirstPost(&funcs, &ng, [&traversedOrder](const NodeT* n) { traversedOrder.emplace_back(n); });
    const std::vector<const NodeT*> expectedPostOrder{ node2, node1, node0, node5, node4, node3 };
    assert(std::equal(traversedOrder.cbegin(), traversedOrder.cend(), expectedPostOrder.cbegin(), expectedPostOrder.cend()));
  }

  std::cout << "|_ Passed!" << std::endl;
}

int main()
{
  runTest(AdjacencyListGraph::GetFunctions(), AdjacencyListGraph::getGraphTypeName());
  runTest(EdgeSetGraph::GetFunctions(), EdgeSetGraph::getGraphTypeName());

  std::cout << "All tests passed!" << std::endl;

  return 0;
}
