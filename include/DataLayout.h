#ifndef DATALAYOUT_H
#define DATALAYOUT_H

#include <string>
#include "typedefs.h"

namespace CostModel {
  class DataLayout {
    private:
      const std::string NAME;
      const unsigned int EXTENT;
      const AccessPattern PATTERN;

    public:
      // assumes copy cost of ap is reasonable
      DataLayout(std::string name, unsigned int extent, AccessPattern& ap)
        : NAME(name), EXTENT(extent), PATTERN(ap) {};

      std::string getName() { return NAME; }
      unsigned int getExtent() { return EXTENT; }
      AccessPattern getPattern() { return PATTERN; }
  };
}

#endif
