#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "utils.h"
#include "Device.h"
#include "Link.h"
#include "DataLayout.h"
#include "Topology.h"
#include "Access.h"
#include "Hardware.h"
#include "CostModel.h"

using namespace CostModel;

TEST_CASE("Device", "[unit]")
{
  SECTION("NULL DEVICE") {
    const Device DEV_NULL(NULL);
    REQUIRE(DEV_NULL.isNull());
    REQUIRE(DEV_NULL.getID() == 0);
    REQUIRE_THAT(DEV_NULL.getName(), Catch::Equals("NULL"));
    REQUIRE(DEV_NULL.getBasicAccessCost(1) == 0);
    REQUIRE(DEV_NULL.getBasicAccessCost(100) == 0);
    REQUIRE(DEV_NULL.getExpensiveAccessCost(0) == 0);
    REQUIRE(DEV_NULL.getExpensiveAccessCost(100) == 0);
    REQUIRE(DEV_NULL.getCapacity() == 0.0);
    REQUIRE(DEV_NULL.getVectorLength() == 0);
  }

  SECTION("One Device") {
    const std::string NAME("DRAM");
    const unsigned int BAC = 100;
    const unsigned int EAC = 100 * BAC;
    const double CAP = 256e9;
    const unsigned int VECLEN = 0;
    const Device DRAM(NAME, BAC, EAC, CAP, VECLEN);

    REQUIRE_FALSE(DRAM.isNull());
    REQUIRE(DRAM.getID() == 1);
    REQUIRE_THAT(DRAM.getName(), Catch::Equals(NAME));
    REQUIRE(DRAM.getBasicAccessCost(1) == BAC);
    REQUIRE(DRAM.getBasicAccessCost(10) == 10 * BAC);
    REQUIRE(DRAM.getExpensiveAccessCost(1) == EAC);
    REQUIRE(DRAM.getExpensiveAccessCost(7) == 7 * EAC);
    REQUIRE(DRAM.getCapacity() == CAP);
    REQUIRE(DRAM.getVectorLength() == VECLEN);
  }

  SECTION("Multiple Devices") {
    const Device DRAM(std::string("DRAM"), 100, 10000, 256e9, 0);
    const Device L1(std::string("L1 Cache"), 10, 20, 32768, 512);
    const Device DRAM2(DRAM);

    REQUIRE(DRAM.getID() == 2);
    REQUIRE(DRAM2.getID() == 4);
    REQUIRE(L1.getID() == 3);
    REQUIRE_THAT(DRAM.getName(), Catch::Equals(std::string("DRAM")));
    REQUIRE_THAT(DRAM2.getName(), Catch::Equals(std::string("DRAM")));
    REQUIRE_THAT(L1.getName(), Catch::Equals(std::string("L1 Cache")));
    REQUIRE(L1.getVectorLength() == 512);
    REQUIRE(DRAM2.getVectorLength() == 0);
  }
}

TEST_CASE("Link", "[unit]")
{
  SECTION("Default Link") {
    const Link LINK;
    REQUIRE(LINK.getLatency() == 0);
    REQUIRE(LINK.getInverseBW() == 0);
  }

  SECTION("One Link") {
    const unsigned int LAT = 2;
    const unsigned int INV_BW = 4E4;
    const DevID DEV_A = 5;
    const DevID DEV_B = 12;
    const LinkID LID = unorderedCantor(DEV_A, DEV_B);
    Link link(LAT, INV_BW);

    link.setLinkID(DEV_B, DEV_A);

    REQUIRE(link.getLatency() == LAT);
    REQUIRE(link.getInverseBW() == INV_BW);
    REQUIRE(link.getLinkID() == LID);
  }

  SECTION("Multiple Links") {
    const unsigned int LAT1 = 2;
    const unsigned int LAT2 = 3;
    const unsigned int INV_BW1 = 4E4;
    const unsigned int INV_BW2 = 57E3;
    const DevID DEV_A = 5;
    const DevID DEV_B = 12;
    const DevID DEV_C = 2;

    Link link1(LAT1, INV_BW1);
    Link link2(LAT2, INV_BW2);
    Link link3;

    link1.setLinkID(DEV_A, DEV_B);
    link2.setLinkID(DEV_B, DEV_A);

    REQUIRE(link1.getLatency() == LAT1);
    REQUIRE(link2.getLatency() == LAT2);
    REQUIRE(link1.getInverseBW() == INV_BW1);
    REQUIRE(link2.getInverseBW() == INV_BW2);
    REQUIRE(link1.getLinkID() == link2.getLinkID());

    link3 = link1;
    REQUIRE(link3.getLatency() == LAT1);
    REQUIRE(link3.getInverseBW() == INV_BW1);
    REQUIRE(link3.getLinkID() == link1.getLinkID());

    link3 = link1 + link2;
    REQUIRE(link3.getLatency() == LAT1 + LAT2);
    REQUIRE(link3.getInverseBW() == INV_BW1 + INV_BW2);
    REQUIRE(link3.getLinkID() == 0);

    link3.setLinkID(DEV_C, DEV_A);
    REQUIRE_FALSE((link3.getLinkID() == link1.getLinkID()
      || link3.getLinkID() == link2.getLinkID()));

    link1 += link3;
    REQUIRE(link1.getLinkID() == 0);
    REQUIRE(link1.getLatency() == LAT1 + LAT1 + LAT2);
    REQUIRE(link1.getInverseBW() == INV_BW1 + INV_BW1 + INV_BW2);
  }
}

