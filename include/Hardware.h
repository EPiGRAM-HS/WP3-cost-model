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
      const Device NULLDEV;

    public:
      Hardware() = delete;
      Hardware(const std::vector<std::tuple<std::string, Cost, Cost, double,
        unsigned int>>&);
      Hardware(const std::vector<std::tuple<std::string, Cost, Cost, double,
        unsigned int>>&, NetworkType);
      Hardware(const std::vector<std::tuple<std::string, Cost, Cost, double,
        unsigned int>>&, Hardware&);
      Hardware(const std::vector<std::tuple<std::string, Cost, Cost, double,
        unsigned int>>&, Hardware&, NetworkType);

      std::string getDeviceName(const DevID) const;
      unsigned int getNumDevices() const { return num_devices; }
      const Device& getDevice(const DevID) const;
      const std::vector<Device>& getDevices() const { return devices; }
      Topology& getTopology() { return topo; }
  };
}

#endif
