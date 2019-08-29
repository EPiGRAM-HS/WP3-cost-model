#include <tuple>
#include "Access.h"

namespace CostModel {
  const AccessPattern Access::unrollAccessPattern(
    const AccessPattern& IN_PATTERN, const DataLayout& layout) {
    const AccessPattern& LPATTERN = layout.getPattern();
    AccessPattern out_pattern;

    if (LPATTERN.empty() || IN_PATTERN.empty()) return out_pattern;

    const AccessType DTYPE_FIRST = std::get<0>(LPATTERN.front());

    size_t unrolled_size = 0;
    for (auto ACC : IN_PATTERN)
      unrolled_size += std::get<1>(ACC) * LPATTERN.size();
    out_pattern.reserve(unrolled_size);

    for (auto ACC : IN_PATTERN) {
      for (size_t rep = 0; rep < std::get<1>(ACC); ++rep) {
        if (std::get<0>(ACC) <= DTYPE_FIRST) {
          // cost is just that of underlying datatype
          for (auto LACC : LPATTERN) out_pattern.push_back(LACC);
        } else {
          // cost of initial access should be promoted
          out_pattern.push_back(std::make_pair(std::get<0>(ACC), 1));

          auto [INIT_ACC, INIT_COUNT] = LPATTERN.front();
          if (INIT_COUNT > 1)
          out_pattern.push_back(std::make_pair(INIT_ACC, INIT_COUNT - 1));

          for (auto LPAT_IT = LPATTERN.begin() + 1; LPAT_IT != LPATTERN.end();
            ++LPAT_IT) out_pattern.push_back(*LPAT_IT);
        }
      }
    }

    return out_pattern;
  }
}
