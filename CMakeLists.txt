cmake_minimum_required(VERSION 3.16)
project(Spectrotool)

include(SyriusBuildUtils/CMakeUtils.cmake)
include(cmake/CMakeLibs.cmake)

fetch_openxlsx()

set(SPECTROTOOL_SRC
    src/json.hpp
    src/Compound.cpp
    src/Compound.hpp
    src/Defs.hpp
    src/Worksheet.cpp
    src/Worksheet.hpp
)

add_executable(Spectrotool main.cpp ${SPECTROTOOL_SRC})
target_link_libraries(Spectrotool OpenXLSX)

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTS)
    # Add GTest
    fetch_gtest()
    enable_testing()

    message(STATUS "Building tests")

    add_executable(SpectrotoolTest ${SPECTROTOOL_SRC}
        test/TestWorkSheet.cpp
        test/TestWorkSheet.hpp)
    add_dependencies(SpectrotoolTest gtest)

    target_link_libraries(SpectrotoolTest GTest::gtest GTest::gtest_main OpenXLSX)

    add_test(NAME CTestSpectrotool COMMAND ${PROOT} $<TARGET_FILE:SpectrotoolTest>)
    set_tests_properties(CTestSpectrotool PROPERTIES
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif ()