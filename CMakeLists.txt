cmake_minimum_required(VERSION 3.20)
project(dune-nd-det-systs)

if(CMAKE_INSTALL_PREFIX STREQUAL "" OR CMAKE_INSTALL_PREFIX STREQUAL
  "/usr/local")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}")
elseif(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}")
endif()


add_library(dune_nd_det_systs INTERFACE)

# --- CPM ---

file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.43.1/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# --- duneanaobj ---

if(NOT DUNE_ANAOBJ_BRANCH)
  set(DUNE_ANAOBJ_BRANCH v03_16_00)
endif()

find_package(duneanaobj QUIET)
if(NOT TARGET duneanaobj::StandardRecord) # need to fetch it
  include(FetchContent)
  FetchContent_Declare(
      duneanaobj
      GIT_REPOSITORY https://github.com/DUNE/duneanaobj.git
      GIT_TAG        ${DUNE_ANAOBJ_BRANCH}
    )
  set(SKIP_CET ON)
  FetchContent_MakeAvailable(duneanaobj)
endif()

target_include_directories(dune_nd_det_systs INTERFACE
  "${duneanaobj_SOURCE_DIR}")

if(TARGET duneanaobj::StandardRecord)
  target_link_libraries(dune_nd_det_systs INTERFACE
    duneanaobj::StandardRecord)
elseif(TARGET StandardRecord)
  target_link_libraries(dune_nd_det_systs INTERFACE
    StandardRecord)
else()
  message(FATAL_ERROR "Could not find a duneanaobj StandardRecord target")
endif()

# --- duneanasel ---

# CPMAddPackage(
#   NAME duneanasel
#   GITHUB_REPOSITORY DUNE/duneanasel
#   GIT_TAG           main
#   DOWNLOAD_ONLY     YES
# )

# target_include_directories(dune_nd_det_systs INTERFACE
#   "${duneanasel_SOURCE_DIR}")

# --- ROOT ---

find_package(ROOT 6 REQUIRED)

target_link_libraries(dune_nd_det_systs INTERFACE
  ROOT::Core
  ROOT::Tree
  ROOT::RIO
)

add_executable(test test.cpp)
target_link_libraries(test PUBLIC
  dune_nd_det_systs)