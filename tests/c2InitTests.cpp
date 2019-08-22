#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Device.h"

using namespace CostModel;

TEST_CASE("Device", "[initialisation]")
{
  SECTION("NULL DEVICE") {
    const Device DEV_NULL(NULL);
    REQUIRE(DEV_NULL.isNull());
  }
}
