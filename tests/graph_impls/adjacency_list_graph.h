#pragma once

#include <list>
#include <ostream>
#include <string>
#include <string_view>

#include <graph_interchange/graph_base.h>

namespace AdjacencyListGraph
{
  class Node {
  public:
    Node(std::string_view id) : connections(), id(id) {}

    void connect(Node* node) {
      connections.emplace_back(node);
      node->connections.emplace_back(this);
    }

    void disconnect(Node* node) {
      node->connections.remove_if([this](const Node* n) { return n == this; });
      connections.remove_if([node](const Node* n) { return n == node; });
    }

    void disconnectAll() {
      for (Node* node : connections) {
        node->connections.remove_if([this](const Node* n) { return n == this; });
      }
      connections.clear();
    }

    const std::list<Node*>& getConnections() const { return connections; }

    friend std::ostream& operator<<(std::ostream& os, Node const& node) {
      return os << node.id;
    }

  private:
    std::list<Node*> connections;
    std::string id;
  };

  class NodeGraph0 {
  public:
    Node* addNode(std::string_view id) {
      return &nodes.emplace_back(id);
    }

    void connect(Node* node0, Node* node1) {
      node0->connect(node1);
    }

    void disconnect(Node* node0, Node* node1) {
      node0->disconnect(node1);
    }

    void disconnectAll(Node* node) {
      node->disconnectAll();
    }

    void remove(Node* node) {
      node->disconnectAll();
      nodes.remove_if([node](const Node n) { return &n == node; });
    }

    const std::list<Node>& getNodes() const { return nodes; }
    const std::list<Node*>& getConnections(Node* node) const { return node->getConnections(); }

  private:
    std::list<Node> nodes;
  };

  NodeGraph0 createGraph() {
    return NodeGraph0{};
  }

  std::string getGraphTypeName() {
    return "AdjacencyListGraph";
  }

  inline gic::NodeFuncs<NodeGraph0, Node, const std::list<Node>&, const std::list<Node*>&> GetFunctions() {
    return gic::NodeFuncs<NodeGraph0, Node, const std::list<Node>&, const std::list<Node*>&>
    {
      .createGraph          = &createGraph,
      .addNode              = [](NodeGraph0* ng, std::string_view id) { return ng->addNode(id); },
      .removeNode           = [](NodeGraph0* ng, Node* node) { ng->remove(node); },
      .getNodes             = [](const NodeGraph0* ng) -> const std::list<Node>& { return ng->getNodes(); },
      .getInConnections     = [](const Node* n) -> const std::list<Node*>& { return n->getConnections(); },
      .getOutConnections    = [](const Node* n) -> const std::list<Node*>& { return n->getConnections(); },
      .getConnections       = [](const Node* n) -> const std::list<Node*>& { return n->getConnections(); },
      .getNumInConnections  = [](const Node* n) -> size_t { return n->getConnections().size(); },
      .getNumOutConnections = [](const Node* n) -> size_t { return n->getConnections().size(); },
      .getNumConnections    = [](const Node* n) -> size_t { return n->getConnections().size(); },
      .hasInConnections     = [](const Node* n) -> bool { return !n->getConnections().empty(); },
      .hasOutConnections    = [](const Node* n) -> bool { return !n->getConnections().empty(); },
      .hasConnections       = [](const Node* n) -> bool { return !n->getConnections().empty(); },
      .connect              = [](Node* n0, Node* n1) { n0->connect(n1); },
    };
  }
}
