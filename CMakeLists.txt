cmake_minimum_required(VERSION 3.10.2)
project(system-monitor-distribution)

option(SMG_BUILD_TESTS "Build tests." ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (SMG_BUILD_TESTS)
        # add compiler and linker flags used for coverage report
        # this has to be done before adding any subproject
        set (GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
        set (GCC_COVERAGE_LINK_FLAGS "-coverage -lgcov")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
    endif (SMG_BUILD_TESTS)
endif ()

if (WIN32)
    set (CMAKE_PREFIX_PATH "C:/Qt/5.13.2/msvc2017_64")
endif(WIN32)

set (REDISTORIUM_BUILD_EXAMPLES OFF CACHE BOOL "Build examples." FORCE)
set (REDISTORIUM_BUILD_TESTS  OFF CACHE BOOL "Build tests." FORCE)
add_subdirectory(external/redistorium)

add_subdirectory(source)


if(SMG_BUILD_TESTS)
    add_subdirectory(test)
endif()
