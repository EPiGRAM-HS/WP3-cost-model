#ifndef COSTMODEL_H
#define COSTMODEL_H

#include <map>
#include <string>
#include "typedefs.h"
#include "Hardware.h"
#include "DataLayout.h"

namespace CostModel {
  class BasicCostModel {
    protected:
      Hardware hardware;
      std::unordered_map<std::string, DataLayout> known_data_layouts;

      void defaultLayouts() {
        std::string str_cmbyte("CM_BYTE");
        AccessPattern ap(1, std::make_pair(AccessType::BASIC, 1));
        known_data_layouts.emplace(str_cmbyte, DataLayout(str_cmbyte, 1, ap));
        return;
      }

    public:
      BasicCostModel() = delete;
      BasicCostModel(const std::vector<DevInfo>& hw_info)
        : hardware(hw_info) { defaultLayouts(); }
      BasicCostModel(Hardware& hw) : hardware(hw) { defaultLayouts(); }

      Hardware& getHardware() { return hardware; }

      void addDataLayout(std::string name, unsigned int extent,
        AccessPattern& ap) {
        known_data_layouts.emplace(name, DataLayout(name, extent, ap));
        return;
      }
      void rmDataLayout(std::string name) {
        known_data_layouts.erase(name);
        return;
      }
      const DataLayout& getDataLayout(const std::string NAME) const {
        return known_data_layouts.at(NAME);
      }

      // cost to access data in a particular pattern on a particular device
      Cost accessCost(const DevID DEV_ID, const DataLayout& LAYOUT,
        const AccessPattern& AP, const unsigned int COUNT)
        { return accessCost(DEV_ID, LAYOUT, AP, COUNT, hardware); }
      Cost accessCost(const DevID, const DataLayout&, const AccessPattern&,
        const unsigned int, const Hardware&);

      // cost to move data from device A to device B
      Cost movementCost(const DevID DEV_SRC, const DataLayout& LAYOUT_SRC,
        const DevID DEV_DEST, const DataLayout& LAYOUT_DEST)
        { return movementCost(DEV_SRC, LAYOUT_SRC, DEV_DEST, LAYOUT_DEST,
          hardware); }
      Cost movementCost(const DevID, const DataLayout&, const DevID,
        const DataLayout&, Hardware&);

      // should data be moved from device A to device B?
      bool movementDecision(const DevID DEV_SRC, const DataLayout& LAYOUT_SRC,
        const DevID DEV_DEST, const DataLayout& LAYOUT_DEST,
        const AccessPattern& AP, const unsigned int COUNT)
        { return movementDecision(DEV_SRC, LAYOUT_SRC, DEV_DEST, LAYOUT_DEST,
          AP, COUNT, hardware); }
      bool movementDecision(const DevID, const DataLayout&, const DevID,
        const DataLayout&, const AccessPattern&, const unsigned int,
        Hardware&);

      // which device would be best for this data?
      DevID recommendDevice(const DataLayout& LAYOUT, const AccessPattern& AP,
        const unsigned int COUNT)
        { return recommendDevice(LAYOUT, AP, COUNT, hardware); }
      DevID recommendDevice(const DataLayout&, const AccessPattern&,
        const unsigned int, const Hardware&);
  };
}

#endif
