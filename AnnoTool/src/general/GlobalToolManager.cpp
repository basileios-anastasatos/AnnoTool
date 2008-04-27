#include "include/GlobalToolManager.h"
#include "AnnoGraphicsScene.h"
#include "AllGraphicsTools.h"

#include "GlobalLogger.h"
using ::logging::GlobalLogger;

namespace anno {

    GlobalToolManager *GlobalToolManager::_me = NULL;

    GlobalToolManager::GlobalToolManager() {
        _resetFlag = false;
        _curScene = NULL;
        _curTool = NULL;
        _curToolId = GtNone;
    }

    GlobalToolManager::~GlobalToolManager() {
        if (_curTool != NULL) {
            delete _curTool;
        }
    }

    void GlobalToolManager::setupGlobalToolManager() {
        _me = new GlobalToolManager();
    }

    GlobalToolManager *GlobalToolManager::instance() {
        if (_me == NULL) {
            setupGlobalToolManager();
        }
        return _me;
    }

    void GlobalToolManager::reset() {
        if (_me != NULL) {
            delete _me;
            _me = NULL;
        }
    }

    void GlobalToolManager::clearTool() {
        if (_curTool != NULL) {
            delete _curTool;
        }
        _curTool = NULL;
        _curToolId = GtNone;
    }

    void GlobalToolManager::setScene(graphics::AnnoGraphicsScene *scene) {
        _curScene = scene;
    }

    graphics::AnnoGraphicsScene *GlobalToolManager::curScene() {
        return _curScene;
    }

    bool GlobalToolManager::hasScene() const {
        return (_curScene != NULL);
    }

    void GlobalToolManager::selectTool(GlobalToolManager::SelGraphicsTool tool) {
        if (_curScene == NULL) {
            GlobalLogger::instance()->logWarning("Aborted tool selection due to missing scene.");
            return;
        }

        switch (tool) {
            case GtNone: {
                    GlobalLogger::instance()->logDebug("Selected Tool GtNone");
                    clearTool();
                    break;
                }
            case GtPointer: {
                    clearTool();
                    _curTool = new graphics::ToolPointer(_curScene);
                    GlobalLogger::instance()->logDebug("Selected Tool GtPointer");
                    break;
                }
            case GtHand: {
                    clearTool();
                    _curTool = new graphics::ToolHand(_curScene);
                    GlobalLogger::instance()->logDebug("Selected Tool GtHand");
                    break;
                }
            case GtRect: {
                    clearTool();
                    _curTool = new graphics::ToolRect(_curScene);
                    GlobalLogger::instance()->logDebug("Selected Tool GtRect");
                    break;
                }
            default:
                GlobalLogger::instance()->logWarning("Aborted tool selection due to unknown tool type.");
                return;
        }
        _curToolId = tool;
        _resetFlag = false;
    }

    GlobalToolManager::SelGraphicsTool GlobalToolManager::curToolId() const {
        return _curToolId;
    }

    graphics::GraphicsTool *GlobalToolManager::curTool() {
        if(_resetFlag) {
            GlobalLogger::instance()->logDebug("Restoring current tool after reset.");
            selectTool(_curToolId);
        }
        return _curTool;
    }

    bool GlobalToolManager::hasTool() const {
        return ((_resetFlag && _curToolId != GtNone) || _curTool != NULL);
    }

    void GlobalToolManager::resetAll() {
        if (_curTool != NULL) {
            delete _curTool;
        }
        _curTool = NULL;
        _curScene = NULL;
        _resetFlag = true;
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
