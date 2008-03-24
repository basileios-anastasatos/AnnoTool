TEMPLATE = app
TARGET = AnnoTool
CONFIG -= debug
UI_DIR = ./src/uiStuff/gen
INCLUDEPATH += ./src/uiStuff/include \
    ./src/uiStuff/gen \
    ./src/annoDataTypes/include \
    ./src/annoExceptions/include \
    ./src/annoGraphics/include
QT += core \
    gui \
    xml
HEADERS += ./src/annoHelper/logging/include/GlobalLogger.h \
    ./src/annoHelper/logging/include/SingleFileLogger.h \
    ./src/annoHelper/logging/include/MultiLogger.h \
    ./src/annoHelper/logging/include/DummyLogger.h \
    ./src/annoHelper/logging/include/ConsoleLogger.h \
    ./src/annoHelper/logging/include/Logger.h \
    ./src/uiStuff/include/zoomcontrol.h \
    ./src/annoExceptions/include/AllAnnoExceptions.h \
    ./src/annoGraphics/include/AnnoGraphicsControlPoint.h \
    ./src/annoGraphics/include/AnnoGraphicsPixmap.h \
    ./src/annoGraphics/include/AnnoGraphicsPolygon.h \
    ./src/annoDataTypes/include/AnnoClassDefinition.h \
    ./src/annoDataTypes/include/AnnoClassList.h \
    ./src/annoExceptions/include/AnnoException.h \
    ./src/annoExceptions/include/IOException.h \
    ./src/annoExceptions/include/NameConflictException.h \
    ./src/annoExceptions/include/NoSuchElementException.h \
    ./src/annoExceptions/include/OutOfRangeException.h \
    ./src/annoExceptions/include/XmlException.h \
    ./src/annoExceptions/include/XmlFormatException.h \
    ./src/uiStuff/include/annotoolmainwindow.h
SOURCES += ./src/annoHelper/logging/GlobalLogger.cpp \
    ./src/annoHelper/logging/SingleFileLogger.cpp \
    ./src/annoHelper/logging/MultiLogger.cpp \
    ./src/annoHelper/logging/DummyLogger.cpp \
    ./src/annoHelper/logging/ConsoleLogger.cpp \
    ./src/annoHelper/logging/Logger.cpp \
    ./src/uiStuff/zoomcontrol.cpp \
    ./src/annoGraphics/AnnoGraphicsControlPoint.cpp \
    ./src/annoGraphics/AnnoGraphicsPixmap.cpp \
    ./src/annoGraphics/AnnoGraphicsPolygon.cpp \
    ./src/annoDataTypes/AnnoClassDefinition.cpp \
    ./src/annoDataTypes/AnnoClassList.cpp \
    ./src/annoExceptions/AnnoException.cpp \
    ./src/annoExceptions/IOException.cpp \
    ./src/annoExceptions/NameConflictException.cpp \
    ./src/annoExceptions/NoSuchElementException.cpp \
    ./src/annoExceptions/OutOfRangeException.cpp \
    ./src/annoExceptions/XmlException.cpp \
    ./src/annoExceptions/XmlFormatException.cpp \
    ./src/uiStuff/annotoolmainwindow.cpp \
    ./src/main.cpp
FORMS += ./src/uiStuff/forms/zoomcontrol.ui \
    ./src/uiStuff/forms/annotoolmainwindow.ui
RESOURCES += ./src/uiStuff/forms/icons.qrc
