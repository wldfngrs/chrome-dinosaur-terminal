function(CompileStandardLibraryHeaderUnit header)

    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        execute_process(COMMAND ${CMAKE_CXX_COMPILER} -std=c++20 -fmodules-ts -x c++-system-header ${header})
        message("GCC Created Header Unit for => ${header}")

    elseif(${CMAKE_CXX_COMPILER_ID} MATCHES ".*Clang")
        execute_process(COMMAND ${CMAKE_CXX_COMPILER} "-std=c++20 --precompile -x c++-system-header ${header}")

    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
        execute_process(COMMAND ${CMAKE_CXX_COMPILER} /std:c++latest /exportHeader /headerName:angle ${header} OUTPUT_VARIABLE cout)
    endif()

    message("Compiled Header Unit for => ${header} \n ${cout} \n")

endfunction()

function(CreateMsvcCompilerFlags headername)
target_compile_options(project_options INTERFACE /headerUnit:angle "${headername}=./${headername}.ifc")
endfunction()

set(SystemHeaders iostream vector memory algorithm iterator string numeric ranges string_view map charconv array sstream cmath cstdint cstring limits utility stdexcept system_error bit)

foreach(headername ${SystemHeaders})
    CompileStandardLibraryHeaderUnit(${headername})
endforeach()

#add -fmodule-ts for gcc
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
target_compile_options(project_options INTERFACE -fmodules-ts -flang-info-include-translate)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
target_compile_options(project_options INTERFACE "/translateInclude")
    foreach(headername ${SystemHeaders})
        CreateMsvcCompilerFlags(${headername}) 
    endforeach()
endif()
