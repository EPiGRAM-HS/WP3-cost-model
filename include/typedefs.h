#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <tuple>
#include <vector>

namespace CostModel {
  enum AccessType {
    FREE,
    BASIC,
    EXPENSIVE
  };

  enum NetworkType {
    PART_CONN_GRAPH,
    FULL_CONN_GRAPH,
    STAR,
    RING,
    CART
  };

  typedef unsigned int Cost;
  typedef unsigned int DevID;
  typedef unsigned int LinkID;
  typedef std::vector<std::tuple<AccessType, unsigned int>> AccessPattern;
}

#endif
