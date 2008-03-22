TEMPLATE = app
TARGET = AnnoTool
CONFIG -= debug
UI_DIR = ./src/uiStuff/gen
INCLUDEPATH += ./src/uiStuff/include ./uiStuff/gen
QT += core \
    gui \
    xml
HEADERS += ./src/uiStuff/include/annotoolmainwindow.h
SOURCES += ./src/uiStuff/annotoolmainwindow.cpp \
    ./src/main.cpp
FORMS += ./src/uiStuff/forms/annotoolmainwindow.ui
RESOURCES += 
