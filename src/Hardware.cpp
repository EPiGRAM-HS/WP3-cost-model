#include <vector>
#include <tuple>
#include <string>
#include "typedefs.h"
#include "Hardware.h"
#include "Device.h"
#include "Topology.h"

namespace CostModel {
  Hardware::Hardware(const std::vector<std::tuple<std::string, Cost, Cost,
  double, unsigned int>>& device_info) :
  num_devices(device_info.size()),
  topo(num_devices, NetworkType::PART_CONN_GRAPH), NULLDEV(NULL) {

    std::vector<DevID> dev_ids;

    dev_ids.reserve(num_devices);
    devices.reserve(num_devices);
    for (auto const& dev_info : device_info) {
      devices.emplace_back(std::get<0>(dev_info), std::get<1>(dev_info),
        std::get<2>(dev_info), std::get<3>(dev_info), std::get<4>(dev_info));
      dev_ids.push_back(devices.back().getID());
    }

    topo.addDevice(dev_ids);

    return;
  }

  Hardware::Hardware(const std::vector<std::tuple<std::string, Cost, Cost,
  double, unsigned int>>& device_info, NetworkType net_type) :
  num_devices(device_info.size()), topo(num_devices, net_type),
  NULLDEV(NULL) {

    std::vector<DevID> dev_ids;

    dev_ids.reserve(num_devices);
    devices.reserve(num_devices);
    for (auto const& dev_info : device_info) {
      devices.emplace_back(std::get<0>(dev_info), std::get<1>(dev_info),
        std::get<2>(dev_info), std::get<3>(dev_info), std::get<4>(dev_info));
      dev_ids.push_back(devices.back().getID());
    }

    topo.addDevice(dev_ids);

    return;
  }

  Hardware::Hardware(const std::vector<std::tuple<std::string, Cost, Cost,
  double, unsigned int>>& device_info, Hardware& old_hw) :
  num_devices(device_info.size() + old_hw.devices.size()),
  devices(std::move(old_hw.devices)), topo(num_devices, old_hw.topo),
  NULLDEV(NULL) {

    devices.reserve(num_devices);

    std::vector<DevID> dev_ids;
    for (auto const& dev_info : device_info) {
      devices.emplace_back(std::get<0>(dev_info), std::get<1>(dev_info),
        std::get<2>(dev_info), std::get<3>(dev_info), std::get<4>(dev_info));
      dev_ids.push_back(devices.back().getID());
    }

    topo.addDevice(dev_ids);

    return;
  }

  Hardware::Hardware(const std::vector<std::tuple<std::string, Cost, Cost,
  double, unsigned int>>& device_info, Hardware& old_hw, NetworkType net_type) :
  num_devices(device_info.size() + old_hw.devices.size()),
  devices(std::move(old_hw.devices)), topo(num_devices, old_hw.topo, net_type),
  NULLDEV(NULL) {

    devices.reserve(num_devices);

    std::vector<DevID> dev_ids;
    for (auto const& dev_info : device_info) {
      devices.emplace_back(std::get<0>(dev_info), std::get<1>(dev_info),
        std::get<2>(dev_info), std::get<3>(dev_info), std::get<4>(dev_info));
      dev_ids.push_back(devices.back().getID());
    }

    topo.addDevice(dev_ids);

    return;
  }

  void Hardware::printDevices(std::ostream& os) const {
    os << "Hardware device list:\n" << "  NAME  :  ID\n";
    for (const Device& dev : devices) {
      os << "  " <<  dev.getName() << "  :  " << dev.getID()
      << "\n";
    }
    os << "\n";

    return;
  }

  void Hardware::printTopology(std::ostream& os) const {
    topo.graphWriter(os);
    os << "\n";
    return;
  }

  void Hardware::displayTopology(const std::string& TITLE,
  const std::string& FNAME) const {
    topo.graphDisplay(TITLE, FNAME);
    return;
  }

  std::string Hardware::getDeviceName(const DevID DEV_ID) const {
    for (const Device& dev : devices) {
      if (dev.getID() == DEV_ID) return dev.getName();
    }
    return std::string("NULL");
  }

  const Device& Hardware::getDevice(const DevID DEV_ID) const {
    for (const Device& dev : devices) {
      if(dev.getID() == DEV_ID) return dev;
    }
    return NULLDEV;
  }
}