TEST_CASE("DataLayout", "[unit]")
{
  const std::string NAME("complex double");
  const unsigned int EXTENT = 16;
  AccessPattern AP_COMPDBL(1, std::make_pair(AccessType::BASIC, 16));

  const DataLayout COMPLEX_DOUBLE(NAME, EXTENT, AP_COMPDBL);

  REQUIRE_THAT(COMPLEX_DOUBLE.getName(), Catch::Equals(NAME));
  REQUIRE(COMPLEX_DOUBLE.getExtent() == EXTENT);
  REQUIRE(COMPLEX_DOUBLE.getPattern().size() == 1);
  // check we can get iterator if necessary
  for (auto iter = COMPLEX_DOUBLE.getPattern().begin();
  iter != COMPLEX_DOUBLE.getPattern().end(); ++iter) {
    REQUIRE(std::get<0>(*iter) == AccessType::BASIC);
    REQUIRE(std::get<1>(*iter) == 16);
  }
}

TEST_CASE("Topology", "[unit]")
{
  const unsigned int NUM_DEVICES = 4;
  const NetworkType DEFAULT_NET = NetworkType::PART_CONN_GRAPH;
  const NetworkType FULL_NET = NetworkType::FULL_CONN_GRAPH;
  std::vector<DevID> dev_id_vec = {1, 2, 4, 7};

  SECTION("Initialisation") {
    {
      Topology topo(NUM_DEVICES);
      REQUIRE(topo.getNetworkType() == DEFAULT_NET);
      REQUIRE(topo.getNumDevices() == 0);
      REQUIRE(topo.getNumLinks() == 0);
    }
    {
      Topology topo(NUM_DEVICES, FULL_NET);
      REQUIRE(topo.getNetworkType() == FULL_NET);
      REQUIRE(topo.getNumDevices() == 0);
      REQUIRE(topo.getNumLinks() == 0);
    }
    {
      Topology topo(dev_id_vec.size(), dev_id_vec);
      REQUIRE(topo.getNetworkType() == DEFAULT_NET);
      REQUIRE(topo.getNumDevices() == dev_id_vec.size());
      REQUIRE(topo.getNumLinks() == 0);
    }
    {
      Topology topo(dev_id_vec.size(), dev_id_vec, FULL_NET);
      REQUIRE(topo.getNetworkType() == FULL_NET);
      REQUIRE(topo.getNumDevices() == dev_id_vec.size());
      REQUIRE(topo.getNumLinks() == 0);
    }
    {
      Topology old_topo(1);
      Topology topo(NUM_DEVICES, old_topo);
      REQUIRE(topo.getNetworkType() == DEFAULT_NET);
      REQUIRE(topo.getNumDevices() == 0);
      REQUIRE(topo.getNumLinks() == 0);
    }
    {
      Topology old_topo(1);
      Topology topo(NUM_DEVICES, old_topo, FULL_NET);
      REQUIRE(topo.getNetworkType() == FULL_NET);
      REQUIRE(topo.getNumDevices() == 0);
      REQUIRE(topo.getNumLinks() == 0);
    }
  }

  SECTION("Devices") {
    Topology topo(NUM_DEVICES);

    topo.addDevice(1);
    topo.addDevice(2);
    REQUIRE(topo.getNumDevices() == 2);
    topo.addDevice(7);
    topo.addDevice(5);
    REQUIRE(topo.getNumDevices() == NUM_DEVICES);
    topo.addDevice(5);
    REQUIRE(topo.getNumDevices() == NUM_DEVICES);
    topo.addDevice(6);
    REQUIRE(topo.getNumDevices() == NUM_DEVICES+1);
    REQUIRE(topo.getNumLinks() == 0);

    topo.removeDevice(6);
    REQUIRE(topo.getNumDevices() == NUM_DEVICES);
    topo.removeDevice(3);
    REQUIRE(topo.getNumDevices() == NUM_DEVICES);
    topo.removeDevice(1);
    topo.removeDevice(2);
    topo.removeDevice(7);
    topo.removeDevice(5);
    REQUIRE(topo.getNumDevices() == 0);
    topo.removeDevice(5);
    REQUIRE(topo.getNumDevices() == 0);
    REQUIRE(topo.getNumLinks() == 0);
  }

  SECTION("Links") {
    std::vector<DevID> dev_id_vec = {1, 2, 3, 4};
    Topology topo(dev_id_vec.size(), dev_id_vec);
    const Link CABLE(200, 5);

    for (DevID ida : dev_id_vec) {
      for (DevID idb: dev_id_vec) {
        REQUIRE_FALSE(topo.linkExists(ida, idb));
      }
    }

    topo.setLink(1, 2, CABLE);
    REQUIRE(topo.getNumLinks() == 1);
    REQUIRE(topo.linkExists(2, 1));
    topo.setLink(2, 1, CABLE);
    REQUIRE(topo.getNumLinks() == 1);
    REQUIRE(topo.linkExists(1, 2));

    topo.unsetLink(3, 4);
    REQUIRE(topo.getNumLinks() == 1);
    topo.unsetLink(15, 17);
    REQUIRE(topo.getNumLinks() == 1);
    topo.unsetLink(1, 2);
    REQUIRE(topo.getNumLinks() == 0);
    REQUIRE_FALSE(topo.linkExists(1, 2));
  }

  SECTION("Route") {
    std::vector<DevID> dev_id_vec = {1, 2, 3, 4};
    Topology topo(dev_id_vec.size(), dev_id_vec);
    const Link CABLE(200, 5);
    std::vector<DevID> route(dev_id_vec.size());

    for (auto iter = dev_id_vec.begin(); iter != dev_id_vec.end()-1; ++iter) {
      topo.setLink(*iter, *(iter+1), CABLE);
      REQUIRE(topo.routeExists(*iter, *(iter+1)));
    }
    REQUIRE(topo.routeExists(dev_id_vec.front(), dev_id_vec.back()));
    route = topo.getMostDirectRoute(dev_id_vec.front(), dev_id_vec.back());
    REQUIRE(route == dev_id_vec);

    size_t mid = dev_id_vec.size() / 2;
    topo.unsetLink(dev_id_vec.at(mid), dev_id_vec.at(mid+1));
    route = topo.getMostDirectRoute(dev_id_vec.front(), dev_id_vec.back());
    REQUIRE_FALSE(topo.routeExists(dev_id_vec.front(), dev_id_vec.back()));
    REQUIRE(route.empty());

    topo.setLink(dev_id_vec.front(), dev_id_vec.back(), CABLE);
    route = topo.getMostDirectRoute(dev_id_vec.front(), dev_id_vec.back());
    REQUIRE(topo.routeExists(dev_id_vec.front(), dev_id_vec.back()));
    REQUIRE(route.size() == 2);
    REQUIRE(route.at(0) == dev_id_vec.front());
    REQUIRE(route.at(1) == dev_id_vec.back());

    topo.setLink(dev_id_vec.at(mid), dev_id_vec.at(mid+1), CABLE);
    route = topo.getMostDirectRoute(dev_id_vec.front(), dev_id_vec.back());
    REQUIRE(route.size() == 2);
    REQUIRE(route.at(0) == dev_id_vec.front());
    REQUIRE(route.at(1) == dev_id_vec.back());

    route = topo.getMostDirectRoute(dev_id_vec.back(), dev_id_vec.back());
    REQUIRE(route.size() == 1);
    REQUIRE(route.at(0) == dev_id_vec.back());
  }
}

