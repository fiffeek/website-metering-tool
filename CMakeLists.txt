cmake_minimum_required(VERSION 3.10)
project(website_metering)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_LOG_DYN_LINK -pthread")
FIND_PACKAGE(Boost 1.65.1.0 COMPONENTS program_options filesystem log REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
add_subdirectory(src/dependencies/cpr)
include_directories(${CPR_INCLUDE_DIRS})
include_directories(/src)

add_executable(website_metering src/server.cpp src/ src/*/ src/database/logger.hpp)
TARGET_LINK_LIBRARIES(website_metering LINK_PUBLIC ${Boost_LIBRARIES} ${CPR_LIBRARIES})