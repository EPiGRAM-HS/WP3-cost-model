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

      void setLinkID(DevID A, DevID B) {
        link_id = CostModel::unorderedCantor(A, B);
        return;
      };
      LinkID getLinkID() const { return link_id; }
      unsigned int getLatency() const { return latency; }
      double getInverseBW() const { return inverse_bw; }
  };
}

#endif
