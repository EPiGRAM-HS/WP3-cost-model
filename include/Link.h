#ifndef LINK_H
#define LINK_H

#include "typedefs.h"
#include "utils.h"

namespace CostModel {
  class Link {
    private:
      LinkID link_id;
      unsigned int latency;
      double inverse_bw;

    public:
      Link() = delete;
      Link(unsigned int lat, double inv_bw)
      : latency(lat), inverse_bw(inv_bw) {};

      Link& operator+=(const Link& RHS) {
        // treating inverse bandwidth (s/B) as summative here...
        link_id = 0;
        latency += RHS.latency;
        inverse_bw += RHS.latency;
        return *this;
      }
      friend Link operator+(Link lhs, const Link& RHS) { return lhs += RHS; }

      void setLinkID(DevID A, DevID B) {
        link_id = unorderedCantor(A, B);
        return;
      }
      LinkID getLinkID() const { return link_id; }
      unsigned int getLatency() const { return latency; }
      double getInverseBW() const { return inverse_bw; }
  };
}

#endif
