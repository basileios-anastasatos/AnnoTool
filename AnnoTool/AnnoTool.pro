TEMPLATE += app
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
    ./src/annoFilter/include \
    ./src/annoExceptions/include \
    ./src/annoGraphics/include \
    ./src/annoHelper/include \
    ./src/annoHelper/logging/include \
    ./src/utils/include \
    ./src/general/include \
    ./src/general/imexporter/include \
    ./src/uiStuff/include \
    ./src/uiStuff/gen \
    ./src/uiStuff/helper/include \
    ./src/libAnnotationDeprecated/include
QT += core \
    gui \
    opengl \
    xml
HEADERS += ./src/annoFilter/include/annoFiltersForward.h \
    ./src/uiStuff/include/AnnoGraphicsView.h \
    ./src/uiStuff/include/FilterControl.h \
    ./src/uiStuff/helper/include/AnnoGLView.h \
    ./src/uiStuff/helper/include/AnnoFilterHighlighter.h \
    ./src/uiStuff/helper/include/AnnoFiltersModelAdapter.h \
    ./src/annoFilter/include/AnnoFilterManager.h \
    ./src/uiStuff/include/DlgFilterEdit.h \
    ./src/annoFilter/include/AfrSwitch.h \
    ./src/annoFilter/include/AllAnnoFilterRules.h \
    ./src/annoFilter/include/AnnoFilterXmlLoader.h \
    ./src/annoFilter/include/AfrScore.h \
    ./src/annoFilter/include/AfrClass.h \
    ./src/annoFilter/include/AfrAttributeValue.h \
    ./src/annoFilter/include/LogicFilterRule.h \
    ./src/annoFilter/include/AnnoFilterRuleAtom.h \
    ./src/annoFilter/include/AfrXor.h \
    ./src/annoFilter/include/AfrNot.h \
    ./src/annoFilter/include/AfrOr.h \
    ./src/annoFilter/include/AfrAnd.h \
    ./src/annoFilter/include/AnnoFilter.h \
    ./src/annoFilter/include/AnnoFilterRule.h \
    ./src/annoDataTypes/include/AnnoFileListSorter.h \
    ./src/utils/include/PriorityList.h \
    ./src/utils/include/SortedList.h \
    ./src/uiStuff/helper/include/RecentAttrValues.h \
    ./src/uiStuff/include/DlgEditAttributeShort.h \
    ./src/uiStuff/include/ShapeContextMenu.h \
    ./src/uiStuff/include/DlgEditClass.h \
    ./src/annoExceptions/include/NullPointerException.h \
    ./src/annoDataTypes/include/AnnoOperationHelper.h \
    ./src/uiStuff/include/DlgIdlExporter.h \
    ./src/general/include/GlobalInfo.h \
    ./src/annoExceptions/include/FormatException.h \
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
SOURCES += ./src/uiStuff/AnnoGraphicsView.cpp \
    ./src/uiStuff/FilterControl.cpp \
    ./src/uiStuff/helper/AnnoGLView.cpp \
    ./src/uiStuff/helper/AnnoFilterHighlighter.cpp \
    ./src/uiStuff/helper/AnnoFiltersModelAdapter.cpp \
    ./src/annoFilter/AnnoFilterManager.cpp \
    ./src/uiStuff/DlgFilterEdit.cpp \
    ./src/annoFilter/AfrSwitch.cpp \
    ./src/annoFilter/AnnoFilterXmlLoader.cpp \
    ./src/annoFilter/AfrScore.cpp \
    ./src/annoFilter/AfrClass.cpp \
    ./src/annoFilter/AfrAttributeValue.cpp \
    ./src/annoFilter/LogicFilterRule.cpp \
    ./src/annoFilter/AnnoFilterRuleAtom.cpp \
    ./src/annoFilter/AfrXor.cpp \
    ./src/annoFilter/AfrNot.cpp \
    ./src/annoFilter/AfrOr.cpp \
    ./src/annoFilter/AfrAnd.cpp \
    ./src/annoFilter/AnnoFilter.cpp \
    ./src/annoFilter/AnnoFilterRule.cpp \
    ./src/general/GlobalInfo.cpp \
    ./src/annoExceptions/AnnoException.cpp \
    ./src/annoExceptions/NullPointerException.cpp \
    ./src/annoExceptions/FormatException.cpp \
    ./src/annoExceptions/IOException.cpp \
    ./src/annoExceptions/NameConflictException.cpp \
    ./src/annoExceptions/NoSuchElementException.cpp \
    ./src/annoExceptions/OutOfRangeException.cpp \
    ./src/annoExceptions/IllegalStateException.cpp \
    ./src/annoExceptions/XmlException.cpp \
    ./src/annoExceptions/XmlFormatException.cpp \
    ./src/annoDataTypes/AnnoClassDefinition.cpp \
    ./src/annoDataTypes/AnnoClassList.cpp \
    ./src/annoDataTypes/AnnoAvClassList.cpp \
    ./src/annoDataTypes/AnnoImageInfo.cpp \
    ./src/annoDataTypes/AnnoInfo.cpp \
    ./src/annoDataTypes/Annotation.cpp \
    ./src/annoDataTypes/AnnoFileData.cpp \
    ./src/annoDataTypes/AnnoProject.cpp \
    ./src/annoDataTypes/AnnoOperationHelper.cpp \
    ./src/annoDataTypes/AnnoFileListSorter.cpp \
    ./src/annoDataTypes/AnnoShape.cpp \
    ./src/annoDataTypes/AnnoRectangle.cpp \
    ./src/annoDataTypes/AnnoEllipse.cpp \
    ./src/annoDataTypes/AnnoSinglePoint.cpp \
    ./src/annoDataTypes/AnnoPolygon.cpp \
    ./src/annoGraphics/AnnoGraphicsShape.cpp \
    ./src/annoGraphics/AnnoGraphicsShapeCreator.cpp \
    ./src/annoGraphics/AnnoGraphicsControlPoint.cpp \
    ./src/annoGraphics/AnnoGraphicsRect.cpp \
    ./src/annoGraphics/AnnoGraphicsEllipse.cpp \
    ./src/annoGraphics/AnnoGraphicsSinglePoint.cpp \
    ./src/annoGraphics/AnnoGraphicsPolygon.cpp \
    ./src/annoGraphics/AnnoGraphicsPixmap.cpp \
    ./src/annoGraphics/AnnoGraphicsScene.cpp \
    ./src/annoGraphics/GraphicsTool.cpp \
    ./src/annoGraphics/ToolHand.cpp \
    ./src/annoGraphics/ToolPointer.cpp \
    ./src/annoGraphics/ToolSinglePoint.cpp \
    ./src/annoGraphics/ToolRect.cpp \
    ./src/annoGraphics/ToolEllipse.cpp \
    ./src/annoGraphics/ToolPolygon.cpp \
    ./src/annoGraphics/MapperHelper.cpp \
    ./src/utils/PriorityList.cpp \
    ./src/utils/SortedList.cpp \
    ./src/general/GlobalToolManager.cpp \
    ./src/general/GlobalConfig.cpp \
    ./src/general/GlobalImageLoader.cpp \
    ./src/general/GlobalProjectManager.cpp \
    ./src/general/GlobalImExportManager.cpp \
    ./src/general/imexporter/IdlExporterPlugin.cpp \
    ./src/general/imexporter/IdlImporterPlugin.cpp \
    ./src/annoHelper/XmlHelper.cpp \
    ./src/annoHelper/logging/GlobalLogger.cpp \
    ./src/annoHelper/logging/Logger.cpp \
    ./src/annoHelper/logging/SingleFileLogger.cpp \
    ./src/annoHelper/logging/MultiLogger.cpp \
    ./src/annoHelper/logging/DummyLogger.cpp \
    ./src/annoHelper/logging/ConsoleLogger.cpp \
    ./src/libAnnotationDeprecated/libAn_XmlHelpers.cpp \
    ./src/libAnnotationDeprecated/libAn_Annotation.cpp \
    ./src/libAnnotationDeprecated/libAn_AnnotationList.cpp \
    ./src/libAnnotationDeprecated/libAn_AnnoRect.cpp \
    ./src/uiStuff/DlgFileChooser.cpp \
    ./src/uiStuff/DlgLoaderDetails.cpp \
    ./src/uiStuff/DlgImporter.cpp \
    ./src/uiStuff/DlgExporter.cpp \
    ./src/uiStuff/DlgIdlExporter.cpp \
    ./src/uiStuff/DlgAbout.cpp \
    ./src/uiStuff/DlgProjectDetails.cpp \
    ./src/uiStuff/DlgAddImage.cpp \
    ./src/uiStuff/DlgEditAttribute.cpp \
    ./src/uiStuff/DlgEditAttributeShort.cpp \
    ./src/uiStuff/DlgEditClass.cpp \
    ./src/uiStuff/DlgNewProject.cpp \
    ./src/uiStuff/helper/RecentAttrValues.cpp \
    ./src/uiStuff/helper/AnnoDataClassesModelAdapter.cpp \
    ./src/uiStuff/helper/AnnoDataModelAdapter.cpp \
    ./src/uiStuff/helper/AnnoListModelAdapter.cpp \
    ./src/uiStuff/helper/AnnoFileListModelAdapter.cpp \
    ./src/uiStuff/AnnoDataWidget.cpp \
    ./src/uiStuff/AnnoFileListWidget.cpp \
    ./src/uiStuff/AnnoListWidget.cpp \
    ./src/uiStuff/ZoomControl.cpp \
    ./src/uiStuff/ShapeContextMenu.cpp \
    ./src/uiStuff/AnnoToolMainWindow.cpp \
    ./src/main.cpp
