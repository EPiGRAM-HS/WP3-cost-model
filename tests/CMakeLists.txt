add_executable(
  c2InitTests c2InitTests.cpp
)

target_link_libraries(c2InitTests Catch2::Catch2 costmodel)
ParseAndAddCatchTests(c2InitTests)
