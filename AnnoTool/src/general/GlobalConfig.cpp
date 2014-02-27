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

//	void ShapeConfig::toXml(QXmlStreamWriter& writer) const
//	{
//		writer.writeStartElement("shapeConfig");
//
//		writer.writeStartElement("pen");
//		writer.writeAttribute("name", "normal");
//		writer.writeTextElement("style", QString::number((int)penNormal.style(), 10));
//		writer.writeTextElement("width", QString::number(penNormal.width(), 10));
//		writer.writeTextElement("color", XmlHelper::colorAsArgbString(penNormal.color()));
//		writer.writeEndElement();
//
//		writer.writeStartElement("pen");
//		writer.writeAttribute("name", "selected");
//		writer.writeTextElement("style", QString::number((int)penSelected.style(), 10));
//		writer.writeTextElement("width", QString::number(penSelected.width(), 10));
//		writer.writeTextElement("color", XmlHelper::colorAsArgbString(penSelected.color()));
//		writer.writeEndElement();
//
//		writer.writeStartElement("brush");
//		writer.writeAttribute("name", "normal");
//		writer.writeTextElement("style", QString::number((int)brushNormal.style(), 10));
//		writer.writeTextElement("color", XmlHelper::colorAsArgbString(brushNormal.color()));
//		writer.writeEndElement();
//
//		writer.writeStartElement("brush");
//		writer.writeAttribute("name", "selected");
//		writer.writeTextElement("style", QString::number((int)brushSelected.style(), 10));
//		writer.writeTextElement("color", XmlHelper::colorAsArgbString(brushSelected.color()));
//		writer.writeEndElement();
//
//		writer.writeEndElement();
//	}
//
//	void ShapeConfig::loadFromXml(QXmlStreamReader& reader) throw(XmlException*)
//	{
//		QString tagShape("shapeConfig");
//		QString tagPen("pen");
//		QString tagBrush("brush");
//
//		if (!reader.isStartElement() || reader.name() != tagShape)
//		{
//			throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagShape, reader.name().toString());
//		}
//
//		while (!reader.atEnd())
//		{
//			if (reader.isStartElement() && reader.name() == tagPen)
//			{
//				QString name = reader.attributes().value("name").toString();
//				QString style;
//				QString width;
//				QString color;
//
//				if (!XmlHelper::skipToStartElement("style", reader))
//				{
//					throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "style", reader.name().toString());
//				}
//				style = reader.readElementText();
//
//				if (!XmlHelper::skipToStartElement("width", reader))
//				{
//					throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "width", reader.name().toString());
//				}
//				width = reader.readElementText();
//
//				if (!XmlHelper::skipToStartElement("color", reader))
//				{
//					throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "color", reader.name().toString());
//				}
//				color = reader.readElementText();
//
//				bool ok1 = false;
//				bool ok2 = false;
//				int acStyle = style.toInt(&ok1, 10);
//				int acWidth = width.toInt(&ok2, 10);
//				QColor acColor = XmlHelper::argbStringToColor(color);
//
//				if (!ok1 || !ok2 || !acColor.isValid())
//				{
//					throw new XmlFormatException(__FILE__, __LINE__, "Could not parse pen information.");
//				}
//				QPen acPen((Qt::PenStyle)acStyle);
//				acPen.setWidth(acWidth);
//				acPen.setColor(acColor);
//
//				if (name == "normal")
//				{
//					penNormal = acPen;
//				}
//				else if (name == "selected")
//				{
//					penSelected = acPen;
//				}
//			}
//			else if (reader.isStartElement() && reader.name() == tagBrush)
//			{
//				QString name = reader.attributes().value("name").toString();
//				QString style;
//				QString color;
//
//				if (!XmlHelper::skipToStartElement("style", reader))
//				{
//					throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "style", reader.name().toString());
//				}
//				style = reader.readElementText();
//
//				if (!XmlHelper::skipToStartElement("color", reader))
//				{
//					throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "color", reader.name().toString());
//				}
//				color = reader.readElementText();
//
//				bool ok = false;
//				int acStyle = style.toInt(&ok, 10);
//				QColor acColor = XmlHelper::argbStringToColor(color);
//
//				if (!ok || !acColor.isValid())
//				{
//					throw new XmlFormatException(__FILE__, __LINE__, "Could not parse brush information.");
//				}
//				QBrush acBrush((Qt::BrushStyle)acStyle);
//				acBrush.setColor(acColor);
//
//				if (name == "normal")
//				{
//					brushNormal = acBrush;
//				}
//				else if (name == "selected")
//				{
//					brushSelected = acBrush;
//				}
//			}
//			else if (reader.isEndElement() && reader.name() == tagShape)
//			{
//				reader.readNext();
//				break;
//			}
//			else
//				reader.readNext();
//		}
//	}
//
//	ShapeConfig ShapeConfig::fromXml(QXmlStreamReader& reader) throw(XmlException*)
//	{
//		ShapeConfig tmp;
//		tmp.loadFromXml(reader);
//		return tmp;
//	}

    GlobalConfig *GlobalConfig::instance() {
        if (_me == NULL) {
            setupConfig();
        }
        return _me;
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
        _settingsInt.insert("logging.loglevel", (int)logging::Info);
        _settingsInt.insert("graphics.cpSize", 10);
        _settingsInt.insert("graphics.useGL", 0);
        _settingsInt.insert("graphics.glSampling", 4);

        graphics::VisualShapeConfig defaultShapeConfig(QColor(0, 255, 255, 255),
                QColor(0, 255, 255, 255), QColor(255, 255, 255, 0),
                QColor(255, 255, 255, 30), 1, 2);
        _settingsShapeConfig.insert("default", defaultShapeConfig);
    }

    void GlobalConfig::clearConfig() {
        _settingsInt.clear();
        _settingsDouble.clear();
        _settingsString.clear();
        _settingsFile.clear();
        _settingsShapeConfig.clear();
    }

    void GlobalConfig::insertXmlSetting(const QString &name, const QString &type,
                                        const QString &value) throw(FormatException *) {
        if (type == "int") {
            int acVal;
            bool ok = false;
            acVal = value.toInt(&ok, 10);
            if (!ok) {
                throw new FormatException(__FILE__, __LINE__,
                                          QString("Invalid XML-Setting format of [%1] with type [%2] and value [%3].").arg(name).arg(type).arg(value));
            }
            _settingsInt.insert(name, acVal);
        } else if (type == "double") {
            double acVal;
            bool ok = false;
            acVal = value.toDouble(&ok);
            if (!ok) {
                throw new FormatException(__FILE__, __LINE__,
                                          QString("Invalid XML-Setting format of [%1] with type [%2] and value [%3].").arg(name).arg(type).arg(value));
            }
            _settingsDouble.insert(name, acVal);
        } else if (type == "string") {
            _settingsString.insert(name, value);
        } else if (type == "path") {
            QFileInfo acFile(value);
            if (!acFile.isFile() && !acFile.isDir() && !acFile.isSymLink()) {
                throw new FormatException(__FILE__, __LINE__,
                                          QString("Invalid XML-Setting format of [%1] with type [%2] and value [%3].").arg(name).arg(type).arg(value));
            }
            _settingsFile.insert(name, acFile);
        } else {
            throw new FormatException(__FILE__, __LINE__,
                                      QString("Invalid XML-Setting-Type, name was [%1] with type [%2] and value [%3].").arg(name).arg(type).arg(value));
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
                } else if(reader.attributes().value("version") != "0.3") {
                    GlobalLogger::instance()->logWarning("Config file has obsolete format, loading defaults. Current config will be overwritten with new defaults.");
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
            throw new AnnoException(__FILE__, __LINE__, "Error while writing config to file.", e);
        }
        writer.writeEndDocument();
        file.flush();
        file.close();
    }

    void GlobalConfig::loadFromXml(QXmlStreamReader &reader) throw(XmlException *) {
        QString tagSetting("annoSettings");
        QString tagSection("section");
        QString secGeneral("general");
        QString secShapeColors("shapeConfig");
        if (!reader.isStartElement() || reader.name() != tagSetting) {
            throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagSetting, reader.name().toString());
        }

        while (!reader.atEnd()) {
            if (reader.isStartElement() && reader.name() == tagSection) {
                QString id = reader.attributes().value("id").toString();
                if (id == secGeneral) {
                    loadSectionGeneral(reader);
                } else if (id == secShapeColors) {
                    loadSectionShapeConfig(reader);
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
                try {
                    insertXmlSetting(name, type, value);
                } catch(FormatException *e) {
                    throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML setting format.", e);
                }
            } else if (reader.isEndElement() && reader.name() == tagSection) {
                reader.readNext();
                break;
            }
            reader.readNext();
        }
    }

    void GlobalConfig::loadSectionShapeConfig(QXmlStreamReader &reader) throw(XmlException *) {
        QString tagSection("section");
        QString tagInfo("shapeInfo");
        while (!reader.atEnd()) {
            if (reader.isStartElement() && reader.name() == tagInfo) {
                QString name = reader.attributes().value("name").toString();
                if (name.isEmpty()) {
                    throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML setting format.");
                }
                if (!XmlHelper::skipToStartElement("visualShapeConfig", reader)) {
                    throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "visualShapeConfig", reader.name().toString());
                }

                graphics::VisualShapeConfig sc;
                try {
                    sc.loadFromXml(reader);
                } catch(XmlException *e) {
                    throw new XmlException(__FILE__, __LINE__, "Could not load XML shape config.", e);
                }
                _settingsShapeConfig.insert(name, sc);
            } else if (reader.isEndElement() && reader.name() == tagSection) {
                reader.readNext();
                break;
            }
            reader.readNext();
        }
    }

    void GlobalConfig::writeToXml(QXmlStreamWriter &writer) const throw(XmlException *) {
        writer.writeStartElement("annoSettings");
        writer.writeAttribute("version", "0.3");
        writeSectionGeneral(writer);
        writeSectionShapeConfig(writer);
        writer.writeEndElement();
    }

    void GlobalConfig::writeSectionGeneral(QXmlStreamWriter &writer) const
    throw(XmlException *) {
        writer.writeStartElement("section");
        writer.writeAttribute("id", "general");

        QMapIterator<QString, int> iInt(_settingsInt);
        while (iInt.hasNext()) {
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

    void GlobalConfig::writeSectionShapeConfig(QXmlStreamWriter &writer) const
    throw(XmlException *) {
        writer.writeStartElement("section");
        writer.writeAttribute("id", "shapeConfig");

        QMapIterator<QString, graphics::VisualShapeConfig> iShape(_settingsShapeConfig);
        while (iShape.hasNext()) {
            iShape.next();
            writer.writeStartElement("shapeInfo");
            writer.writeAttribute("name", iShape.key());
            iShape.value().toXml(writer);
            writer.writeEndElement();
        }

        writer.writeEndElement();
    }

    int GlobalConfig::getInt(QString s) const throw(NoSuchElementException *) {
        if (_settingsInt.contains(s)) {
            return _settingsInt[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

    double GlobalConfig::getDouble(QString s) const throw(NoSuchElementException *) {
        if (_settingsDouble.contains(s)) {
            return _settingsDouble[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

    QString GlobalConfig::getString(QString s) const throw(NoSuchElementException *) {
        if (_settingsString.contains(s)) {
            return _settingsString[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

    QFileInfo GlobalConfig::getFile(QString s) const throw(NoSuchElementException *) {
        if (_settingsFile.contains(s)) {
            return _settingsFile[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

    graphics::VisualShapeConfig GlobalConfig::getShapeConfig(QString s) const
    throw(NoSuchElementException *) {
        if (_settingsShapeConfig.contains(s)) {
            return _settingsShapeConfig[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

    int GlobalConfig::getInt(QString s, int defaultValue) const {
        if (_settingsInt.contains(s)) {
            return _settingsInt[s];
        } else {
            return defaultValue;
        }
    }

    double GlobalConfig::getDouble(QString s, double defaultValue) const {
        if (_settingsDouble.contains(s)) {
            return _settingsDouble[s];
        } else {
            return defaultValue;
        }
    }

    QString GlobalConfig::getString(QString s, QString defaultValue) const {
        if (_settingsString.contains(s)) {
            return _settingsString[s];
        } else {
            return defaultValue;
        }
    }

    QFileInfo GlobalConfig::getFile(QString s, QFileInfo defaultValue) const {
        if (_settingsFile.contains(s)) {
            return _settingsFile[s];
        } else {
            return defaultValue;
        }
    }

    graphics::VisualShapeConfig GlobalConfig::getShapeConfig(const QString &key, const graphics::VisualShapeConfig &defaultValue) const {
        if (_settingsShapeConfig.contains(key)) {
            return _settingsShapeConfig[key];
        } else {
            return defaultValue;
        }
    }

} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
