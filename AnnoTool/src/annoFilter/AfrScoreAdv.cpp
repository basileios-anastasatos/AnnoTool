#include "include/AfrScoreAdv.h"
#include "Annotation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AnnoFilterXmlLoader.h"
#include "XmlHelper.h"
using anno::helper::XmlHelper;

namespace anno {
    namespace filter {
        const QString AfrScoreAdv::XML_NAME("scoreAdv");

        AfrScoreAdv::AfrScoreAdv(bool autoDelete) :
            AnnoFilterRuleAtom(autoDelete), _lower(0.0), _upper(1.0), _lowerIncl(true), _upperIncl(true) {
        }

        AfrScoreAdv::AfrScoreAdv(const double &lower, const double &upper, bool lowerInclusive, bool upperInclusive, bool autoDelete) :
            AnnoFilterRuleAtom(autoDelete), _lower(lower), _upper(upper), _lowerIncl(lowerInclusive), _upperIncl(upperInclusive) {
        }

        AfrScoreAdv::~AfrScoreAdv() {
        }

        AfrScoreAdv *AfrScoreAdv::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            AfrScoreAdv *pRule = new AfrScoreAdv();

            try {
                pRule->loadFromXml(reader);
            } catch(exc::XmlException *e) {
                delete pRule;
                throw e;
            }

            return pRule;
        }

        bool AfrScoreAdv::isValid() const {
            return (!std::isnan(_lower) && !std::isnan(_upper));
        }

        QString AfrScoreAdv::toString() const {
            QString str("<Score Compare Adv [%1 %2]>");
            return str;
        }

        void AfrScoreAdv::toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            writer.writeEmptyElement(XML_NAME);
            writer.writeAttribute("lower", QString::number(_lower));
            writer.writeAttribute("upper", QString::number(_upper));
            writer.writeAttribute("li", _lowerIncl ? "1" : "0");
            writer.writeAttribute("ui", _upperIncl ? "1" : "0");
        }

        void AfrScoreAdv::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            QString curParent = reader.name().toString();
            if(!reader.isStartElement() || !isXmlName(curParent)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_NAME, curParent);
            }

            QString strUpper = reader.attributes().value("upper").toString();
            QString strLower = reader.attributes().value("lower").toString();
            QString strUI = reader.attributes().value("ui").toString();
            QString strLI = reader.attributes().value("li").toString();

            if(strUpper.isEmpty() || strLower.isEmpty() || strUI.isEmpty() || strLI.isEmpty()) {
                throw new exc::XmlFormatException(__FILE__, __LINE__, "Score compare data incomplete!");
            }

            //TODO loading advanced settings
//			bool ok = false;
//			double dScore = strScore.toDouble(&ok);
//			if (!ok)
//			{
//				throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid score value.");
//			}
//			_score = dScore;
//
//			CompOp op = stringToOp(strOp);
//			if(op == InvalidCompOp)
//			{
//				throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid score compare operation.");
//			}
//			_op = op;
//			reader.readNext();
            //reader.readNext();
        }

        bool AfrScoreAdv::evalInternal(const dt::Annotation *anno) const throw(exc::IllegalStateException *) {
            return (withinLower(anno->score()) && withinUpper(anno->score()));
        }

        bool AfrScoreAdv::withinLower(const double &score) const {
            if(_lowerIncl) {
                return (score >= _lower);
            }
            return (score > _lower);
        }

        bool AfrScoreAdv::withinUpper(const double &score) const {
            if(_upperIncl) {
                return (score <= _upper);
            }
            return (score < _upper);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
