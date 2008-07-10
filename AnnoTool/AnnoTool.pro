TEMPLATE = app
TARGET = AnnoTool
CONFIG += release
CONFIG -= exceptions_off \
    rtti_off \
    stl_off \
    debug
UI_DIR = ./src/uiStuff/gen
MOC_DIR = ./tmp/moc
RCC_DIR = ./tmp
INCLUDEPATH += ./src/annoDataTypes/include \
    ./src/annoExceptions/include \
    ./src/annoGraphics/include \
    ./src/annoHelper/include \
    ./src/annoHelper/logging/include \
    ./src/general/include \
    ./src/general/imexporter/include \
    ./src/uiStuff/include \
    ./src/uiStuff/gen \
    ./src/uiStuff/helper/include \
    ./src/libAnnotationDeprecated/include
QT += core \
    gui \
    xml
HEADERS += ./src/annoExceptions/include/FormatException.h \
    ./src/annoGraphics/include/ToolPolygon.h \
    ./src/annoDataTypes/include/AnnoPolygon.h \
    ./src/uiStuff/include/AnnoToolMainWindow.h \
    ./src/uiStuff/include/ZoomControl.h \
    ./src/annoGraphics/include/ToolEllipse.h \
    ./src/annoGraphics/include/AnnoGraphicsEllipse.h \
    ./src/annoDataTypes/include/AnnoEllipse.h \
    ./src/annoGraphics/include/ToolSinglePoint.h \
    ./src/uiStuff/include/DlgAbout.h \
    ./src/annoDataTypes/include/AnnoSinglePoint.h \
    ./src/annoGraphics/include/AnnoGraphicsSinglePoint.h \
    ./src/uiStuff/include/DlgEditAttribute.h \
    ./src/annoDataTypes/include/annoTypesForward.h \
    ./src/uiStuff/include/DlgImporter.h \
    ./src/uiStuff/include/DlgExporter.h \
    ./src/uiStuff/helper/include/AnnoDataClassesModelAdapter.h \
    ./src/uiStuff/helper/include/AnnoDataModelAdapter.h \
    ./src/general/imexporter/include/IdlExporterPlugin.h \
    ./src/general/imexporter/include/IdlImporterPlugin.h \
    ./src/uiStuff/include/DlgNewProject.h \
    ./src/libAnnotationDeprecated/include/libAn_Annotation.h \
    ./src/libAnnotationDeprecated/include/libAn_AnnotationList.h \
    ./src/libAnnotationDeprecated/include/libAn_AnnoRect.h \
    ./src/libAnnotationDeprecated/include/libAn_XmlHelpers.h \
    ./src/general/include/ExporterPlugin.h \
    ./src/general/include/ImporterPlugin.h \
    ./src/general/include/GlobalImExportManager.h \
    ./src/annoGraphics/include/AllGraphicsTools.h \
    ./src/annoGraphics/include/AllAnnoGraphics.h \
    ./src/general/include/importGlobals.h \
    ./src/general/include/GlobalToolManager.h \
    ./src/annoGraphics/include/ToolRect.h \
    ./src/annoGraphics/include/ToolHand.h \
    ./src/annoGraphics/include/ToolPointer.h \
    ./src/annoGraphics/include/GraphicsTool.h \
    ./src/annoGraphics/include/MapperHelper.h \
    ./src/general/include/GlobalConfig.h \
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
    ./src/annoExceptions/include/XmlFormatException.h
SOURCES += ./src/annoExceptions/FormatException.cpp \
    ./src/annoGraphics/ToolPolygon.cpp \
    ./src/annoDataTypes/AnnoPolygon.cpp \
    ./src/uiStuff/AnnoToolMainWindow.cpp \
    ./src/uiStuff/ZoomControl.cpp \
    ./src/annoGraphics/ToolEllipse.cpp \
    ./src/annoGraphics/AnnoGraphicsEllipse.cpp \
    ./src/annoDataTypes/AnnoEllipse.cpp \
    ./src/annoGraphics/ToolSinglePoint.cpp \
    ./src/uiStuff/DlgAbout.cpp \
    ./src/annoDataTypes/AnnoSinglePoint.cpp \
    ./src/annoGraphics/AnnoGraphicsSinglePoint.cpp \
    ./src/uiStuff/DlgEditAttribute.cpp \
    ./src/uiStuff/DlgImporter.cpp \
    ./src/uiStuff/DlgExporter.cpp \
    ./src/uiStuff/helper/AnnoDataClassesModelAdapter.cpp \
    ./src/uiStuff/helper/AnnoDataModelAdapter.cpp \
    ./src/general/imexporter/IdlExporterPlugin.cpp \
    ./src/general/imexporter/IdlImporterPlugin.cpp \
    ./src/uiStuff/DlgNewProject.cpp \
    ./src/libAnnotationDeprecated/libAn_XmlHelpers.cpp \
    ./src/libAnnotationDeprecated/libAn_Annotation.cpp \
    ./src/libAnnotationDeprecated/libAn_AnnotationList.cpp \
    ./src/libAnnotationDeprecated/libAn_AnnoRect.cpp \
    ./src/general/GlobalImExportManager.cpp \
    ./src/general/GlobalToolManager.cpp \
    ./src/annoGraphics/ToolRect.cpp \
    ./src/annoGraphics/ToolHand.cpp \
    ./src/annoGraphics/ToolPointer.cpp \
    ./src/annoGraphics/GraphicsTool.cpp \
    ./src/annoGraphics/MapperHelper.cpp \
    ./src/general/GlobalConfig.cpp \
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
    ./src/main.cpp
FORMS += ./src/uiStuff/forms/AnnoToolMainWindow.ui \
    ./src/uiStuff/forms/ZoomControl.ui \
    ./src/uiStuff/forms/DlgAbout.ui \
    ./src/uiStuff/forms/DlgEditAttribute.ui \
    ./src/uiStuff/forms/DlgImporter.ui \
    ./src/uiStuff/forms/DlgExporter.ui \
    ./src/uiStuff/forms/DlgNewProject.ui \
    ./src/uiStuff/forms/AnnoDataWidget.ui \
    ./src/uiStuff/forms/AnnoListWidget.ui \
    ./src/uiStuff/forms/DlgLoaderDetails.ui \
    ./src/uiStuff/forms/DlgAddImage.ui \
    ./src/uiStuff/forms/AnnoFileListWidget.ui \
    ./src/uiStuff/forms/DlgFileChooser.ui \
    ./src/uiStuff/forms/DlgProjectDetails.ui
RESOURCES += ./src/uiStuff/forms/cursors.qrc \
    ./src/uiStuff/forms/others.qrc \
    ./src/uiStuff/forms/icons.qrc
win32 { 
    release { 
        DESTDIR = ./bin/win32/release
        OBJECTS_DIR = ./bin/win32/release/obj
    }
    debug { 
        DESTDIR = ./bin/win32/debug
        OBJECTS_DIR = ./bin/win32/debug/obj
    }
}
unix { 
    release { 
        DESTDIR = ./bin/unix/release
        OBJECTS_DIR = ./bin/unix/release/obj
    }
    debug { 
        DESTDIR = ./bin/unix/debug
        OBJECTS_DIR = ./bin/unix/debug/obj
    }
}
macx { 
    release { 
        DESTDIR = ./bin/macOSX/release
        OBJECTS_DIR = ./bin/macOSX/release/obj
    }
    debug { 
        DESTDIR = ./bin/macOSX/debug
        OBJECTS_DIR = ./bin/macOSX/debug/obj
    }
}
