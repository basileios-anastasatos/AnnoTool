#ifndef GLOBALCONFIG_H_
#define GLOBALCONFIG_H_

#include "AllAnnoExceptions.h"
#include <QPen>
#include <QBrush>
#include <QColor>

//namespace AnnoTool
namespace anno {
    using namespace ::anno::exc;

    struct FileExtensions {
        QString projects;
        QString classDefs;
        QString annotations;

        inline FileExtensions();
        inline FileExtensions(const QString &pr, const QString &cd,
                              const QString &at);
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

            //				QFileInfo _defaultAnnoClassDir;
            //				QFileInfo _defaultSaveDir;

        public:
            static const FileExtensions fileExt;
            static const ShapeColors shapeColors;

        private:
            GlobalConfig();
            static void setupConfig();

        public:
            virtual ~GlobalConfig();

        public:
            static const GlobalConfig *instance();
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

    inline ShapeColors::ShapeColors(const QPen &pN, const QPen &pS,
                                    const QBrush &bN, const QBrush &bS, int wN, int wS) {
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
