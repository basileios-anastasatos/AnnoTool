#ifndef TOOLHAND_H_
#define TOOLHAND_H_

#include "GraphicsTool.h"

namespace anno {
    namespace graphics {

        class ToolHand : public anno::graphics::GraphicsTool {
            public:
                ToolHand(AnnoGraphicsScene *scene);
                virtual ~ToolHand();

                // general tool information
            public:
                virtual bool handlesCp() const;
                virtual bool handlesShape() const;
                virtual bool handlesImage() const;
        };

    }
}

#endif /*TOOLHAND_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
