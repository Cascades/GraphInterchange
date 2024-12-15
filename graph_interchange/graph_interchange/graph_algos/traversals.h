#pragma once

#include <functional>
#include <iostream>
#include <stack>
#include <unordered_map>

#include "graph_interchange/graph_base.h"

namespace gic
{
namespace alg
{
  template <typename GraphT, typename NodeT, typename ConnItr, typename NodeItr, typename OpT>
  inline void DepthFirstPre(
    const NodeFuncs<GraphT, NodeT, ConnItr, NodeItr>* funcs,
    const GraphT* graph,
    OpT&& op) {

    std::unordered_map<const NodeT*, bool> visited;

    for (const NodeT& node : funcs->getNodes(graph)) {
      visited[&node] = false;
    }

    for (const NodeT& srcNode : funcs->getNodes(graph)) {
      if (visited[&srcNode]) {
        continue;
      }

      std::stack<const NodeT*> stack;
      stack.push(&srcNode);

      while (!stack.empty())
      {
        const NodeT* currNode = stack.top();
        stack.pop();

        if (!visited[currNode])
        {
          op(currNode);
          visited[currNode] = true;
        }

        for (NodeT* connectedNode : funcs->getConnections(currNode)) {
          if (!visited[connectedNode]) {
            stack.push(connectedNode);
          }
        }
      }
    }
  }

  template <typename GraphT, typename NodeT, typename ConnItr, typename NodeItr, typename OpT>
  inline void DepthFirstPost(
    const NodeFuncs<GraphT, NodeT, ConnItr, NodeItr>* funcs,
    const GraphT* graph,
    OpT&& op) {

    std::unordered_map<const NodeT*, bool> visited;

    for (const NodeT& node : funcs->getNodes(graph)) {
      visited[&node] = false;
    }

    for (const NodeT& srcNode : funcs->getNodes(graph)) {
      if (visited[&srcNode]) {
        continue;
      }

      std::stack<const NodeT*> stack;
      stack.push(&srcNode);

      while (!stack.empty())
      {
        const NodeT* currNode = stack.top();

        while (true) {
          visited[currNode] = true;

          bool traversalOccured = false;
          for (NodeT* connectedNode : funcs->getConnections(currNode)) {
            if (!visited[connectedNode]) {
              stack.push(connectedNode);
              traversalOccured = true;
            }
          }

          if (!traversalOccured) {
            break;
          }

          currNode = stack.top();
        }

        stack.pop();
        op(currNode);
      }
    }
  }
}
}