TEST_CASE("Access", "[unit]") {
  const AccessPattern AP_EMPTY;
  const AccessPattern AP_FREE(1, std::make_pair(AccessType::FREE, 8));
  const AccessPattern AP_BASIC(1, std::make_pair(AccessType::BASIC, 8));
  const AccessPattern AP_EXPENSIVE(1,
    std::make_pair(AccessType::EXPENSIVE, 8) );
  const AccessPattern AP_STRIDED(
    {std::make_pair(AccessType::EXPENSIVE, 1),
    std::make_pair(AccessType::BASIC, 3),
    std::make_pair(AccessType::EXPENSIVE, 1),
    std::make_pair(AccessType::BASIC, 3)} );

  const DataLayout DL_EMPTY(std::string("EMPTY"), 0, AP_EMPTY);
  const DataLayout DL_FREE(std::string("FREE"), 8, AP_FREE);
  const DataLayout DL_BASIC(std::string("BASIC"), 8, AP_BASIC);
  const DataLayout DL_EXPENSIVE(std::string("EXPENSIVE"), 8, AP_EXPENSIVE);
  const DataLayout DL_STRIDED(std::string("STRIDED"), 8, AP_STRIDED);

  SECTION("Empty") {
    Access access_one(AP_EMPTY, DL_EMPTY);
    Access access_ten(AP_EMPTY, DL_EMPTY, 10);

    REQUIRE(access_one.getReps() == 1);
    REQUIRE(access_one.begin() == access_one.end());
    REQUIRE(access_ten.getReps() == 10);
    REQUIRE(access_ten.begin() == access_ten.end());
  }

  SECTION("Unrolling") {
    {
      int count = 0;
      Access access(AP_FREE, DL_FREE, 1);
      REQUIRE(access.getReps() == 1);
      for (auto ITER = access.begin(); ITER != access.end(); ++ITER) {
        REQUIRE(std::get<0>(*ITER) == AccessType::FREE);
        REQUIRE(std::get<1>(*ITER) == 8);
        ++count;
      }
      REQUIRE(count == 8);
    }

    {
      int count = 0;
      Access access(AP_BASIC, DL_FREE, 2);
      AccessPattern expected({std::make_pair(AccessType::BASIC, 1),
        std::make_pair(AccessType::FREE, 7)});
      REQUIRE(access.getReps() == 2);
      for (auto ITER = access.begin(); ITER != access.end(); ++ITER) {
        REQUIRE(std::get<0>(*ITER) == std::get<0>(expected.at(count % 2)));
        REQUIRE(std::get<1>(*ITER) == std::get<1>(expected.at(count % 2)));
        ++count;
      }
      REQUIRE(count == 16);
    }

    {
      int count = 0;
      Access access(AP_EXPENSIVE, DL_FREE, 2);
      AccessPattern expected({std::make_pair(AccessType::EXPENSIVE, 1),
        std::make_pair(AccessType::FREE, 7)});
      REQUIRE(access.getReps() == 2);
      for (auto ITER = access.begin(); ITER != access.end(); ++ITER) {
        REQUIRE(std::get<0>(*ITER)
          == std::get<0>(expected.at(count % expected.size())));
        REQUIRE(std::get<1>(*ITER)
          == std::get<1>(expected.at(count % expected.size())));
        ++count;
      }
      REQUIRE(count == 16);
    }

    {
      Access access(AP_BASIC, DL_EXPENSIVE);
      AccessPattern expected(8, std::make_pair(AccessType::EXPENSIVE, 8));
      REQUIRE(access.getReps() == 1);
      int idx = 0;
      for (auto ITER = access.begin(); ITER != access.end(); ++ITER) {
        REQUIRE(std::get<0>(*ITER) == std::get<0>(expected.at(idx)));
        REQUIRE(std::get<1>(*ITER) == std::get<1>(expected.at(idx)));
        ++idx;
      }
    }

    {
      Access access(AP_BASIC, DL_STRIDED);
      AccessPattern expected( {
        std::make_pair(AccessType::EXPENSIVE, 1),
        std::make_pair(AccessType::BASIC, 3),
        std::make_pair(AccessType::EXPENSIVE, 1),
        std::make_pair(AccessType::BASIC, 3)
      } );
      REQUIRE(access.getReps() == 1);
      int idx = 0;
      for (auto ITER = access.begin(); ITER != access.end(); ++ITER) {
        REQUIRE(std::get<0>(*ITER)
          == std::get<0>(expected.at(idx % expected.size())));
        REQUIRE(std::get<1>(*ITER)
          == std::get<1>(expected.at(idx % expected.size())));
        ++idx;
      }

      {
        Access access(AP_STRIDED, DL_BASIC);
        AccessPattern expected( {
          std::make_pair(AccessType::EXPENSIVE, 1),
          std::make_pair(AccessType::BASIC, 7),
          std::make_pair(AccessType::BASIC, 8),
          std::make_pair(AccessType::BASIC, 8),
          std::make_pair(AccessType::BASIC, 8)
        } );
        REQUIRE(access.getReps() == 1);
        int idx = 0;
        for (auto ITER = access.begin(); ITER != access.end(); ++ITER) {
          REQUIRE(std::get<0>(*ITER)
            == std::get<0>(expected.at(idx % expected.size())));
          REQUIRE(std::get<1>(*ITER)
            == std::get<1>(expected.at(idx % expected.size())));
          ++idx;
        }
      }
    }
  }
}

