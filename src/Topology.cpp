#include <vector>
#include <tuple>
#include <algorithm>
#include "Topology.h"
#include "lemon/bfs.h"
#include "lemon/dijkstra.h"
#include "lemon/lgf_writer.h"
#include "lemon/graph_to_eps.h"

namespace CostModel {
  void Topology::reserveEdge(unsigned int num_devices, NetworkType type) {
    switch (type) {
      case NetworkType::PART_CONN_GRAPH: // nothing we can do!
        break;
      case NetworkType::FULL_CONN_GRAPH:
        topo_graph.reserveEdge((num_devices * (num_devices-1)) / 2);
        topo_edges.reserve((num_devices * (num_devices-1)) / 2);
        break;
      case NetworkType::STAR:
        topo_graph.reserveEdge(num_devices - 1);
        topo_edges.reserve(num_devices - 1);
        break;
      case NetworkType::RING:
        topo_graph.reserveEdge(num_devices);
        topo_edges.reserve(num_devices);
        break;
      case NetworkType::CART: // nothing we can do!
        break;
    }
    return;
  }

  Topology::Topology(const unsigned int num_devices, const NetworkType type)
    : NETWORK_TYPE(type), topo_devs(topo_graph), topo_links(topo_graph) {
    // The node and edge maps should be updated with changes to the underlying
    // graph, but we will test this.
    topo_graph.reserveNode(num_devices);
    topo_nodes.reserve(num_devices);
    reserveEdge(num_devices, type);

    return;
  }

  Topology::Topology(const unsigned int num_devices,
    const std::vector<DevID>& dev_vec, const NetworkType type)
    : NETWORK_TYPE(type), topo_devs(topo_graph), topo_links(topo_graph) {
    topo_graph.reserveNode(num_devices);
    topo_nodes.reserve(num_devices);
    reserveEdge(num_devices, type);

    lemon::ListGraph::Node node;
    for (const DevID& dev_id : dev_vec) {
      node = topo_graph.addNode();
      topo_devs[node] = dev_id;
      topo_nodes.emplace(dev_id, node);
    }

    return;
  }

  Topology::Topology(const unsigned int num_devices, const Topology& src)
    : NETWORK_TYPE(src.NETWORK_TYPE), topo_devs(topo_graph),
    topo_nodes(src.topo_nodes), topo_links(topo_graph),
    topo_edges(src.topo_edges) {

    lemon::GraphCopy<lemon::ListGraph, lemon::ListGraph>
      copy_graph(src.topo_graph, topo_graph);
    copy_graph.nodeMap(src.topo_devs, topo_devs);
    copy_graph.edgeMap(src.topo_links, topo_links);
    copy_graph.run();

    if (num_devices > topo_nodes.max_size()) {
      topo_graph.reserveNode(num_devices);
      topo_nodes.reserve(num_devices);
      reserveEdge(num_devices, NETWORK_TYPE);
    }

    return;
  }

  Topology::Topology(const unsigned int num_devices, const Topology& src,
    const NetworkType net_type) : NETWORK_TYPE(net_type), topo_devs(topo_graph),
    topo_nodes(src.topo_nodes), topo_links(topo_graph),
    topo_edges(src.topo_edges) {

    lemon::GraphCopy<lemon::ListGraph, lemon::ListGraph>
      copy_graph(src.topo_graph, topo_graph);
    copy_graph.nodeMap(src.topo_devs, topo_devs);
    copy_graph.edgeMap(src.topo_links, topo_links);
    copy_graph.run();

    if (num_devices > topo_nodes.max_size()) {
      topo_graph.reserveNode(num_devices);
      topo_nodes.reserve(num_devices);
      reserveEdge(num_devices, NETWORK_TYPE);
    }

    return;
  }

  void Topology::graphWriter(std::ostream& os) const {
    lemon::graphWriter(topo_graph, os)
      .edgeMap("Links", topo_links)
      .nodeMap("Devices", topo_devs)
      .run();

    return;
  }

  void Topology::graphDisplay(const std::string& TITLE,
  const std::string& FNAME) const {
    lemon::graphToEps(topo_graph, FNAME).title(TITLE).run();
    return;
  }

