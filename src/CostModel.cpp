#include <limits>
#include "typedefs.h"
#include "CostModel.h"
#include "Access.h"

namespace CostModel {
  Cost BasicCostModel::accessCost(const DevID DEV_ID, const DataLayout& LAYOUT,
  const AccessPattern& AP, const unsigned int COUNT, const Hardware& HARDWARE) {
    const Cost BAC = HARDWARE.getDevice(DEV_ID).getBasicAccessCost(1);
    const Cost EAC = HARDWARE.getDevice(DEV_ID).getExpensiveAccessCost(1);
    const Access ACCESS(AP, LAYOUT, COUNT);
    Cost total = 0;

    // for each access in add the cost for the type of access multiplied by the
    // count
    for (auto acc_iter = ACCESS.begin(); acc_iter != ACCESS.end(); ++acc_iter) {
      switch (std::get<0>(*acc_iter)) {
        case AccessType::FREE       : break;
        case AccessType::BASIC      : total += BAC * std::get<1>(*acc_iter);
                                      break;
        case AccessType::EXPENSIVE  : total += EAC * std::get<1>(*acc_iter);
                                      break;
      }
    }

    total *= COUNT;

    return total;
  }

  Cost BasicCostModel::movementCost(const DevID DEV_SRC,
    const DataLayout& LAYOUT_SRC, const DevID DEV_DEST,
    const DataLayout& LAYOUT_DEST, Hardware& hw) {
    const AccessPattern WORST_AP(1, std::make_pair(AccessType::EXPENSIVE, 1));

    // this is a very bad model

    Cost read_cost = accessCost(DEV_SRC, LAYOUT_SRC, WORST_AP, 1, hw);
    Cost write_cost = accessCost(DEV_DEST, LAYOUT_DEST, WORST_AP, 1, hw);

    const std::vector<DevID>
      route = hw.getTopology().getMostDirectRoute(DEV_SRC, DEV_DEST);
    Cost transport_cost = AccessType::EXPENSIVE * (route.size()-1);

    return read_cost + write_cost + transport_cost;
  }

  bool BasicCostModel::movementDecision(const DevID DEV_SRC,
    const DataLayout& LAYOUT_SRC, const DevID DEV_DEST,
    const DataLayout& LAYOUT_DEST, const AccessPattern& AP,
    const unsigned int COUNT, Hardware& hw) {
    bool decision = false;

    Cost access_src = accessCost(DEV_SRC, LAYOUT_SRC, AP, COUNT, hw);
    Cost access_dest = accessCost(DEV_DEST, LAYOUT_DEST, AP, COUNT, hw);
    Cost move_cost = COUNT * movementCost(DEV_SRC, LAYOUT_SRC, DEV_DEST,
                              LAYOUT_DEST, hw);

    if (access_dest + move_cost < access_src) decision = true;

    return decision;
  }

  DevID BasicCostModel::recommendDevice(const DataLayout& LAYOUT,
    const AccessPattern& AP, const unsigned int COUNT, const Hardware& HARDWARE)
  {
    DevID winner = 0;
    Cost lowest_cost = std::numeric_limits<Cost>::max();

    for (auto dev_iter = HARDWARE.getDevices().begin();
      dev_iter != HARDWARE.getDevices().end(); ++dev_iter) {
      Cost cost = accessCost(dev_iter->getID(), LAYOUT, AP, COUNT, HARDWARE);
      if (cost < lowest_cost) winner = dev_iter->getID();
    }

    return winner;
  }
}
