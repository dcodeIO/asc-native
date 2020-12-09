cmake_minimum_required(VERSION 3.18)
project(asc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-int-conversion -Wno-unused-variable -Wno-unused-function -Wno-uninitialized")
add_executable(asc
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/assemblyscript.post.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/wtf.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/wabt/wasm2c/wasm-rt-impl.c"
)
target_include_directories(asc PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/binaryen/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/wabt/wasm2c"
)
find_library(BINARYEN_LIB binaryen PATHS
  "${CMAKE_CURRENT_SOURCE_DIR}/binaryen/build/lib"
  "${CMAKE_CURRENT_SOURCE_DIR}/binaryen/lib"
)
target_link_libraries(asc PRIVATE "${BINARYEN_LIB}")
set_target_properties(asc PROPERTIES
  COMPILE_FLAGS "-m32"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
