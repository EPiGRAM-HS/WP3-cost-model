#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <vector>
#include <tuple>
#include <unordered_map>
#include "typedefs.h"
#include "lemon/list_graph.h"
#include "Link.h"

namespace CostModel {
  class Topology {
    private:
      const NetworkType NETWORK_TYPE;
      // ListGraph type from Lemon -- topo_graph describes the network,
      // topo_devs maps nodes to devices, topo_links maps edges to Links
      lemon::ListGraph topo_graph;
      lemon::ListGraph::NodeMap<DevID> topo_devs;
      std::unordered_map<DevID, lemon::ListGraph::Node> topo_nodes;
      lemon::ListGraph::EdgeMap<Link> topo_links;
      std::unordered_map<LinkID, lemon::ListGraph::Edge> topo_edges;

      void reserveEdge(unsigned int, NetworkType);

    public:
      Topology(unsigned int, NetworkType);
      Topology(unsigned int, std::vector<DevID>, NetworkType);
      Topology(Topology&) = delete;

      NetworkType getNetworkType() { return NETWORK_TYPE; }
      void setLink(const DevID, const DevID, Link&);
      void unsetLink(const DevID, const DevID);
      bool linkExists(const DevID, const DevID);
      bool routeExists(const DevID, const DevID);
      std::vector<DevID> getMostDirectRoute(const DevID, const DevID);
      std::vector<DevID> getLowestLatencyRoute(const DevID, const DevID);
      std::vector<DevID> getHighestBWRoute(const DevID, const DevID);
  };
}

#endif
