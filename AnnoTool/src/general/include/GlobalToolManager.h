#ifndef GLOBALTOOLMANAGER_H_
#define GLOBALTOOLMANAGER_H_

#include "GraphicsTool.h"

namespace anno {
    namespace graphics {
        class AnnoGraphicsScene;
    }

    class GlobalToolManager {
            // enum of selectable tools
        public:
            enum SelGraphicsTool {
                GtNone, GtPointer, GtHand, GtRect
            };

            // Singleton members
        private:
            /**
             * The singleton pointer to the only class instance.
             */
            static GlobalToolManager *_me;

            // other members
        private:
            bool _resetFlag;
            graphics::AnnoGraphicsScene *_curScene;
            graphics::GraphicsTool *_curTool;
            SelGraphicsTool _curToolId;

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
            virtual ~GlobalToolManager();

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
            void setScene(graphics::AnnoGraphicsScene *scene);
            graphics::AnnoGraphicsScene *curScene();
            bool hasScene() const;
            void selectTool(SelGraphicsTool tool);
            SelGraphicsTool curToolId() const;
            graphics::GraphicsTool *curTool();
            bool hasTool() const;
            void resetAll();

    };

}

#endif /*GLOBALTOOLMANAGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
