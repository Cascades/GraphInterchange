#pragma once

#include <string_view>

namespace gic
{
template <typename GraphT, typename NodeT, typename NodeItr, typename ConnItr>
struct NodeFuncs
{
  GraphT  (*createGraph)          ();
  NodeT*  (*addNode)              (GraphT*, std::string_view id);
  void    (*removeNode)           (GraphT*, NodeT* node);
  NodeItr (*getNodes)             (const GraphT*);
  ConnItr (*getInConnections)     (const NodeT* node);
  ConnItr (*getOutConnections)    (const NodeT* node);
  ConnItr (*getConnections)       (const NodeT* node);
  size_t  (*getNumInConnections)  (const NodeT* node);
  size_t  (*getNumOutConnections) (const NodeT* node);
  size_t  (*getNumConnections)    (const NodeT* node);
  bool    (*hasInConnections)     (const NodeT* node);
  bool    (*hasOutConnections)    (const NodeT* node);
  bool    (*hasConnections)       (const NodeT* node);
  void    (*connect)              (NodeT* node0, NodeT* node1);
};
}
