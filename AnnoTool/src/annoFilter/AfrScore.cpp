#include "include/AfrScore.h"
#include "Annotation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AnnoFilterXmlLoader.h"
#include "XmlHelper.h"
using anno::helper::XmlHelper;


namespace anno {
    namespace filter {
        const QString AfrScore::XML_NAME("scoreCmp");

        AfrScore::AfrScore() :
            AnnoFilterRuleAtom(true) {
        }

        AfrScore::AfrScore(double score, CompOp op) :
            AnnoFilterRuleAtom(true), _score(score), _op(op) {
        }

        AfrScore::~AfrScore() {
            printf("delete <AfrScore>\n");
        }

        AfrScore *AfrScore::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            AfrScore *pRule = new AfrScore();

            try {
                pRule->loadFromXml(reader);
            } catch(exc::XmlException *e) {
                delete pRule;
                throw e;
            }

            return pRule;
        }

        bool AfrScore::opCompare(const double &curScore) const {
            switch (_op) {
                case Equal:
                    return (curScore == _score);
                case LessThan:
                    return (curScore < _score);
                case GreaterThan:
                    return (curScore > _score);
                case LessThanEqual:
                    return (curScore <= _score);
                case GreaterThanEqual:
                    return (curScore >= _score);
                default:
                    return false;
            }
        }

        QString AfrScore::opToString() const {
            switch(_op) {
                case Equal:
                    return QString("eq");
                case LessThan:
                    return QString("lt");
                case GreaterThan:
                    return QString("gt");
                case LessThanEqual:
                    return QString("lte");
                case GreaterThanEqual:
                    return QString("gte");
                default:
                    return QString("Invalid Op");
            }
        }

        AfrScore::CompOp AfrScore::stringToOp(QString str) const {
            if(QString::compare(str, QString("eq"), Qt::CaseInsensitive) == 0) {
                return Equal;
            } else if(QString::compare(str, QString("gt"), Qt::CaseInsensitive) == 0) {
                return GreaterThan;
            } else if(QString::compare(str, QString("lt"), Qt::CaseInsensitive) == 0) {
                return LessThan;
            } else if(QString::compare(str, QString("gte"), Qt::CaseInsensitive) == 0) {
                return GreaterThanEqual;
            } else if(QString::compare(str, QString("lte"), Qt::CaseInsensitive) == 0) {
                return LessThanEqual;
            } else {
                return InvalidCompOp;
            }
        }

        bool AfrScore::isValid() const {
            return (!std::isnan(_score) && _op != InvalidCompOp);
        }

        QString AfrScore::toString() const {
            QString str("<Score Compare [%1 %2]>");
            str = str.arg(opToString()).arg(_score);
            return str;
        }

        void AfrScore::toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            writer.writeEmptyElement(XML_NAME);
            writer.writeAttribute("value", QString::number(_score));
            writer.writeAttribute("op", opToString());
        }

        void AfrScore::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            QString curParent = reader.name().toString();
            if(!reader.isStartElement() || !isXmlName(curParent)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_NAME, curParent);
            }

            QString strScore = reader.attributes().value("value").toString();
            QString strOp = reader.attributes().value("op").toString();

            if(strScore.isEmpty() || strOp.isEmpty()) {
                throw new exc::XmlFormatException(__FILE__, __LINE__, "Score compare data incomplete!");
            }

            bool ok = false;
            double dScore = strScore.toDouble(&ok);
            if (!ok) {
                throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid score value.");
            }
            _score = dScore;

            CompOp op = stringToOp(strOp);
            if(op == InvalidCompOp) {
                throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid score compare operation.");
            }
            _op = op;
            reader.readNext();
            //reader.readNext();
        }

        bool AfrScore::eval(const dt::Annotation *anno) const throw(exc::IllegalStateException *) {
            return opCompare(anno->score());
        }


    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
