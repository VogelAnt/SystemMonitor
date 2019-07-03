#-------------------------------------------------
#
# Project created by QtCreator 2019-06-16T10:38:33
#
#-------------------------------------------------

QT += core gui network widgets

TARGET = system-monitor-gui
TEMPLATE = app

INCLUDEPATH += \
$$PWD/external/redistorium/include \
$$PWD/external/json/include

LIBS += \
-L$$PWD/external/redistorium/build/redistorium.lib/ -lredistorium \
-L$$PWD/external/json/

CONFIG += c++1z

SOURCES += \
$$files(source/*.cpp)

HEADERS += \
$$files(source/*.h)

FORMS += \
$$files(source/*.ui)

DESTDIR = $$PWD/build
MOC_DIR = $$DESTDIR/moc
OBJECTS_DIR = $$DESTDIR/obj
UI_DIR = $$DESTDIR/ui
