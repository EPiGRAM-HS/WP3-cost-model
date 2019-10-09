#ifndef LINK_H
#define LINK_H

#include <iostream>
#include "typedefs.h"
#include "utils.h"

namespace CostModel {
  class Link {
    private:
      LinkID link_id;
      unsigned int latency;     // in nanoseconds
      unsigned int inverse_bw;  // in nanoseconds per bit

    public:
      Link() : latency(0), inverse_bw(0) {};
      Link(unsigned int lat, unsigned int inv_bw)
      : latency(lat), inverse_bw(inv_bw) {};

      Link& operator+=(const Link& RHS) {
        // treating inverse bandwidth (ns/b) as summative here...
        link_id = 0;
        latency += RHS.latency;
        inverse_bw += RHS.inverse_bw;
        return *this;
      }
      friend Link operator+(Link lhs, const Link& RHS) { return lhs += RHS; }

      friend std::ostream& operator<<(std::ostream& os, const Link& LINK) 
        { return os << LINK.link_id; }

      void setLinkID(const DevID A, const DevID B) {
        link_id = unorderedCantor(A, B);
        return;
      }
      LinkID getLinkID() const { return link_id; }
      unsigned int getLatency() const { return latency; }
      unsigned int getInverseBW() const { return inverse_bw; }
  };
}

#endif
