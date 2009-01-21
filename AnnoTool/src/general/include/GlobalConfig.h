#ifndef GLOBALCONFIG_H_
#define GLOBALCONFIG_H_

#include "AllAnnoExceptions.h"
#include "VisualShapeConfig.h"
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

        FileExtensions();
        FileExtensions(const QString &pr, const QString &cd, const QString &at);
        static QString asFilter(const QString &ext);
    };

//	class ShapeConfig
//	{
//			// public members
//		public:
//			QPen penNormal;
//			QPen penSelected;
//			QBrush brushNormal;
//			QBrush brushSelected;
//
//			// Constructors
//		public:
//			ShapeConfig();
//			ShapeConfig(const QPen& pN, const QPen& pS, const QBrush& bN, const QBrush& bS);
//			ShapeConfig(const QPen& pN, int widthN, const QPen& pS, int widthS, const QBrush& bN, const QBrush& bS);
//
//			// Xml Interface
//		public:
//			void toXml(QXmlStreamWriter& writer) const;
//			void loadFromXml(QXmlStreamReader& reader) throw(XmlException*);
//			static ShapeConfig fromXml(QXmlStreamReader& reader) throw(XmlException*);
//	};

    class GlobalConfig {
        private:
            static GlobalConfig *_me;

            // actual configuration settings
        private:
            QMap<QString, int> _settingsInt;
            QMap<QString, double> _settingsDouble;
            QMap<QString, QString> _settingsString;
            QMap<QString, QFileInfo> _settingsFile;
            QMap<QString, graphics::VisualShapeConfig> _settingsShapeConfig;

            // static settings that are not stored in file
        public:
            static const FileExtensions fileExt;

        private:
            GlobalConfig();
            static void setupConfig();

        public:
            static GlobalConfig *instance();

            // internal loading/saving stuff
        private:
            QDir configDir();
            QFileInfo configFile();
            void loadDefaults();
            void clearConfig();
            void insertXmlSetting(const QString &name, const QString &type, const QString &value) throw(FormatException *);
            void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
            void loadSectionGeneral(QXmlStreamReader &reader) throw(XmlException *);
            void loadSectionShapeConfig(QXmlStreamReader &reader) throw(XmlException *);
            void writeToXml(QXmlStreamWriter &writer) const throw(XmlException *);
            void writeSectionGeneral(QXmlStreamWriter &writer) const throw(XmlException *);
            void writeSectionShapeConfig(QXmlStreamWriter &writer) const throw(XmlException *);

            // public config query interface
        public:
            int getInt(QString s) const throw(NoSuchElementException *);
            double getDouble(QString s) const throw(NoSuchElementException *);
            QString getString(QString s) const throw(NoSuchElementException *);
            QFileInfo getFile(QString s) const throw(NoSuchElementException *);
            graphics::VisualShapeConfig getShapeConfig(QString s) const
            throw(NoSuchElementException *);
            int getInt(QString s, int defaultValue) const;
            double getDouble(QString s, double defaultValue) const;
            QString getString(QString s, QString defaultValue) const;
            QFileInfo getFile(QString s, QFileInfo defaultValue) const;
            graphics::VisualShapeConfig getShapeConfig(QString s, graphics::VisualShapeConfig defaultValue) const;

            // Load & Save interface
        public:
            void loadConfig();
            void saveConfig() throw(AnnoException *);
    };

    // inlining: FileExtensions
    //-----------------------------------------------------------
    inline FileExtensions::FileExtensions() :
        projects("atp"), classDefs("atc"), annotations("ata") {
    }

    inline FileExtensions::FileExtensions(const QString &pr, const QString &cd,
                                          const QString &at) :
        projects(pr), classDefs(cd), annotations(at) {
    }

    inline QString FileExtensions::asFilter(const QString &ext) {
        return QString("*.%1").arg(ext);
    }
    //-----------------------------------------------------------

    // inlining: ShapeConfig
    //-----------------------------------------------------------
//	inline ShapeConfig::ShapeConfig() :
//		penNormal(Qt::SolidLine), penSelected(Qt::SolidLine), brushNormal(Qt::SolidPattern),
//				brushSelected(Qt::SolidPattern)
//	{
//		penNormal.setColor(QColor(0, 0, 200, 255));
//		penNormal.setWidth(1);
//		penSelected.setColor(QColor(0, 0, 255, 255));
//		penSelected.setWidth(2);
//		brushNormal.setColor(QColor(255, 255, 255, 0));
//		brushSelected.setColor(QColor(255, 255, 255, 50));
//	}
//
//	inline ShapeConfig::ShapeConfig(const QPen& pN, const QPen& pS, const QBrush& bN,
//			const QBrush& bS) :
//		penNormal(pN), penSelected(pS), brushNormal(bN), brushSelected(bS)
//	{
//	}
//
//	inline ShapeConfig::ShapeConfig(const QPen& pN, int widthN, const QPen& pS, int widthS,
//			const QBrush& bN, const QBrush& bS) :
//		penNormal(pN), penSelected(pS), brushNormal(bN), brushSelected(bS)
//	{
//		penNormal.setWidth(widthN);
//		penSelected.setWidth(widthS);
//	}
    //-----------------------------------------------------------

    // inlining: GlobalConfig
    //-----------------------------------------------------------
    inline GlobalConfig::GlobalConfig() {
    }

    inline void GlobalConfig::setupConfig() {
        _me = new GlobalConfig();
    }
    //-----------------------------------------------------------

}
//end namespace anno

#endif /*GLOBALCONFIG_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