FORMS += ./src/uiStuff/forms/DlgFilterEdit.ui \
    ./src/uiStuff/forms/DlgEditAttributeShort.ui \
    ./src/uiStuff/forms/DlgEditClass.ui \
    ./src/uiStuff/forms/AnnoToolMainWindow.ui \
    ./src/uiStuff/forms/ZoomControl.ui \
    ./src/uiStuff/forms/AnnoFileListWidget.ui \
    ./src/uiStuff/forms/AnnoDataWidget.ui \
    ./src/uiStuff/forms/AnnoListWidget.ui \
    ./src/uiStuff/forms/DlgNewProject.ui \
    ./src/uiStuff/forms/DlgAddImage.ui \
    ./src/uiStuff/forms/DlgProjectDetails.ui \
    ./src/uiStuff/forms/DlgAbout.ui \
    ./src/uiStuff/forms/DlgEditAttribute.ui \
    ./src/uiStuff/forms/DlgImporter.ui \
    ./src/uiStuff/forms/DlgExporter.ui \
    ./src/uiStuff/forms/DlgLoaderDetails.ui \
    ./src/uiStuff/forms/DlgFileChooser.ui \
    ./src/uiStuff/forms/DlgIdlExporter.ui
RESOURCES += ./src/uiStuff/forms/icons.qrc \
    ./src/uiStuff/forms/cursors.qrc \
    ./src/uiStuff/forms/others.qrc
win32 { 
    RC_FILE = ./src/uiStuff/forms/appicon.rc
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
    ICON = ./src/uiStuff/forms/res/appicon.ico
    release { 
        DESTDIR = ./bin/macOSX/release
        OBJECTS_DIR = ./bin/macOSX/release/obj
    }
    debug { 
        DESTDIR = ./bin/macOSX/debug
        OBJECTS_DIR = ./bin/macOSX/debug/obj
    }
}
