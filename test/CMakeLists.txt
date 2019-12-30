cmake_minimum_required (VERSION 3.13.4)
project (unit-test)

add_executable (unit-test
    main.cpp
    ../source/orderinformation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../external/googletest/googletest/src/gtest-all.cc
    ${CMAKE_CURRENT_LIST_DIR}/../external/googletest/googlemock/src/gmock-all.cc
)

target_include_directories(unit-test
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../external/json/include
    ${CMAKE_CURRENT_LIST_DIR}/../external/open62541/include
    ${CMAKE_CURRENT_LIST_DIR}/../external/redistorium/include
    ${CMAKE_CURRENT_LIST_DIR}/../source
    ${CMAKE_CURRENT_LIST_DIR}/../external/googletest/googletest
    ${CMAKE_CURRENT_LIST_DIR}/../external/googletest/googletest/include
    ${CMAKE_CURRENT_LIST_DIR}/../external/googletest/googlemock
    ${CMAKE_CURRENT_LIST_DIR}/../external/googletest/googlemock/include
    ${Qt5Core_INCLUDE_DIRS}
)

SET_PROPERTY (TARGET unit-test PROPERTY CXX_STANDARD 17)
SET_PROPERTY (TARGET unit-test PROPERTY LINKER_LANGUAGE CXX)

find_package(Qt5 COMPONENTS Core Network REQUIRED)
set(CMAKE_AUTOMOC ON)

if(WIN32)
    set(PLATFORM_SPECIFIC_LIBS "")
endif(WIN32)

if(UNIX)
    set(PLATFORM_SPECIFIC_LIBS pthread)
endif(UNIX)

target_link_libraries (unit-test
    redistorium

    ${PLATFORM_SPECIFIC_LIBS}
    Qt5::Core Qt5::Network
)
