# Graph Interchange - A library for bringing together graph representations

The aim of this library is to allow any node graph and any algorithm which operates on node graphs to pass through a simple unified interchange. This should allow a corpus of node graph algorithms to be built, agnostic of implementation, and used on any node graph implementation with ease.

## Usage

### Node Graph Implementers

Write your graph implementation however you please. Then just tell the interchange how to use it. Like so:

```
NodeGraph createGraph() {
  return NodeGraph{};
}

std::string getGraphTypeName() {
  return "EdgeSetGraph";
}

inline gic::NodeFuncs<Graph, Node, const std::list<Node>&, ConnItr> GetFunctions() {
  return gic::NodeFuncs<Graph, Node, const std::list<Node>&, ConnItr>
  {
    .createGraph          = &createGraph,
    .addNode              = [](NodeGraph* ng, std::string_view id) { return ng->addNode(id); },
    .removeNode           = [](NodeGraph* ng, Node* node) { ng->remove(node); },
    .getNodes             = [](const NodeGraph* ng) -> const std::list<Node>&{ return ng->getNodes(); },
    .getInConnections     = [](const Node* n) -> ConnectionItr1 { return n->getNodegraph()->getConnections(n); },
    .getOutConnections    = [](const Node* n) -> ConnectionItr1 { return n->getNodegraph()->getConnections(n); },
    .getConnections       = [](const Node* n) -> ConnectionItr1 { return n->getNodegraph()->getConnections(n); },
    .getNumInConnections  = [](const Node* n) -> size_t { return n->getNodegraph()->getNumConnections(n); },
    .getNumOutConnections = [](const Node* n) -> size_t { return n->getNodegraph()->getNumConnections(n); },
    .getNumConnections    = [](const Node* n) -> size_t { return n->getNodegraph()->getNumConnections(n); },
    .hasInConnections     = [](const Node* n) -> bool { return n->getNodegraph()->hasConnections(n); },
    .hasOutConnections    = [](const Node* n) -> bool { return n->getNodegraph()->hasConnections(n); },
    .hasConnections       = [](const Node* n) -> bool { return n->getNodegraph()->hasConnections(n); },
    .connect              = [](Node* n0, Node* n1) { n0->getNodegraph()->connect(n0, n1); },
  };
}
```

### Node Graph Algorithm Implementers

Template your function and include a `NodeFuncs` object and `GraphT` object in the parameters. Like so:

```
template <typename GraphT, typename NodeT, typename ConnItr, typename NodeItr>
inline void PrintNodes(
  const NodeFuncs<GraphT, NodeT, ConnItr, NodeItr>* funcs,
  const GraphT* graph) {

  for (const NodeT& node : funcs->getNodes(graph)) {
    std::cout << node << std::endl;
  }
}
```