TEST_CASE("Hardware", "[unit]")
{
  typedef std::vector<std::tuple<std::string, Cost, Cost, double, unsigned int>>
    DevInfoVec;
  typedef std::tuple<std::string, Cost, Cost, double, unsigned int> DevInfo;

  const DevInfo L1(std::string("L1"), 1, 2, 32768, 512);
  const DevInfo L2(std::string("L2"), 10, 20, 262144, 0);
  const DevInfo L3(std::string("L3"), 100, 200, 2.097e6, 0);
  const DevInfo DRAM(std::string("DRAM"), 10000, 20000, 6.872E10, 0);
  const DevInfo GPU(std::string("GPU"), 1000, 2000, 8.59E9, 16);

  DevInfoVec device_info( { L1, L2, L3, DRAM, GPU } );

  SECTION("device_info only") {
    Hardware hardware(device_info);
    REQUIRE(hardware.getNumDevices() == device_info.size());
    REQUIRE(hardware.getTopology().getNetworkType()
      == NetworkType::PART_CONN_GRAPH);
    REQUIRE(hardware.getTopology().getNumDevices() == device_info.size());
    REQUIRE(hardware.getTopology().getNumLinks() == 0);

    DevID devID = 1;
    for (const Device& dev : hardware.getDevices()) {
      REQUIRE(dev.getID() == devID);
      ++devID;
    }

    for (DevID devID = 1; devID <= device_info.size(); ++devID) {
      REQUIRE_THAT(hardware.getDeviceName(devID),
        Catch::Equals(std::get<0>(device_info.at(devID-1))));
    }

    REQUIRE(hardware.getDevice(10).isNull());
  }

  SECTION("device_info and net_type") {
    Hardware hardware(device_info, NetworkType::FULL_CONN_GRAPH);
    REQUIRE(hardware.getNumDevices() == device_info.size());
    REQUIRE(hardware.getTopology().getNetworkType()
      == NetworkType::FULL_CONN_GRAPH);
    REQUIRE(hardware.getTopology().getNumDevices() == device_info.size());
    REQUIRE(hardware.getTopology().getNumLinks() == 0);

    const DevID first_devID = 6;
    DevID devID = first_devID;
    for (const Device& dev : hardware.getDevices()) {
      REQUIRE(dev.getID() == devID);
      ++devID;
    }

    int idx = 0;
    for (DevID devID = first_devID; devID < first_devID + device_info.size();
      ++devID) {
      REQUIRE_THAT(hardware.getDeviceName(devID),
        Catch::Equals(std::get<0>(device_info.at(idx))));
      ++idx;
    }

    REQUIRE(hardware.getDevice(1).isNull());
  }

  SECTION("device_info and old_hw") {
    Hardware old_hardware(device_info);
    const DevInfo GPU2(std::string("GPU2"), 1000, 2000, 8.59E9, 16);
    DevInfoVec add_hw(1, GPU2);

    const int NUM_DEVICES = device_info.size() + add_hw.size();

    std::vector<DevID> old_ids;
    for (const Device& dev : old_hardware.getDevices())
      old_ids.push_back(dev.getID());

    Hardware hardware(add_hw, old_hardware);

    REQUIRE(hardware.getNumDevices() == NUM_DEVICES);
    REQUIRE(hardware.getTopology().getNetworkType()
      == NetworkType::PART_CONN_GRAPH);
    REQUIRE(hardware.getTopology().getNumDevices() == NUM_DEVICES);
    REQUIRE(hardware.getTopology().getNumLinks() == 0);

    std::vector<DevID> ids;
    for (size_t idx = 0; idx < device_info.size(); ++idx) {
      REQUIRE(old_ids.at(idx) == hardware.getDevices().at(idx).getID());
      ids.push_back(hardware.getDevices().at(idx).getID());
    }

    DevID devID = old_ids.back() + 1;
    for (int idx = device_info.size(); idx < NUM_DEVICES; ++idx) {
      REQUIRE(hardware.getDevices().at(idx).getID() == devID);
      ++devID;
    }

    REQUIRE(hardware.getDevice(1).isNull());
  }

  SECTION("device_info, old_hw, and net_type") {
    Hardware old_hardware(device_info);
    const DevInfo GPU2(std::string("GPU2"), 1000, 2000, 8.59E9, 16);
    DevInfoVec add_hw(1, GPU2);

    const int NUM_DEVICES = device_info.size() + add_hw.size();

    std::vector<DevID> old_ids;
    for (const Device& dev : old_hardware.getDevices())
      old_ids.push_back(dev.getID());

    Hardware hardware(add_hw, old_hardware, NetworkType::FULL_CONN_GRAPH);

    REQUIRE(hardware.getNumDevices() == NUM_DEVICES);
    REQUIRE(hardware.getTopology().getNetworkType()
      == NetworkType::FULL_CONN_GRAPH);
    REQUIRE(hardware.getTopology().getNumDevices() == NUM_DEVICES);
    REQUIRE(hardware.getTopology().getNumLinks() == 0);

    std::vector<DevID> ids;
    for (size_t idx = 0; idx < device_info.size(); ++idx) {
      REQUIRE(old_ids.at(idx) == hardware.getDevices().at(idx).getID());
      ids.push_back(hardware.getDevices().at(idx).getID());
    }

    DevID devID = old_ids.back() + 1;
    for (int idx = device_info.size(); idx < NUM_DEVICES; ++idx) {
      REQUIRE(hardware.getDevices().at(idx).getID() == devID);
      ++devID;
    }

    REQUIRE(hardware.getDevice(1).isNull());
  }
}

