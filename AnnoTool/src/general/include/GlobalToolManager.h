#ifndef GLOBALTOOLMANAGER_H_
#define GLOBALTOOLMANAGER_H_

#include "GraphicsTool.h"
#include "RecentAttrValues.h"
#include <QUuid>

class QGraphicsView;
class ShapeContextMenu;

namespace anno {
    namespace graphics {
        class AnnoGraphicsScene;
    }
    namespace dt {
        class Annotation;
    }

    class GlobalToolManager : public QObject {
            Q_OBJECT

            // enum of selectable tools
        public:
            enum SelGraphicsTool {
                GtNone, GtPointer, GtSinglePoint, GtRect, GtPolygon, GtEllipse, GtNone2
            };

            // Singleton members
        private:
            /**
             * The singleton pointer to the only class instance.
             */
            static GlobalToolManager *_me;

            // other members
        private:
            RecentAttrValues _recentValues;
            bool _resetFlag;
            QGraphicsView *_curView;
            ShapeContextMenu *_curMenu;
            graphics::AnnoGraphicsScene *_curScene;
            graphics::GraphicsTool *_curTool;
            SelGraphicsTool _curToolId;
            QUuid _lastAnnoAdded;
            QUuid _lockedParentAnno;

            // private singleton stuff
        private:
            /**
             * The private singleton constructor.
             */
            GlobalToolManager();

            /**
             * Internal helper for initializing the singleton.
             */
            static void setupGlobalToolManager();

            // destructor
        public:
            /**
             * Virtual destructor.
             */
            ~GlobalToolManager();

            // public singleton stuff
        public:
            /**
             * Singleton access method.
             * This method enables the global access to the only class instance.
             *
             * @return The only class instance.
             */
            static GlobalToolManager *instance();

            /**
             * Singleton reset method.
             * This method provides the capability to reset the only existing
             * class instance.
             */
            static void reset();

            // internal stuff
        private:
            void clearTool();

            // general interface
        public:
            void setView(QGraphicsView *view);
            QGraphicsView *curView();
            bool hasView() const;
            void setScene(graphics::AnnoGraphicsScene *scene);
            graphics::AnnoGraphicsScene *curScene();
            bool hasScene() const;
            bool selectTool(SelGraphicsTool tool);
            bool selectToolDefault();
            SelGraphicsTool curToolId() const;
            graphics::GraphicsTool *curTool();
            bool hasTool() const;
            void resetAll();

            void triggerShapeContextMenu(anno::dt::Annotation *anno) const;
            RecentAttrValues *recentValues();

            bool hasLockedAnno() const;
            bool hasLastAnno() const;
            QUuid getLockedAnno() const;
            QUuid getLastAnno() const;
            void setLockedAnno(const QUuid &uuid);
            void setLastAnno(const QUuid &uuid);
            void resetLockedAnno();
            void resetLastAnno();

        signals:
            void toolSelected(anno::GlobalToolManager::SelGraphicsTool tool, bool reset);


    };

}

#endif /*GLOBALTOOLMANAGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
