#ifndef LINK_H
#define LINK_H

namespace CostModel {
  class Link {
    private:
      const unsigned int LATENCY;
      const double INV_BW;

    public:
      Link() = delete;
      Link(unsigned int lat, double inv_bw)
      : LATENCY(lat), INV_BW(inv_bw) {};

      unsigned int getLatency() { return LATENCY; }
      double getInverseBW() { return INV_BW; }
  };
}

#endif
