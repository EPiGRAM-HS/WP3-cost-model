#include <tuple>
#include "Access.h"

namespace CostModel {
  const AccessPattern Access::unrollAccessPattern(
    const AccessPattern& in_pattern, const DataLayout& layout) {
    const AccessPattern& lpattern = layout.getPattern();

    size_t unrolled_size = 0;
    for (auto acc : in_pattern)
      unrolled_size += std::get<1>(acc) * lpattern.size();

    AccessPattern out_pattern(unrolled_size);

    for (auto in_acc : in_pattern) {
      for (size_t rep = 0; rep < std::get<1>(in_acc); ++rep) {
        if (std::get<0>(in_acc) < AccessType::EXPENSIVE) {
          // cost is just that of underlying datatype
          for (auto acc : lpattern) out_pattern.push_back(acc);
        } else {
          // cost of initial access should be promoted to EXPENSIVE
          out_pattern.push_back(std::make_pair(AccessType::EXPENSIVE, 1));

          auto [init_type, init_count] = lpattern.front();
          if (init_count > 1)
          out_pattern.push_back(std::make_pair(init_type, init_count - 1));

          for (auto lpat_it = lpattern.begin() + 1; lpat_it != lpattern.end();
            ++lpat_it) out_pattern.push_back(*lpat_it);
        }
      }
    }

    return out_pattern;
  }
}
