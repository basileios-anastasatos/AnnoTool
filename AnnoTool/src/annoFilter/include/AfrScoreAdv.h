#ifndef AFRSCOREADV_H_
#define AFRSCOREADV_H_

#include "AnnoFilterRuleAtom.h"

namespace anno {
    namespace filter {

        class AfrScoreAdv : public AnnoFilterRuleAtom {
            private:
                static const QString XML_NAME;

            private:
                double _lower;
                double _upper;
                bool _lowerIncl;
                bool _upperIncl;

            private:
                bool withinLower(const double &score) const;
                bool withinUpper(const double &score) const;

            public:
                AfrScoreAdv(bool autoDelete = true);
                AfrScoreAdv(const double &lower, const double &upper, bool lowerInclusive = false, bool upperInclusive = false, bool autoDelete = true);

                virtual ~AfrScoreAdv();

                // Class specific methods
            public:
                static bool isXmlName(const QString &str);
                static QString xmlName();
                static AfrScoreAdv *fromXml(QXmlStreamReader &reader) throw(exc::XmlException *);
                double getLower() const;
                double getUpper() const;
                bool lowerIncl() const;
                bool upperIncl() const;

                void setLower(const double &limit);
                void setUpper(const double &limit);
                void setLowerIncl(bool value);
                void setUpperIncl(bool value);

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
                virtual bool evalInternal(const dt::Annotation *anno) const
                throw(exc::IllegalStateException *);
                // ------------------------------------------------------------------------------------

        };

        // Inlining
        // ------------------------------------------------------------------------------------
        inline bool AfrScoreAdv::isXmlName(const QString &str) {
            return (QString::compare(str, XML_NAME, Qt::CaseInsensitive) == 0);
        }

        inline QString AfrScoreAdv::xmlName() {
            return XML_NAME;
        }

        inline double AfrScoreAdv::getLower() const {
            return _lower;
        }

        inline double AfrScoreAdv::getUpper() const {
            return _upper;
        }

        inline bool AfrScoreAdv::lowerIncl() const {
            return _lowerIncl;
        }

        inline bool AfrScoreAdv::upperIncl() const {
            return _upperIncl;
        }

        inline void AfrScoreAdv::setLower(const double &limit) {
            _lower = limit;
        }

        inline void AfrScoreAdv::setUpper(const double &limit) {
            _upper = limit;
        }

        inline void AfrScoreAdv::setLowerIncl(bool value) {
            _lowerIncl = value;
        }

        inline void AfrScoreAdv::setUpperIncl(bool value) {
            _upperIncl = value;
        }
        // ------------------------------------------------------------------------------------


    }
}

#endif /* AFRSCOREADV_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
