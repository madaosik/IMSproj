cmake_minimum_required(VERSION 3.8)
project(IMS)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall")
link_libraries(-lsimlib)

set(SOURCE_FILES src/ims_ca.cpp src/model.cpp)
add_executable(IMS ${SOURCE_FILES})