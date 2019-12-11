TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QT += core

INCLUDEPATH += \
$$PWD/../source/ \
$$PWD/../external/json/include \
$$PWD/../external/opc-ua-server-c/include \
$$PWD/../external/open62541 \
$$PWD/../external/googletest/googletest \
$$PWD/../external/googletest/googlemock \
$$PWD/../external/googletest/googletest/include \
$$PWD/../external/googletest/googlemock/include \
$$PWD/../external/googletest/googletest/include/gtest \
$$PWD/../external/googletest/googlemock/include/gmock

LIBS += \
-L$$PWD/../external/json/

CONFIG += c++1z

HEADERS += \
$$files($$PWD/../external/googletest/googletest/include/gtest/*.h) \
$$files($$PWD/../external/googletest/googlemock/include/gmock/*.h) \
$$PWD/../source/OrderInformation.h \
$$files($$PWD/*.h)

SOURCES += \
$$PWD/../external/open62541/open62541.c \
$$files($$PWD/../external/opc-ua-server-c/source/*.cpp) \
$$PWD/../source/OrderInformation.cpp \
$$PWD/../external/googletest/googletest/src/gtest-all.cc \
$$PWD/../external/googletest/googlemock/src/gmock-all.cc \
main.cpp
