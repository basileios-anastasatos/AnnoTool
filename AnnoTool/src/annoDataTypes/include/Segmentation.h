#pragma once

#include "Annotation.h"
#include <QImage>
#include <QPainterPath>
#include "GrabCut.h"
#include "AnnoSegmentation.h"

using namespace util;

//namespace AnnoTool
namespace anno {
    class InteractiveGrabcut;

    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;

        class Segmentation : public Annotation {
                Q_OBJECT

                friend class AnnoAttribute;

                // private vars
            private:
                std::vector<QPainterPath>	_v_painterFGPath;
                std::vector<QPainterPath>	_v_painterBGPath;
                util::InteractiveGrabcut	*_grabCutContext;
                QString						_sImagePath;


                // internal XML stuff
            private:
                virtual void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);

                // public con/destructors
            public:
                Segmentation(QObject *parent = 0);
                Segmentation(const Segmentation *anno, QObject *parent = 0);
                virtual ~Segmentation();

                // public member access interface
            public:
                void appendFGPath(const QPainterPath &fgPath);
                void appendBGPath(const QPainterPath &bgPath);
                void emptyFGPath();
                void emptyBGPath();
                const std::vector<QPainterPath> getFGPath();
                const std::vector<QPainterPath> getBGPath();
                util::InteractiveGrabcut *provideGrabCutContext();
                util::InteractiveGrabcut *provideGrabCutContext(const QString &sPath, const QRectF &boundBoxRect, const QImage *qSegmMask = NULL);

                // various methods
            public:
                void saveSegmentationImage(const QString &sPath);
                void setSegmentationImagePath(const QString &sPath);
                void buildSegmentationImage();
                void recalculateSegmentation(QRectF &newRect);

                // public XML interface
            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static Annotation *fromXml(QXmlStreamReader &reader) throw(XmlException *);

        };

        //-----------------------------------------------------------

    }
//end namespace dt
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
