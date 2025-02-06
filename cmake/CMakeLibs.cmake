include(FetchContent)

function(fetch_openxlsx)
    if (NOT TARGET OpenXLSX)
        message(STATUS "Fetching openxlsx")
        FetchContent_Declare(
                OpenXLSX
                GIT_REPOSITORY https://github.com/troldal/OpenXLSX.git
                GIT_TAG master
                GIT_PROGRESS TRUE # Show progress
                GIT_SHALLOW TRUE # Only fetch the latest commit
        )
        FetchContent_MakeAvailable(OpenXLSX)
    else()
        message(WARNING "No target gtest found!")
    endif()
endfunction()