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
        message(WARNING "No target openxlsx found!")
    endif()
endfunction()

function(fetch_syriuscore)
    if (NOT TARGET SyriusCore)
        message(STATUS "Fetching openxlsx")
        FetchContent_Declare(
                SyriusCore
                GIT_REPOSITORY https://github.com/SyriusEngine/SyriusCore.git
                GIT_TAG main
                GIT_PROGRESS TRUE # Show progress
                GIT_SHALLOW TRUE # Only fetch the latest commit
        )
        FetchContent_MakeAvailable(SyriusCore)
    else()
        message(WARNING "No target SyriusCore found!")
    endif()
endfunction()