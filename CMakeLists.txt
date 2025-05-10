cmake_minimum_required(VERSION 3.16)
project(Spectrotool)

include(SyriusBuildUtils/CMakeUtils.cmake)
include(cmake/CMakeLibs.cmake)

fetch_openxlsx()
fetch_syriuscore()
fetch_syriusutils()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

set(SPECTROTOOL_HEADERS
    include/Spectrotool/Compound.hpp
    include/Spectrotool/Defs.hpp
        include/Spectrotool/SampleList.hpp
    include/Spectrotool/CorrectionFactor.hpp
        include/Spectrotool/DataTable.hpp
        include/Spectrotool/Parsers/DataTableParser.hpp
        include/Spectrotool/Parsers/ParserMessages.hpp
        include/Spectrotool/Parsers/SampleListParser.hpp
        include/Spectrotool/Parsers/CorrectionFactorParser.hpp
)

set(SPECTROTOOL_SRC
    include/Spectrotool/json.hpp
    src/Compound.cpp
        src/SampleList.cpp
    src/CorrectionFactor.cpp
    src/utils.cpp
    src/utils.hpp
        src/DataTable.cpp
        src/Parsers/DataTableParser.cpp
        src/Parsers/SampleListParser.cpp
        src/Parsers/CorrectionFactorParser.cpp
)

add_library(Spectrotool STATIC ${SPECTROTOOL_SRC} ${SPECTROTOOL_HEADERS})
target_include_directories(Spectrotool PUBLIC include)
target_link_libraries(Spectrotool OpenXLSX SyriusUtils)

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
        SpectrotoolApp/Panels/OverviewPanel.cpp
        SpectrotoolApp/Panels/OverviewPanel.hpp
    SpectrotoolApp/Worker.cpp
    SpectrotoolApp/Worker.hpp
        SpectrotoolApp/Panels/DataTablePanel.cpp
        SpectrotoolApp/Panels/DataTablePanel.hpp)

add_executable(SpectrotoolApp main.cpp ${SPECTROTOOL_APP_SRC})
target_link_libraries(SpectrotoolApp Spectrotool SyriusCore)

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTS)
    # Add GTest
    fetch_gtest()
    enable_testing()

    message(STATUS "Building tests")

    add_executable(SpectrotoolTest ${SPECTROTOOL_SRC}
            test/TestDataTable.cpp
            test/Parsers/TestDataTableParser.cpp
            test/Parsers/TestSampleListParser.cpp
            test/TestSampleList.cpp
            test/Parsers/TestCorrectionFactorParser.cpp)
    add_dependencies(SpectrotoolTest gtest)

    target_link_libraries(SpectrotoolTest GTest::gtest GTest::gtest_main OpenXLSX SyriusUtils)

    add_test(NAME CTestSpectrotool COMMAND ${PROOT} $<TARGET_FILE:SpectrotoolTest>)
    set_tests_properties(CTestSpectrotool PROPERTIES
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif ()