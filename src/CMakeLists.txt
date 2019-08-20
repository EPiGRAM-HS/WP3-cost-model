add_library(costmodel STATIC Access.cpp Device.cpp Hardware.cpp Topology.cpp)

target_compile_features(costmodel PUBLIC cxx_std_17)

# clang/GCC warnings
target_compile_options(costmodel
  PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall>
)

target_include_directories(costmodel
  PUBLIC ${CMAKE_SOURCE_DIR}/include ${LEMON_INCLUDE_DIR}
)
