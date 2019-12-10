cmake_minimum_required(VERSION 3.11)
project(x3pcpp)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/lib/zipper/)
include_directories(${CMAKE_SOURCE_DIR}/lib/rapidxml)

add_executable(${PROJECT_NAME} src/x3pcpp.cpp src/x3pcpp.h src/main.cpp)

target_link_libraries(${PROJECT_NAME}
        z
        stdc++fs
        opencv_core
        opencv_highgui
        opencv_imgcodecs
        ${CMAKE_SOURCE_DIR}/lib/zipper/libZipper-static.a
)