  void Topology::addDevice(const DevID DEV_ID) {
    if (topo_nodes.find(DEV_ID) == topo_nodes.end()) {
      lemon::ListGraph::Node node = topo_graph.addNode();
      topo_devs[node] = DEV_ID;
      topo_nodes.emplace(DEV_ID, node);
    }
    return;
  }

  void Topology::addDevice(const std::vector<DevID>& DEV_VEC) {
    for (DevID dev_id : DEV_VEC) {
      addDevice(dev_id);
    }
    return;
  }

  void Topology::removeDevice(const DevID DEV_ID) {
    if (topo_nodes.find(DEV_ID) != topo_nodes.end()) {
      // bit more complicated, we need to also remove any connected edges
      lemon::ListGraph::Node node = topo_nodes.at(DEV_ID);

      for (lemon::ListGraph::IncEdgeIt edge_it(topo_graph, node);
        edge_it != lemon::INVALID; ++edge_it) {
        // get edge and corresponding LinkID
        lemon::ListGraph::Edge edge(edge_it);
        LinkID link_id = topo_links[edge].getLinkID();
        // remove both!
        topo_graph.erase(edge);
        topo_edges.erase(link_id);
      }

      // should now be safe to remove node
      topo_graph.erase(node);
      topo_nodes.erase(DEV_ID);
    }
    return;
  }

  void Topology::removeDevice(const std::vector<DevID>& DEV_VEC) {
    // take the coward's way out
    for (DevID dev_id : DEV_VEC) {
      removeDevice(dev_id);
    }
    return;
  }

  void Topology::setLink(const DevID IDA, const DevID IDB, Link link) {
    if (!linkExists(IDA, IDB)) {
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
    }

    return;
  }

  void Topology::unsetLink(const DevID IDA, const DevID IDB) {
    if (linkExists(IDA, IDB)) {
      const LinkID LINK_ID = unorderedCantor(IDA, IDB);
      const lemon::ListGraph::Edge EDGE = topo_edges.at(LINK_ID);

      topo_graph.erase(EDGE);
      topo_edges.erase(LINK_ID);
    }

    return;
  }

  bool Topology::linkExists(const DevID IDA, const DevID IDB) const {
    const LinkID LINK_ID = unorderedCantor(IDA, IDB);

    std::unordered_map<LinkID,lemon::ListGraph::Edge>::const_iterator it_edge
      = topo_edges.find(LINK_ID);

    if(it_edge == topo_edges.end()) return false;

    return true;
  }

  bool Topology::routeExists(const DevID IDA, const DevID IDB) const {
    if (NETWORK_TYPE != NetworkType::PART_CONN_GRAPH) return true;

    const lemon::ListGraph::Node NODE_A = topo_nodes.at(IDA);
    const lemon::ListGraph::Node NODE_B = topo_nodes.at(IDB);

    bool reached = bfs(topo_graph).run(NODE_A, NODE_B);

    return reached;
  }

  std::vector<DevID> Topology::getMostDirectRoute(const DevID IDA,
    const DevID IDB) const {
    std::vector<DevID> route;

    if (IDA == IDB) {
      route.push_back(IDA);
      return route;
    }

    lemon::ListGraph::Node NODE_A = topo_nodes.at(IDA);
    lemon::ListGraph::Node NODE_B = topo_nodes.at(IDB);
    lemon::ListGraph::EdgeMap<int> unit_length(topo_graph, 1);
    lemon::Dijkstra<lemon::ListGraph, lemon::ListGraph::EdgeMap<int>>
      solver(topo_graph, unit_length);

    bool reached = solver.run(NODE_A, NODE_B);
    if (!reached) return route;
    int length = solver.dist(NODE_B);
    route.reserve(length);

    for (lemon::ListGraph::Node node = NODE_B; node != lemon::INVALID;
      node = solver.predNode(node)) {
      DevID dev_id = topo_devs[node];
      route.push_back(dev_id);
    }
    std::reverse(route.begin(), route.end());

    return route;
  }

}
