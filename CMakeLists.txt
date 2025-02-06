cmake_minimum_required(VERSION 3.16)
project(Spectrotool)

include(SyriusBuildUtils/CMakeUtils.cmake)

set(SPECTROTOOL_SRC
)

add_executable(Spectrotool main.cpp ${SPECTROTOOL_SRC})

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTS)
    # Add GTest
    fetch_gtest()
    enable_testing()

    message(STATUS "Building tests")

    add_executable(SpectrotoolTest ${SPECTROTOOL_SRC})
    add_dependencies(SpectrotoolTest gtest)

    target_link_libraries(SpectrotoolTest GTest::gtest GTest::gtest_main)

    add_test(NAME CTestSpectrotool COMMAND ${PROOT} $<TARGET_FILE:SpectrotoolTest>)
    set_tests_properties(CTestSpectrotool PROPERTIES
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif ()