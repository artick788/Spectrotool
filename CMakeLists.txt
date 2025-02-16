cmake_minimum_required(VERSION 3.16)
project(Spectrotool)

include(SyriusBuildUtils/CMakeUtils.cmake)
include(cmake/CMakeLibs.cmake)

fetch_openxlsx()
fetch_syriuscore()

set(SPECTROTOOL_HEADERS
    include/Spectrotool/Compound.hpp
    include/Spectrotool/Defs.hpp
    include/Spectrotool/MassSpecFile.hpp
)

set(SPECTROTOOL_SRC
    src/json.hpp
    src/Compound.cpp
    src/MassSpecFile.cpp
)

add_library(Spectrotool STATIC ${SPECTROTOOL_SRC} ${SPECTROTOOL_HEADERS})
target_include_directories(Spectrotool PUBLIC include)
target_link_libraries(Spectrotool OpenXLSX)

set(SPECTROTOOL_APP_SRC
    SpectrotoolApp/SpectrotoolApp.cpp
    SpectrotoolApp/SpectrotoolApp.hpp
    SpectrotoolApp/SpectrotoolAppInclude.hpp
    SpectrotoolApp/Panels/Panel.cpp
    SpectrotoolApp/Panels/Panel.hpp
    SpectrotoolApp/Store.cpp
    SpectrotoolApp/Store.hpp
    SpectrotoolApp/Panels/MenuPanel.cpp
    SpectrotoolApp/Panels/MenuPanel.hpp
    SpectrotoolApp/Panels/FilePanel.cpp
    SpectrotoolApp/Panels/FilePanel.hpp
    SpectrotoolApp/Worker.cpp
    SpectrotoolApp/Worker.hpp
    SpectrotoolApp/Project.cpp
    SpectrotoolApp/Project.hpp
    SpectrotoolApp/Panels/ProjectPanel.cpp
    SpectrotoolApp/Panels/ProjectPanel.hpp)

add_executable(SpectrotoolApp main.cpp ${SPECTROTOOL_APP_SRC})
target_link_libraries(SpectrotoolApp Spectrotool SyriusCore)

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTS)
    # Add GTest
    fetch_gtest()
    enable_testing()

    message(STATUS "Building tests")

    add_executable(SpectrotoolTest ${SPECTROTOOL_SRC}
            test/TestMassSpecFile.cpp
            test/TestMassSpecFile.hpp)
    add_dependencies(SpectrotoolTest gtest)

    target_link_libraries(SpectrotoolTest GTest::gtest GTest::gtest_main OpenXLSX)

    add_test(NAME CTestSpectrotool COMMAND ${PROOT} $<TARGET_FILE:SpectrotoolTest>)
    set_tests_properties(CTestSpectrotool PROPERTIES
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif ()