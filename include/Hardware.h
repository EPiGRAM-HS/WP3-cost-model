#ifndef HARDWARE_H
#define HARDWARE_H

#include <vector>
#include <tuple>
#include <string>
#include "typedefs.h"
#include "Device.h"
#include "Topology.h"

namespace CostModel {
  class Hardware {
    private:
      unsigned int num_devices;
      std::vector<Device> devices;
      Topology topo;

    public:
      Hardware() = delete;
      Hardware(const std::vector<std::tuple<std::string, Cost, Cost, double,
        unsigned int>>&);
      Hardware(const std::vector<std::tuple<std::string, Cost, Cost, double,
        unsigned int>>&, NetworkType);
      Hardware(const std::vector<std::tuple<std::string, Cost, Cost, double,
        unsigned int>>&, Hardware& old_hw);
      Hardware(const std::vector<std::tuple<std::string, Cost, Cost, double,
        unsigned int>>&, Hardware& old_hw, NetworkType);

      std::string getDeviceName(const DevID) const;
      unsigned int getNumDevices() const { return num_devices; }
      const Device getDevice(const DevID) const;
      Topology& getTopology() { return topo; }
  };
}

#endif
