#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Device.h"
#include "Link.h"
#include "utils.h"
#include "DataLayout.h"

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
    const LinkID LID = CostModel::unorderedCantor(DEV_A, DEV_B);
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
