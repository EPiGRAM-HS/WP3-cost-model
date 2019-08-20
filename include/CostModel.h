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
      BasicCostModel(const std::vector<std::tuple<std::string, Cost, Cost, double,
        unsigned int>>& hw_info) : hardware(hw_info) { defaultLayouts(); }

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
  };
}

#endif
