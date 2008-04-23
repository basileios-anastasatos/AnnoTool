TEMPLATE = app
TARGET = AnnoTool
CONFIG -= debug
UI_DIR = ./src/uiStuff/gen
INCLUDEPATH += ./src/uiStuff/include \
    ./src/uiStuff/helper/include \
    ./src/uiStuff/gen \
    ./src/annoDataTypes/include \
    ./src/annoExceptions/include \
    ./src/annoGraphics/include \
    ./src/annoHelper/include \
    ./src/annoHelper/logging/include \
    ./src/general/include
QT += core \
    gui \
    xml \
    qt3support
HEADERS += ./src/general/include/GlobalConfig.h \
    ./src/annoGraphics/include/AnnoGraphicsScene.h \
    ./src/annoGraphics/include/AnnoGraphicsRect.h \
    ./src/uiStuff/include/AnnoDataWidget.h \
    ./src/uiStuff/helper/include/AnnoListModelAdapter.h \
    ./src/uiStuff/helper/include/AnnoFileListModelAdapter.h \
    ./src/uiStuff/include/AnnoListWidget.h \
    ./src/uiStuff/include/DlgLoaderDetails.h \
    ./src/uiStuff/include/DlgAddImage.h \
    ./src/general/include/ImageLoaderPlugin.h \
    ./src/general/include/GlobalImageLoader.h \
    ./src/uiStuff/include/AnnoFileListWidget.h \
    ./src/uiStuff/include/DlgFileChooser.h \
    ./src/uiStuff/include/DlgProjectDetails.h \
    ./src/annoExceptions/include/IllegalStateException.h \
    ./src/annoDataTypes/include/AnnoAvClassList.h \
    ./src/general/include/GlobalProjectManager.h \
    ./src/uiStuff/include/dlgnewproject.h \
    ./src/annoDataTypes/include/AnnoFileData.h \
    ./src/annoDataTypes/include/AnnoRectangle.h \
    ./src/annoGraphics/include/AnnoGraphicsShapeCreator.h \
    ./src/annoGraphics/include/AnnoGraphicsShape.h \
    ./src/annoDataTypes/include/AnnoShape.h \
    ./src/annoDataTypes/include/AnnoInfo.h \
    ./src/annoDataTypes/include/AnnoImageInfo.h \
    ./src/annoDataTypes/include/Annotation.h \
    ./src/annoHelper/include/XmlHelper.h \
    ./src/annoDataTypes/include/AnnoProject.h \
    ./src/annoHelper/logging/include/GlobalLogger.h \
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
SOURCES += ./src/general/GlobalConfig.cpp \
    ./src/annoGraphics/AnnoGraphicsScene.cpp \
    ./src/annoGraphics/AnnoGraphicsRect.cpp \
    ./src/uiStuff/AnnoDataWidget.cpp \
    ./src/uiStuff/helper/AnnoListModelAdapter.cpp \
    ./src/uiStuff/helper/AnnoFileListModelAdapter.cpp \
    ./src/uiStuff/AnnoListWidget.cpp \
    ./src/uiStuff/DlgLoaderDetails.cpp \
    ./src/uiStuff/DlgAddImage.cpp \
    ./src/general/GlobalImageLoader.cpp \
    ./src/uiStuff/AnnoFileListWidget.cpp \
    ./src/uiStuff/DlgFileChooser.cpp \
    ./src/uiStuff/DlgProjectDetails.cpp \
    ./src/annoExceptions/IllegalStateException.cpp \
    ./src/annoDataTypes/AnnoAvClassList.cpp \
    ./src/general/GlobalProjectManager.cpp \
    ./src/uiStuff/dlgnewproject.cpp \
    ./src/annoDataTypes/AnnoFileData.cpp \
    ./src/annoDataTypes/AnnoRectangle.cpp \
    ./src/annoGraphics/AnnoGraphicsShapeCreator.cpp \
    ./src/annoGraphics/AnnoGraphicsShape.cpp \
    ./src/annoDataTypes/AnnoShape.cpp \
    ./src/annoDataTypes/AnnoInfo.cpp \
    ./src/annoDataTypes/AnnoImageInfo.cpp \
    ./src/annoDataTypes/Annotation.cpp \
    ./src/annoHelper/XmlHelper.cpp \
    ./src/annoDataTypes/AnnoProject.cpp \
    ./src/annoHelper/logging/GlobalLogger.cpp \
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
FORMS += ./src/uiStuff/forms/AnnoDataWidget.ui \
    ./src/uiStuff/forms/AnnoListWidget.ui \
    ./src/uiStuff/forms/DlgLoaderDetails.ui \
    ./src/uiStuff/forms/DlgAddImage.ui \
    ./src/uiStuff/forms/AnnoFileListWidget.ui \
    ./src/uiStuff/forms/DlgFileChooser.ui \
    ./src/uiStuff/forms/DlgProjectDetails.ui \
    ./src/uiStuff/forms/dlgnewproject.ui \
    ./src/uiStuff/forms/zoomcontrol.ui \
    ./src/uiStuff/forms/annotoolmainwindow.ui
RESOURCES += ./src/uiStuff/forms/icons.qrc
