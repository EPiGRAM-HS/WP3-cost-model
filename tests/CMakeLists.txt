add_executable(
  c2UnitTests c2UnitTests.cpp
)

target_link_libraries(c2UnitTests Catch2::Catch2 costmodel)
ParseAndAddCatchTests(c2UnitTests)
