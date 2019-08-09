#ifndef ACCESS_H
#define ACCESS_H

#include <tuple>
#include "typedefs.h"
#include "DataLayout.h"

namespace CostModel {
  class Access {
    private:
      const AccessPattern& PATTERN;
      const DataLayout& DATA_LAYOUT;
      const unsigned int COUNT;

      const AccessPattern unrollAccessPattern(const AccessPattern&,
        const DataLayout&);

    public:
      Access(const AccessPattern& patt, const DataLayout& type, unsigned int count = 1)
      : PATTERN(unrollAccessPattern(patt, type)), DATA_LAYOUT(type),
        COUNT(count) {};

      unsigned int getReps() const { return COUNT; }
      AccessPattern::const_iterator begin() const { return PATTERN.begin(); }
      AccessPattern::const_iterator end() const { return PATTERN.end(); }
  };
}

#endif
