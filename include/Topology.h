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
      Topology(const unsigned int,
        const NetworkType type = NetworkType::PART_CONN_GRAPH);
      Topology(const unsigned int, const std::vector<DevID>&,
        const NetworkType type = NetworkType::PART_CONN_GRAPH);
      Topology(const unsigned int, const Topology&);
      Topology(const unsigned int, const Topology&, const NetworkType);

      NetworkType getNetworkType() const { return NETWORK_TYPE; }
      void addDevice(const DevID);
      void addDevice(const std::vector<DevID>&);
      void removeDevice(const DevID);
      void removeDevice(const std::vector<DevID>&);
      void setLink(const DevID, const DevID, Link&);
      void unsetLink(const DevID, const DevID);
      bool linkExists(const DevID, const DevID) const;
      bool routeExists(const DevID, const DevID) const;
      std::vector<DevID> getMostDirectRoute(const DevID, const DevID) const;
      std::vector<DevID> getLowestLatencyRoute(const DevID, const DevID);
      std::vector<DevID> getHighestBWRoute(const DevID, const DevID);
  };
}

#endif
