#include <vector>
#include <tuple>
#include "Topology.h"

namespace CostModel {
  void Topology::reserveEdge(unsigned int num_devices, NetworkType type) {
    switch (type) {
      case NetworkType::PART_CONN_GRAPH: // nothing we can do!
        break;
      case NetworkType::FULL_CONN_GRAPH:
        topo_graph.reserveEdge((num_devices * (num_devices-1)) / 2);
        break;
      case NetworkType::STAR:
        topo_graph.reserveEdge(num_devices - 1);
        break;
      case NetworkType::RING:
        topo_graph.reserveEdge(num_devices);
        break;
      case NetworkType::CART: // nothing we can do!
        break;
    }
    return;
  }

  Topology::Topology(unsigned int num_devices,
    NetworkType type = NetworkType::PART_CONN_GRAPH)
    : NETWORK_TYPE(type), topo_devs(topo_graph), topo_links(topo_graph) {
    // The node and edge maps should be updated with changes to the underlying
    // graph, but we will test this.
    topo_graph.reserveNode(num_devices);
    topo_nodes.reserve(num_devices);
    reserveEdge(num_devices, type);

    return;
  }

  Topology::Topology(unsigned int num_devices, std::vector<DevID> dev_vec,
    NetworkType type = NetworkType::PART_CONN_GRAPH)
    : NETWORK_TYPE(type), topo_devs(topo_graph), topo_links(topo_graph) {
    topo_graph.reserveNode(num_devices);
    reserveEdge(num_devices, type);

    lemon::ListGraph::Node node;
    for (const DevID& dev_id : dev_vec) {
      node = topo_graph.addNode();
      topo_devs[node] = dev_id;
      topo_nodes.emplace(dev_id, node);
    }

    return;
  }

  void Topology::setLink(const DevID IDA, const DevID IDB, Link& link) {
    const lemon::ListGraph::Node NODE_A = topo_nodes.at(IDA);
    const lemon::ListGraph::Node NODE_B = topo_nodes.at(IDB);

    // add edge to graph and get lemon Edge
    lemon::ListGraph::Edge edge = topo_graph.addEdge(NODE_A, NODE_B);

    // set link ID so we can use it for look-up later
    link.setLinkID(IDA, IDB);

    // add link to EdgeMap
    topo_links[edge] = link;
    // ...and add edge to LinkID map...
    topo_edges.emplace(link.getLinkID(), edge);

    return;
  }

}
