#include "include/ToolHand.h"

namespace anno {
    namespace graphics {

        ToolHand::ToolHand(AnnoGraphicsScene *scene) :
            GraphicsTool(scene) {
        }

        ToolHand::~ToolHand() {
        }

        bool ToolHand::handlesCp() const {
            return false;
        }

        bool ToolHand::handlesShape() const {
            return false;
        }

        bool ToolHand::handlesImage() const {
            return true;
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
