#include "include/GlobalConfig.h"

#include <QDir>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "XmlHelper.h"
#include "GlobalLogger.h"
using logging::GlobalLogger;

//namespace AnnoTool
namespace anno {
    using ::anno::helper::XmlHelper;

    GlobalConfig *GlobalConfig::_me = NULL;
    const FileExtensions GlobalConfig::fileExt("atp", "atc", "ata");
    const ShapeColors GlobalConfig::shapeColors;

    GlobalConfig::GlobalConfig() {
    }

    GlobalConfig::~GlobalConfig() {
    }

    void GlobalConfig::setupConfig() {
        _me = new GlobalConfig();
    }

    GlobalConfig *GlobalConfig::instance() {
        if (_me == NULL) {
            setupConfig();
        }
        return _me;
    }

    int GlobalConfig::getInt(const QString &s) const throw(NoSuchElementException *) {
        if (_settingsInt.contains(s)) {
            return _settingsInt[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

    double GlobalConfig::getDouble(const QString &s) const throw(NoSuchElementException *) {
        if (_settingsDouble.contains(s)) {
            return _settingsDouble[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

    QString GlobalConfig::getString(const QString &s) const throw(NoSuchElementException *) {
        if (_settingsString.contains(s)) {
            return _settingsString[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

    QFileInfo GlobalConfig::getFile(const QString &s) const throw(NoSuchElementException *) {
        if (_settingsFile.contains(s)) {
            return _settingsFile[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

    QDir GlobalConfig::configDir() {
        QString configDirPath("%1/%2");
        configDirPath = configDirPath.arg(QDir::homePath()).arg(ANNOTOOL_CONFIG_DIR);
        return QDir(configDirPath);
    }

    QFileInfo GlobalConfig::configFile() {
        return QFileInfo(configDir().absolutePath() + QString("/%1").arg(ANNOTOOL_CONFIG_FILENAME));
    }

    void GlobalConfig::loadDefaults() {
        _settingsInt.insert("logging.loglevel", (int)logging::Debug);

    }

    void GlobalConfig::clearConfig() {
        _settingsInt.clear();
        _settingsDouble.clear();
        _settingsString.clear();
        _settingsFile.clear();
        _settingsShapeColors.clear();
    }

    void GlobalConfig::insertXmlSetting(const QString &name, const QString &type, const QString &value) {
        if(type == "int") {
            int acVal;
            bool ok = false;
            acVal = value.toInt(&ok, 10);
            if(!ok) {
                //TODO error here!
            }
            _settingsInt.insert(name, acVal);
        } else if(type == "double") {

        }
    }

    void GlobalConfig::loadConfig() {
        QDir confDir = configDir();
        QFileInfo confFile = configFile();
        if (!confDir.exists()) {
            if (!QDir::home().mkdir(ANNOTOOL_CONFIG_DIR)) {
                GlobalLogger::instance()->logError("Could not create anno config dir!");
            }
            GlobalLogger::instance()->logWarning("No config file available, loading defaults.");
            loadDefaults();
        } else if (!confFile.exists()) {
            GlobalLogger::instance()->logWarning("No config file available, loading defaults.");
            loadDefaults();
        } else {
            QFile file(confFile.absoluteFilePath());
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                GlobalLogger::instance()->logWarning("Cannot open config file, loading defaults.");
                loadDefaults();
            } else {
                QXmlStreamReader reader(&file);
                reader.setNamespaceProcessing(true);
                if (!XmlHelper::skipToStartElement("annoSettings", reader)) {
                    GlobalLogger::instance()->logWarning("Config file has invalid format, loading defaults.");
                    loadDefaults();
                } else {
                    try {
                        loadFromXml(reader);
                    } catch(AnnoException *e) {
                        GlobalLogger::instance()->logError(QString("Could not load anno config... %1").arg(e->getTrace()));
                        delete e;
                        clearConfig();
                        GlobalLogger::instance()->logWarning("Config file has invalid format, loading defaults.");
                        loadDefaults();
                    }
                }
                file.close();
            }
        }
    }

    void GlobalConfig::saveConfig() throw(AnnoException *) {
        QDir confDir = configDir();
        QFileInfo confFile = configFile();
        if (!confDir.exists()) {
            if (!QDir::home().mkdir(ANNOTOOL_CONFIG_DIR)) {
                GlobalLogger::instance()->logError("Could not create anno config dir!");
                throw new AnnoException(__FILE__, __LINE__, "Could not create anno config dir within the home directory.");
            }
        }

        QFile file(confFile.absoluteFilePath());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            GlobalLogger::instance()->logError("Could not open config file for writing.");
            throw new AnnoException(__FILE__, __LINE__, "Could not open config file for writing.");
        }

        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        try {
            writeToXml(writer);
        } catch(XmlException *e) {
            GlobalLogger::instance()->logError("Could not complete XML writing. Config file is corrupted.");
            throw new AnnoException(__FILE__, __LINE__, "", e);
        }
        writer.writeEndDocument();
        file.flush();
        file.close();
    }

    void GlobalConfig::loadFromXml(QXmlStreamReader &reader) throw(XmlException *) {
        QString tagSetting("annoSettings");
        QString tagSection("section");
        QString secGeneral("general");
        QString secShapeColors("shapeColors");
        if (!reader.isStartElement() || reader.name() != tagSetting) {
            throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagSetting, reader.name().toString());
        }

        while (!reader.atEnd()) {
            if (reader.isStartElement() && reader.name() == tagSection) {
                QString id = reader.attributes().value("id").toString();
                if (id == secGeneral) {
                    loadSectionGeneral(reader);
                } else if (id == secShapeColors) {
                    loadSectionShapeCols(reader);
                }
            } else if (reader.isEndElement() && reader.name() == tagSetting) {
                reader.readNext();
                break;
            }
            reader.readNext();
        }
    }

    void GlobalConfig::loadSectionGeneral(QXmlStreamReader &reader) throw(XmlException *) {
        QString tagSection("section");
        QString secGeneral("general");
        QString tagSet("set");
        while (!reader.atEnd()) {
            if (reader.isStartElement() && reader.name() == tagSet) {
                QString name = reader.attributes().value("name").toString();
                QString type = reader.attributes().value("type").toString();
                QString value = reader.attributes().value("value").toString();
                if (name.isEmpty() || type.isEmpty() || value.isEmpty()) {
                    throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML setting format.");
                }
                insertXmlSetting(name, type, value);
            } else if (reader.isEndElement() && reader.name() == tagSection) {
                reader.readNext();
                break;
            }
            reader.readNext();
        }
    }

    void GlobalConfig::loadSectionShapeCols(QXmlStreamReader &reader) throw(XmlException *) {
        QString tagSection("section");
        QString secShapeColors("shapeColors");

    }

    void GlobalConfig::writeToXml(QXmlStreamWriter &writer) const throw(XmlException *) {
        writer.writeStartElement("annoSettings");
        writer.writeAttribute("version", "0.2");
        writeSectionGeneral(writer);
        writeSectionShapeCols(writer);
        writer.writeEndElement();
    }

    void GlobalConfig::writeSectionGeneral(QXmlStreamWriter &writer) const throw(XmlException *) {
        writer.writeStartElement("section");
        writer.writeAttribute("id", "general");

        QMapIterator<QString, int> iInt(_settingsInt);
        while(iInt.hasNext()) {
            iInt.next();
            writer.writeEmptyElement("set");
            writer.writeAttribute("name", iInt.key());
            writer.writeAttribute("type", "int");
            writer.writeAttribute("value", QString::number(iInt.value(), 10));
        }

        QMapIterator<QString, double> iDouble(_settingsDouble);
        while (iDouble.hasNext()) {
            iDouble.next();
            writer.writeEmptyElement("set");
            writer.writeAttribute("name", iDouble.key());
            writer.writeAttribute("type", "double");
            writer.writeAttribute("value", QString::number(iDouble.value(), 'f'));
        }

        QMapIterator<QString, QString> iString(_settingsString);
        while (iString.hasNext()) {
            iString.next();
            writer.writeEmptyElement("set");
            writer.writeAttribute("name", iString.key());
            writer.writeAttribute("type", "string");
            writer.writeAttribute("value", iString.value());
        }

        QMapIterator<QString, QFileInfo> iFile(_settingsFile);
        while (iFile.hasNext()) {
            iFile.next();
            writer.writeEmptyElement("set");
            writer.writeAttribute("name", iFile.key());
            writer.writeAttribute("type", "path");
            writer.writeAttribute("value", iFile.value().absolutePath());
        }

        writer.writeEndElement();
    }

    void GlobalConfig::writeSectionShapeCols(QXmlStreamWriter &writer) const throw(XmlException *) {
        writer.writeStartElement("section");
        writer.writeAttribute("id", "shapeColors");
        writer.writeEndElement();
    }

} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
