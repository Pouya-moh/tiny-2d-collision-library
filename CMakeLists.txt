cmake_minimum_required(VERSION 3.14)
project(2D_GeometricObstacle)

set(CMAKE_CXX_STANDARD 14)

if(EXISTS "${PROJECT_SOURCE_DIR}/googletest/CMakeLists.txt")
    add_subdirectory(googletest)
    add_executable(tiny_2d_collision_test
            test/main.cpp
            )
    target_include_directories(tiny_2d_collision_test PRIVATE src)
    target_link_libraries(tiny_2d_collision_test gtest_main)

    message(STATUS "Googletest cloned. Will also create tests")
else()
    message(NOTICE "Googletest not cloned. Tests will be omitted!")
endif()
