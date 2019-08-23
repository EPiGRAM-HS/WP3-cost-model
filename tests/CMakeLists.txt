add_executable(
  c2InitTests c2UnitTests.cpp
)

target_link_libraries(c2UnitTests Catch2::Catch2 costmodel)
ParseAndAddCatchTests(c2UnitTests)
