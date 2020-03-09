cmake_minimum_required (VERSION 3.10.2)
project (system-monitor-gui)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

find_package (Qt5 COMPONENTS Core Gui Network Widgets REQUIRED)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)

qt5_wrap_cpp (redistorium_MOC
    ${CMAKE_CURRENT_LIST_DIR}/../external/redistorium/include/redistorium/redis.hpp
    ${CMAKE_CURRENT_LIST_DIR}/../external/redistorium/include/redistorium/networking.hpp
)

add_executable (${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/../external/open62541/build/open62541.c
    ${redistorium_MOC}
    adapters/OpcuaClient.cpp
    adapters/OpcuaClient.h

    devices/IDevice.h
    devices/IDevice.cpp
    devices/OpcuaDevice.cpp
    devices/OpcuaDevice.h

    devices/skills/ISkill.h

    DeviceTabWidget.cpp
    DeviceTabWidget.h
    DeviceWidget.cpp
    DeviceWidget.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    OrderWidget.cpp
    OrderWidget.h
    OrderWidget.ui
    redisclient.cpp
    redisclient.h
    smgCommon.h
)

set_target_properties (${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    LINKER_LANGUAGE CXX
)

target_include_directories (${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../external/json/include
    ${CMAKE_CURRENT_LIST_DIR}/../external/open62541/build
    ${CMAKE_CURRENT_LIST_DIR}/../external/redistorium/include
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
)

if(WIN32)
    set(PLATFORM_SPECIFIC_LIBS Ws2_32)
endif(WIN32)

if(UNIX)
    set(PLATFORM_SPECIFIC_LIBS "")
endif(UNIX)

target_link_libraries (${PROJECT_NAME}
    ${PLATFORM_SPECIFIC_LIBS}
    redistorium
    Qt5::Core Qt5::Widgets
)

if(WIN32)
    if (CMAKE_GENERATOR MATCHES "Visual Studio")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${redistorium_BINARY_DIR}/Release/redistorium.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    else() # the ninja generator also uses msvc, but no build type specific folder
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${redistorium_BINARY_DIR}/redistorium.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endif()

    add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Gui_DIR}/../../../bin/Qt5Guid.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Network_DIR}/../../../bin/Qt5Networkd.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Widgets_DIR}/../../../bin/Qt5Widgetsd.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Core_DIR}/../../../bin/Qt5Core.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Gui_DIR}/../../../bin/Qt5Gui.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Network_DIR}/../../../bin/Qt5Network.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Widgets_DIR}/../../../bin/Qt5Widgets.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif(WIN32)
