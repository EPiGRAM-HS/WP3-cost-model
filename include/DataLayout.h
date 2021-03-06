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
      DataLayout(const std::string name, const unsigned int extent,
        const AccessPattern& ap) : NAME(name), EXTENT(extent), PATTERN(ap) {};

      std::string getName() const { return NAME; }
      unsigned int getExtent() const { return EXTENT; }
      const AccessPattern& getPattern() const { return PATTERN; }
  };
}

#endif