TEST_CASE("BasicCostModel", "[unit]")
{
  typedef std::vector<std::tuple<std::string, Cost, Cost, double, unsigned int>>
    DevInfoVec;
  typedef std::tuple<std::string, Cost, Cost, double, unsigned int> DevInfo;

  const DevInfo L1(std::string("L1"), 1, 2, 32768, 512);
  const DevInfo L2(std::string("L2"), 10, 20, 262144, 0);
  const DevInfo L3(std::string("L3"), 100, 200, 2.097e6, 0);
  const DevInfo DRAM(std::string("DRAM"), 10000, 20000, 6.872E10, 0);
  const DevInfo GPU(std::string("GPU"), 1000, 2000, 8.59E9, 16);

  DevInfoVec device_info( { L1, L2, L3, DRAM, GPU } );
  const size_t NUM_DEVICES = device_info.size();

  BasicCostModel model(device_info);

  SECTION("check hardware") {
    const Hardware& hw = model.getHardware();
    REQUIRE(hw.getNumDevices() == NUM_DEVICES);

    const std::vector<Device>& DEVICES = hw.getDevices();
    int idx = 0;
    for (const auto DEV : DEVICES) {
      REQUIRE_THAT(DEV.getName(),
        Catch::Equals(std::get<0>(device_info.at(idx))));
        ++idx;
    }
  }

  SECTION("known_data_layouts") {
    const DataLayout& BYTE = model.getDataLayout(std::string("CM_BYTE"));
    REQUIRE_THAT(BYTE.getName(), Catch::Equals(std::string("CM_BYTE")));

    const std::string STR_DBL("DL_DOUBLE");
    AccessPattern ap_dbl(1, std::make_pair(AccessType::BASIC, 8));

    model.addDataLayout(STR_DBL, 8, ap_dbl);
    REQUIRE_THAT(model.getDataLayout(STR_DBL).getName(),Catch::Equals(STR_DBL));

    model.rmDataLayout(STR_DBL);
    REQUIRE_THROWS_AS(model.getDataLayout(STR_DBL), std::out_of_range);
  }

  SECTION("zero costs") {
    const Cost FREE_BEER = 0;
    DevID id_DRAM;
    AccessPattern ap_free(1, std::make_pair(AccessType::FREE, 1));
    const DataLayout DL_FREE(std::string("DL_FREE"), 1, ap_free);

    for (const auto DEV : model.getHardware().getDevices()) {
      if (!DEV.getName().compare("DRAM")) id_DRAM = DEV.getID();
    }

    REQUIRE(model.accessCost(id_DRAM, DL_FREE, ap_free, 1) == FREE_BEER);
  }
}
