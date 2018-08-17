cmake_minimum_required(VERSION 3.11)
project(x3pcpp)
set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig) 
if (PKG_CONFIG_FOUND) 
    PKG_CHECK_MODULES(ZLIB zlib) 
endif (PKG_CONFIG_FOUND)

include_directories(${CMAKE_SOURCE_DIR}/lib/zipper/)
include_directories(${CMAKE_SOURCE_DIR}/lib/rapidxml)

add_executable(${PROJECT_NAME} src/x3pcpp.cpp src/x3pcpp.h)

target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
target_link_libraries(${PROJECT_NAME} stdc++fs)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/zipper/libZipper-static.a)
