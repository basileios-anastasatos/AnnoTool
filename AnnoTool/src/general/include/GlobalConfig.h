#ifndef GLOBALCONFIG_H_
#define GLOBALCONFIG_H_

#include "AllAnnoExceptions.h"
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QFileInfo>
#include <QMap>

class QXmlStreamWriter;
class QXmlStreamReader;

#define ANNOTOOL_CONFIG_DIR ".annotoolv2"
#define ANNOTOOL_CONFIG_FILENAME "annoSettings.xml"

//namespace AnnoTool
namespace anno {
    using namespace ::anno::exc;

    struct FileExtensions {
        QString projects;
        QString classDefs;
        QString annotations;

        inline FileExtensions();
        inline FileExtensions(const QString &pr, const QString &cd, const QString &at);
        inline static QString asFilter(const QString &ext);
    };

    struct ShapeColors {
        QPen penNormal;
        QPen penSelected;
        QBrush brushNormal;
        QBrush brushSelected;
        int widthNormal;
        int widthSelected;

        inline ShapeColors();
        inline ShapeColors(const QPen &pN, const QPen &pS, const QBrush &bN,
                           const QBrush &bS, int wN, int wS);
    };

    class GlobalConfig {
        private:
            static GlobalConfig *_me;

            // actual configuration settings
        private:
            QMap<QString, int> _settingsInt;
            QMap<QString, double> _settingsDouble;
            QMap<QString, QString> _settingsString;
            QMap<QString, QFileInfo> _settingsFile;
            QMap<QString, ShapeColors> _settingsShapeColors;

            // static settings that are not stored in file
        public:
            static const FileExtensions fileExt;
            static const ShapeColors shapeColors;

        private:
            GlobalConfig();
            static void setupConfig();

        public:
            ~GlobalConfig();

        public:
            static GlobalConfig *instance();

        private:
            QDir configDir();
            QFileInfo configFile();
            void loadDefaults();
            void clearConfig();
            void insertXmlSetting(const QString &name, const QString &type, const QString &value);
            void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
            void loadSectionGeneral(QXmlStreamReader &reader) throw(XmlException *);
            void loadSectionShapeCols(QXmlStreamReader &reader) throw(XmlException *);
            void writeToXml(QXmlStreamWriter &writer) const throw(XmlException *);
            void writeSectionGeneral(QXmlStreamWriter &writer) const throw(XmlException *);
            void writeSectionShapeCols(QXmlStreamWriter &writer) const throw(XmlException *);

            // public config query interface
        public:
            int getInt(const QString &s) const throw(NoSuchElementException *);
            double getDouble(const QString &s) const throw(NoSuchElementException *);
            QString getString(const QString &s) const throw(NoSuchElementException *);
            QFileInfo getFile(const QString &s) const throw(NoSuchElementException *);
            const ShapeColors &getShapeColors(const QString &s) const
            throw(NoSuchElementException *);
            int getInt(const QString &s, int defaultValue) const;
            double getDouble(const QString &s, double defaultValue) const;
            QString getString(const QString &s, QString defaultValue) const;
            QFileInfo getFile(const QString &s, QFileInfo defaultValue) const;
            const ShapeColors &getShapeColors(const QString &s, const ShapeColors &defaultValue) const;

            // Load & Save interface
        public:
            void loadConfig();
            void saveConfig() throw(AnnoException *);
    };

    inline FileExtensions::FileExtensions() {
    }

    inline FileExtensions::FileExtensions(const QString &pr, const QString &cd,
                                          const QString &at) :
        projects(pr), classDefs(cd), annotations(at) {
    }

    inline QString FileExtensions::asFilter(const QString &ext) {
        Q_ASSERT(!ext.isEmpty());

        QString filter("*.");
        return filter.append(ext);
    }

    inline ShapeColors::ShapeColors() {
        penNormal = QPen(Qt::SolidLine);
        penSelected = QPen(Qt::SolidLine);
        brushNormal = QBrush(Qt::SolidPattern);
        brushSelected = QBrush(Qt::SolidPattern);
        widthNormal = 1;
        widthSelected = 2;

        penNormal.setColor(QColor(30, 30, 255, 200));
        penNormal.setWidth(widthNormal);
        penSelected.setColor(QColor(60, 60, 255, 255));
        penSelected.setWidth(widthSelected);

        brushNormal.setColor(QColor(255, 255, 255, 0));
        brushSelected.setColor(QColor(255, 255, 60, 42));
    }

    inline ShapeColors::ShapeColors(const QPen &pN, const QPen &pS, const QBrush &bN,
                                    const QBrush &bS, int wN, int wS) {
        penNormal = pN;
        penSelected = pS;
        brushNormal = bN;
        brushSelected = bS;
        widthNormal = wN;
        widthSelected = wS;
    }

}
//end namespace anno

#endif /*GLOBALCONFIG_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
