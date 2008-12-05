#ifndef AFRSCORE_H_
#define AFRSCORE_H_

#include "AnnoFilterRuleAtom.h"

namespace anno {
    namespace filter {

        class AfrScore: public AnnoFilterRuleAtom {
            public:
                enum CompOp {
                    InvalidCompOp, Equal, LessThan, GreaterThan, LessThanEqual, GreaterThanEqual
                };

            private:
                static const QString XML_NAME;

            private:
                double _score;
                CompOp _op;

            private:
                bool opCompare(const double &curScore) const;
                QString opToString() const;
                CompOp stringToOp(QString str) const;
                AfrScore();

            public:
                AfrScore(double score, CompOp op);
                virtual ~AfrScore();

                // Class specific methods
            public:
                static bool isXmlName(const QString &str);
                static QString xmlName();
                static AfrScore *fromXml(QXmlStreamReader &reader) throw(exc::XmlException *);
                double getScore() const;
                CompOp getOp() const;

                // inherited interface stuff
                // ------------------------------------------------------------------------------------
                // General interface
            public:
                virtual bool isValid() const;
                virtual QString toString() const;

                // XML interface
            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *);
                virtual void loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *);

                // Filtering interface
            public:
                virtual bool eval(const dt::Annotation *anno) const
                throw(exc::IllegalStateException *);
                // ------------------------------------------------------------------------------------

        };

        // Inlining
        // ------------------------------------------------------------------------------------
        inline bool AfrScore::isXmlName(const QString &str) {
            return (QString::compare(str, XML_NAME, Qt::CaseInsensitive) == 0);
        }

        inline QString AfrScore::xmlName() {
            return XML_NAME;
        }

        inline double AfrScore::getScore() const {
            return _score;
        }

        inline AfrScore::CompOp AfrScore::getOp() const {
            return _op;
        }
        // ------------------------------------------------------------------------------------

    }
}

#endif /* AFRSCORE_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
