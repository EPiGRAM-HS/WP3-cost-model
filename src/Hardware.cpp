#include <vector>
#include <tuple>
#include <string>
#include "typedefs.h"
#include "Hardware.h"
#include "Device.h"
#include "Topology.h"

namespace CostModel {
  Hardware::Hardware(const std::vector<std::tuple<std::string, Cost, Cost,
    double, unsigned int>>& device_info)
    : num_devices(device_info.size()),
    topo(num_devices, NetworkType::PART_CONN_GRAPH) {

    std::vector<DevID> dev_ids;

    dev_ids.reserve(num_devices);
    devices.reserve(num_devices);
    for (auto dev : device_info) {
      Device new_dev = std::make_from_tuple<Device>(dev);
      devices.push_back(new_dev);
      dev_ids.push_back(new_dev.getID());
    }

    topo.addDevice(dev_ids);

    return;
  }

  Hardware::Hardware(const std::vector<std::tuple<std::string, Cost, Cost,
    double, unsigned int>>& device_info, NetworkType net_type)
    : num_devices(device_info.size()), topo(num_devices, net_type) {

    std::vector<DevID> dev_ids;

    dev_ids.reserve(num_devices);
    devices.reserve(num_devices);
    for (auto dev_info : device_info) {
      Device dev = std::make_from_tuple<Device>(dev_info);
      devices.push_back(dev);
      dev_ids.push_back(dev.getID());
    }

    topo.addDevice(dev_ids);

    return;
  }

  Hardware::Hardware(const std::vector<std::tuple<std::string, Cost, Cost,
    double, unsigned int>>& device_info, Hardware& old_hw)
    : num_devices(device_info.size() + old_hw.devices.size()),
    topo(num_devices, old_hw.topo) {

    devices.reserve(num_devices);

    for (Device dev : old_hw.devices) {
      devices.push_back(dev);
    }

    std::vector<DevID> dev_ids(device_info.size());
    for (auto dev_info : device_info) {
      Device dev = std::make_from_tuple<Device>(dev_info);
      devices.push_back(dev);
      dev_ids.push_back(dev.getID());
    }

    topo.addDevice(dev_ids);

    return;
  }

  Hardware::Hardware(const std::vector<std::tuple<std::string, Cost, Cost,
    double, unsigned int>>& device_info, Hardware& old_hw, NetworkType net_type)
    : num_devices(device_info.size() + old_hw.devices.size()),
    topo(num_devices, old_hw.topo, net_type) {

    devices.reserve(num_devices);

    for (Device dev : old_hw.devices) {
      devices.push_back(dev);
    }

    std::vector<DevID> dev_ids(device_info.size());
    for (auto dev_info : device_info) {
      Device dev = std::make_from_tuple<Device>(dev_info);
      devices.push_back(dev);
      dev_ids.push_back(dev.getID());
    }

    topo.addDevice(dev_ids);

    return;
  }

  std::string Hardware::getDeviceName(const DevID DEV_ID) const {
    for (Device dev : devices) {
      if (dev.getID() == DEV_ID) return dev.getName();
    }
    return std::string("NULL");
  }

  const Device Hardware::getDevice(const DevID DEV_ID) const {
    for (Device dev : devices) {
      if(dev.getID() == DEV_ID) return dev;
    }
    return Device(NULL);
  }
}
