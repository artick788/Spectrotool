cmake_minimum_required(VERSION 3.16)
project(Spectrotool)

include(SyriusBuildUtils/CMakeUtils.cmake)
include(cmake/CMakeLibs.cmake)

fetch_openxlsx()
fetch_syriuscore()

set(SPECTROTOOL_HEADERS
    include/Spectrotool/Compound.hpp
    include/Spectrotool/Defs.hpp
    include/Spectrotool/Worksheet.hpp
)

set(SPECTROTOOL_SRC
    src/json.hpp
    src/Compound.cpp
    src/Worksheet.cpp
)

add_library(Spectrotool STATIC ${SPECTROTOOL_SRC} ${SPECTROTOOL_HEADERS})
target_include_directories(Spectrotool PUBLIC include)
target_link_libraries(Spectrotool OpenXLSX)

set(SPECTROTOOL_APP_SRC
        SpectrotoolApp/SpectrotoolApp.cpp
        SpectrotoolApp/SpectrotoolApp.hpp
        SpectrotoolApp/SpectrotoolAppInclude.hpp)

add_executable(SpectrotoolApp main.cpp ${SPECTROTOOL_APP_SRC})
target_link_libraries(SpectrotoolApp Spectrotool SyriusCore)

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