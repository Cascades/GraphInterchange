#pragma once

#include <list>
#include <ostream>
#include <string>
#include <string_view>

#include <graph_interchange/graph_base.h>

namespace EdgeSetGraph
{
  class NodeGraph;

  class Node
  {
  public:
    Node(std::string_view id, NodeGraph* ng) : id(id), ng(ng) {}

    friend std::ostream& operator<<(std::ostream& os, Node const& node) {
      return os << node.id;
    }

    NodeGraph* getNodegraph() const { return ng; }

  private:
    std::string id;
    NodeGraph* ng;
  };

  class ConnectionItr1
  {
  public:
    ConnectionItr1(const std::list<std::pair<Node*, Node*>>& conns, const Node* node) :
      beginItr(conns.cbegin(), conns.cend(), node),
      endItr(conns.cend(), conns.cend(), node) {
    }

    struct InternalItr {
      InternalItr(std::list<std::pair<Node*, Node*>>::const_iterator itr, std::list<std::pair<Node*, Node*>>::const_iterator end, const Node* node) :
        itr(itr),
        end(end),
        node(node) {
      }

      std::list<std::pair<Node*, Node*>>::const_iterator itr;
      std::list<std::pair<Node*, Node*>>::const_iterator end;
      const Node* node;

      InternalItr& operator++() {
        do {
          ++itr;
        } while (itr != end && itr->first != node && itr->second != node);
        return (*this);
      }

      bool operator!=(const InternalItr& other) {
        return itr != other.itr;
      }

      Node* const& operator*() const {
        if (itr->first == node) {
          return itr->second;
        }
        else {
          return itr->first;
        }
      }
    };

    InternalItr begin() { return beginItr; }
    InternalItr end() { return endItr; }

  private:
    InternalItr beginItr;
    InternalItr endItr;
  };

  class NodeGraph
  {
  public:
    Node* addNode(std::string_view id) {
      return &nodes.emplace_back(id, this);
    }

    void connect(Node* node0, Node* node1) {
      edges.emplace_back(node0, node1);
    }

    void disconect(Node* node0, Node* node1) {
      edges.remove_if(
        [node0, node1](const EdgeT& edge)
        { return (edge.first == node0 && edge.second == node1) ||
                 (edge.first == node1 && edge.second == node0); });
    }

    void disconectAll(Node* node) {
      edges.remove_if([node](const EdgeT& edge) { return edge.first == node || edge.second == node; });
    }

    void remove(Node* node) {
      disconectAll(node);
      nodes.remove_if([node](const Node n) { return &n == node; });
    }

    const std::list<Node>& getNodes() const { return nodes; }

    ConnectionItr1 getConnections(const Node* node) const {
      return ConnectionItr1{ edges, node };
    }

    size_t getNumConnections(const Node* node) const {
      size_t numConnections = 0;
      for (const auto& edge : edges) {
        if (edge.first == node || edge.second == node) {
          ++numConnections;
        }
      }
      return numConnections;
    }

    bool hasConnections(const Node* node) const {
      for (const auto& edge : edges) {
        if (edge.first == node || edge.second == node) {
          return true;
        }
      }
      return false;
    }

  private:
    using EdgeT = std::pair<Node*, Node*>;

    std::list<Node> nodes;
    std::list<EdgeT> edges;
  };

  NodeGraph createGraph() {
    return NodeGraph{};
  }

  std::string getGraphTypeName() {
    return "EdgeSetGraph";
  }

  inline gic::NodeFuncs<NodeGraph, Node, const std::list<Node>&, ConnectionItr1> GetFunctions() {
    return gic::NodeFuncs<NodeGraph, Node, const std::list<Node>&, ConnectionItr1>
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
}
