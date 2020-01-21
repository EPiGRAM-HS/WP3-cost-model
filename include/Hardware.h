#ifndef HARDWARE_H
#define HARDWARE_H

#include <iostream>
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
      Hardware(const std::vector<DevInfo>&);
      Hardware(const std::vector<DevInfo>&, NetworkType);
      Hardware(const std::vector<DevInfo>&, Hardware&);
      Hardware(const std::vector<DevInfo>&, Hardware&, NetworkType);
      Hardware(const Hardware& src) :
      num_devices(src.num_devices), devices(src.devices),
      topo(num_devices, src.topo), NULLDEV(NULL) {};

      void printDevices(std::ostream& os = std::cout) const;
      void printTopology(std::ostream& os = std::cout) const;
      void displayTopology(const std::string&, const std::string&) const;

      std::string getDeviceName(const DevID) const;
      unsigned int getNumDevices() const { return num_devices; }
      const Device& getDevice(const DevID) const;
      const std::vector<Device>& getDevices() const { return devices; }
      Topology& getTopology() { return topo; }
  };
}

#endif
