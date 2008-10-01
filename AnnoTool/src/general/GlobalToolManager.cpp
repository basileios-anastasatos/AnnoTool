#include "include/GlobalToolManager.h"
#include "AnnoGraphicsScene.h"
#include "AllGraphicsTools.h"
#include "ShapeContextMenu.h"

#include "GlobalLogger.h"
using ::logging::GlobalLogger;

namespace anno {

    GlobalToolManager *GlobalToolManager::_me = NULL;

    GlobalToolManager::GlobalToolManager() :
        _resetFlag(false), _curView(NULL), _curMenu(NULL), _curScene(NULL), _curTool(NULL),
        _curToolId(GtNone) {
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

    void GlobalToolManager::setView(QGraphicsView *view) {
        _curView = view;
        _curMenu = new ShapeContextMenu((QWidget *)view);
    }

    QGraphicsView *GlobalToolManager::curView() {
        return _curView;
    }

    bool GlobalToolManager::hasView() const {
        return (_curView != NULL);
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
        if (_curView == NULL) {
            GlobalLogger::instance()->logWarning("Aborted tool selection due to missing view.");
            return;
        }
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
                    _curTool = new graphics::ToolPointer(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("Selected Tool GtPointer");
                    break;
                }
            case GtHand: {
                    clearTool();
                    _curTool = new graphics::ToolHand(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("Selected Tool GtHand");
                    break;
                }
            case GtRect: {
                    clearTool();
                    _curTool = new graphics::ToolRect(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("Selected Tool GtRect");
                    break;
                }
            case GtPolygon: {
                    clearTool();
                    _curTool = new graphics::ToolPolygon(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("Selected Tool GtPolygon");
                    break;
                }
            case GtSinglePoint: {
                    clearTool();
                    _curTool = new graphics::ToolSinglePoint(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("Selected Tool GtSinglePoint");
                    break;
                }
            case GtEllipse: {
                    clearTool();
                    _curTool = new graphics::ToolEllipse(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("Selected Tool GtEllipse");
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
        if (_resetFlag) {
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
        if(_curMenu != NULL) {
            delete _curMenu;
        }
        _curTool = NULL;
        _curView = NULL;
        _curMenu = NULL;
        _curScene = NULL;
        _resetFlag = true;
    }

    void GlobalToolManager::triggerShapeContextMenu(anno::dt::Annotation *anno) const {
        if(_curMenu != NULL) {
            _curMenu->triggerMenu(anno);
        }
    }

    RecentAttrValues *GlobalToolManager::recentValues() {
        return &_recentValues;
    }

    bool GlobalToolManager::hasLockedAnno() const {
        return !_lockedParentAnno.isNull();
    }

    bool GlobalToolManager::hasLastAnno() const {
        return !_lastAnnoAdded.isNull();
    }

    QUuid GlobalToolManager::getLockedAnno() const {
        return _lockedParentAnno;
    }

    QUuid GlobalToolManager::getLastAnno() const {
        return _lastAnnoAdded;
    }

    void GlobalToolManager::setLockedAnno(const QUuid &uuid) {
        _lockedParentAnno = uuid;
    }

    void GlobalToolManager::setLastAnno(const QUuid &uuid) {
        _lastAnnoAdded = uuid;
    }

    void GlobalToolManager::resetLockedAnno() {
        _lockedParentAnno = QUuid();
    }

    void GlobalToolManager::resetLastAnno() {
        _lastAnnoAdded = QUuid();
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
