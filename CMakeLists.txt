cmake_minimum_required(VERSION 3.13)
project(Transas)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_library(PCH inc/ProjectPCH.h src/ProjectPCH.cpp)
set_source_files_properties(src/ProjectPCH.cpp
        PROPERTIES COMPILE_FLAGS "-include ../inc/ProjectPCH.h")

set(GTEST gtest/gtest.h gtest/gtest_main.cc gtest/gtest-all.cc)
set(SOURCE src/ProjectPCH.cpp inc/Request.h inc/Stopper.h src/main.cpp)

add_executable(Transas inc/RequestManager.h inc/ProjectPCH.h ${SOURCE})
target_link_libraries(Transas PCH)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(Transas ${Boost_LIBRARIES})

add_executable(Testing task_testing.cpp ${GTEST} inc/RequestManager.h inc/ProjectPCH.h
        src/ProjectPCH.cpp inc/Request.h inc/Stopper.h )
target_link_libraries(Testing ${Boost_LIBRARIES})
