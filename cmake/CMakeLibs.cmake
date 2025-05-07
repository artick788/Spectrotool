include(FetchContent)
include(SyriusBuildUtils/CMakeUtils.cmake)

function(fetch_openxlsx)
    fetch_lib(OpenXLSX https://github.com/troldal/OpenXLSX.git master)
endfunction()

function(fetch_syriuscore)
    fetch_lib(SyriusCore https://github.com/SyriusEngine/SyriusCore.git main)
endfunction()

function(fetch_syriusutils)
    fetch_lib(SyriusUtils https://github.com/SyriusEngine/SyriusUtils.git main)
endfunction()