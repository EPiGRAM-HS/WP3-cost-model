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
        known_data_layouts.emplace(str_cmbyte, DataLayout(str_cmbyte, 1,
          AccessPattern(1, std::make_pair(AccessType::BASIC, 1))));

        return;
      }

    public:
      BasicCostModel() = default;
      BasicCostModel(const std::vector<std::tuple<std::string, Cost, Cost, double,
        unsigned int>>& hw_info) : hardware(hw_info) { defaultLayouts(); }


      Hardware& getHardware() { return hardware; }

  };
}

#endif
