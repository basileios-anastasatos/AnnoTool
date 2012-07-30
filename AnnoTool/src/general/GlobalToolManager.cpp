#include "include/GlobalToolManager.h"
#include "AnnoGraphicsScene.h"
#include "AnnoGraphicsShapeCreator.h"
#include "AllGraphicsTools.h"
#include "ShapeContextMenu.h"
#include "Segmentation.h"

#include "GlobalLogger.h"
using ::logging::GlobalLogger;

#include "qgraphicsview.h"
#include "qpainter.h"


cv::Mat qimage2mat(const QImage &qimage) {
    cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar *)qimage.bits(), qimage.bytesPerLine());
    cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
    int from_to[] = { 0, 0,  1, 1,  2, 2 };
    cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
    return mat2;
}

//QImage mat2qimage(const cv::Mat& mat)
//{
//	cv::Mat rgb;
//    cvtColor(mat, rgb, CV_BGR2RGB);
//    return QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, QImage::Format_RGB888);
//};

QImage *Mat2QImage(const cv::Mat3b &src) {
    QImage *dest = new QImage(src.cols, src.rows, QImage::Format_ARGB32);
    for (int y = 0; y < src.rows; ++y) {
        const cv::Vec3b *srcrow = src[y];
        QRgb *destrow = (QRgb *)dest->scanLine(y);
        for (int x = 0; x < src.cols; ++x) {
            destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
        }
    }
    return dest;
}

//QImage Mat2QImage(const cv::Mat_<double> &src)
//{
//        double scale = 255.0;
//        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
//        for (int y = 0; y < src.rows; ++y) {
//                const double *srcrow = src[y];
//                QRgb *destrow = (QRgb*)dest.scanLine(y);
//                for (int x = 0; x < src.cols; ++x) {
//                        unsigned int color = srcrow[x] * scale;
//                        destrow[x] = qRgba(color, color, color, 255);
//                }
//        }
//        return dest;
//}

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
            _curTool->toolDeactivate();
            delete _curTool;
        }
        _curTool = NULL;
        _curToolId = GtNone;
    }

    void GlobalToolManager::setView(QGraphicsView *view) {
        _curView = view;
        if(_curMenu == NULL) {
            _curMenu = new ShapeContextMenu((QWidget *)view);
        }
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

    bool GlobalToolManager::selectTool(GlobalToolManager::SelGraphicsTool tool) {
        if (_curView == NULL) {
            GlobalLogger::instance()->logWarning("GlobalToolManager: Aborted tool selection due to missing view.");
            return false;
        }
        if (_curScene == NULL) {
            GlobalLogger::instance()->logWarning("GlobalToolManager: Aborted tool selection due to missing scene.");
            return false;
        }

        switch (tool) {
            case GtNone: {
                    GlobalLogger::instance()->logDebug("GlobalToolManager: Selected Tool GtNone.");
                    clearTool();
                    break;
                }
            case GtPointer: {
                    clearTool();
                    _curTool = new graphics::ToolPointer(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("GlobalToolManager: Selected Tool GtPointer.");
                    break;
                }
//			case GtHand:
//			{
//				clearTool();
//				_curTool = new graphics::ToolHand(_curView, _curScene);
//				GlobalLogger::instance()->logDebug("GlobalToolManager: Selected Tool GtHand.");
//				break;
//			}
            case GtRect: {
                    clearTool();
                    _curTool = new graphics::ToolRect(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("GlobalToolManager: Selected Tool GtRect.");
                    break;
                }
            case GtPolygon: {
                    clearTool();
                    _curTool = new graphics::ToolPolygon(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("GlobalToolManager: Selected Tool GtPolygon.");
                    break;
                }
            case GtSinglePoint: {
                    clearTool();
                    _curTool = new graphics::ToolSinglePoint(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("GlobalToolManager: Selected Tool GtSinglePoint.");
                    break;
                }
            case GtEllipse: {
                    clearTool();
                    _curTool = new graphics::ToolEllipse(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("GlobalToolManager: Selected Tool GtEllipse.");
                    break;
                }
            case GtBoundingBox: {
                    clearTool();
                    _curTool = new graphics::ToolBoundingBox(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("GlobalToolManager: Selected Tool GtBoundingBox.");
                    break;
                }
            case GtBrush: {
                    clearTool();
                    _curTool = new graphics::ToolBrush(_curView, _curScene);
                    GlobalLogger::instance()->logDebug("GlobalToolManager: Selected Tool GtBrush.");
                    break;
                }
            default:
                GlobalLogger::instance()->logWarning("GlobalToolManager: Aborted tool selection due to unknown tool type.");
                return false;
        }
        _curToolId = tool;
        emit toolSelected(tool, _resetFlag);
        _resetFlag = false;

        if(_curTool != NULL) {
            _curTool->toolActivate();
        }

        return true;
    }

    bool GlobalToolManager::selectToolDefault() {
        GlobalLogger::instance()->logDebug("GlobalToolManager: Selecting default tool.");
        return selectTool(GtPointer);
    }

    GlobalToolManager::SelGraphicsTool GlobalToolManager::curToolId() const {
        return _curToolId;
    }

    graphics::GraphicsTool *GlobalToolManager::curTool() {
        if (_resetFlag) {
            GlobalLogger::instance()->logDebug("GlobalToolManager: Restoring current tool after reset.");
            selectTool(_curToolId);
        }
        return _curTool;
    }

    bool GlobalToolManager::hasTool() const {
        return ((_resetFlag && _curToolId != GtNone) || _curTool != NULL);
    }

    void GlobalToolManager::resetAll() {
        GlobalLogger::instance()->logDebug("GlobalToolManager: Resetting tool config.");
        if (_curTool != NULL) {
            _curTool->toolDeactivate();
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
            GlobalLogger::instance()->logDebug(QString("GlobalToolManager: Triggering context menu for [%1].").arg(anno->annoId()));
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

    void extractImage(const cv::Mat &src, cv::Rect &rect, cv::Mat &res) {
        double dYStart, dYFinish;
        double dXStart, dXFinish;
        dYStart = (rect.y >= 0) ? rect.y : 0;
        dYFinish = (rect.y + rect.height) <= src.rows ? (rect.y + rect.height) : src.rows;
        dXStart = (rect.x >= 0) ? rect.x : 0;
        dXFinish = (rect.x + rect.width) <= src.cols ? (rect.x + rect.width) : src.cols;

        rect.x = dXStart;
        rect.y = dYStart;
        rect.width = dXFinish - dXStart;
        rect.height = dYFinish - dYStart;

        cv::Mat tmp = src.rowRange(dYStart, dYFinish);
        res = tmp.colRange(dXStart, dXFinish);
    }

    void GlobalToolManager::runGrabCut() {
        /*
         * anna: Here we process the current annotation (-> segmentation)
         * We give the bounding box rectangle and/or the FG/BG mask to grabCut algorithm
         * The resulting picture is shown
         * The resulting segmentation is saved to "binary" image (mask)
         */
        GlobalProjectManager *pm = GlobalProjectManager::instance();
        anno::dt::Annotation *anno = pm->selectedAnno();
        if(anno != NULL) {
            anno::dt::Segmentation *segm = dynamic_cast<anno::dt::Segmentation *>(anno);
            if(segm != NULL) {
                anno::dt::AnnoShape *annoShape = segm->shape();
                if(!annoShape) {
                    return;
                }

                bool bFGPath = false, bBGPath = false;
                const QPainterPath fgPath = segm->getFGPath();
                bFGPath = !fgPath.isEmpty();
                const QPainterPath bgPath = segm->getBGPath();
                bBGPath = !bgPath.isEmpty();

                anno::dt::AnnoShapeType eAnnoShapeType = annoShape->shapeType();
                if(anno::dt::ASTypeBoundingBox == eAnnoShapeType) {
                    QPixmap qPm = _curScene->annoPixmap()->pixmap();
                    QImage qImg = qPm.toImage();
                    cv::Mat inputWholeImg = qimage2mat(qImg);

                    QRectF boundBoxRect = annoShape->boundingRect();

                    double dXInflate = 2 * boundBoxRect.width(), dYInflate = 2 * boundBoxRect.height();

                    cv::Rect rcBBRectInflated( boundBoxRect.x() - dXInflate, boundBoxRect.y() - dYInflate, boundBoxRect.width() + 2 * dXInflate, boundBoxRect.height() + 2 * dYInflate);
                    cv::Mat inputExtractedImg;
                    extractImage(inputWholeImg, rcBBRectInflated, inputExtractedImg);
                    // rcBBRectInflated could be changed in extractImage method, now it contains the actual extracted rectangle

                    cv::Rect rcRectForSegm(boundBoxRect.x() - rcBBRectInflated.x, boundBoxRect.y() - rcBBRectInflated.y, boundBoxRect.width(), boundBoxRect.height());
                    cv::Rect rcRealRect;// rcRealRect is the real min-max bounding rect of the segmentation

                    InteractiveGrabcut *grabCut = segm->provideGrabCutContext(inputExtractedImg, rcRectForSegm);

                    if(bFGPath || bBGPath) {
                        grabCut->updateMask(fgPath, bgPath, rcBBRectInflated.x, rcBBRectInflated.y);    // probably new paths
                    }

                    cv::Mat imgMaskResult;	//image mask for saving
                    cv::Mat resultImg = grabCut->execute(rcRealRect, imgMaskResult, /*bReuseMask ||*/ bFGPath || bBGPath);

                    // convert rcRealRect to global coordinates
                    QRect realRect(boundBoxRect.x() - dXInflate + rcRealRect.x, boundBoxRect.y() - dYInflate + rcRealRect.y, rcRealRect.width, rcRealRect.height);

                    cv::Mat resultExtracted;
                    extractImage(resultImg, rcRectForSegm, resultExtracted);
                    QImage *qImgRes = Mat2QImage(resultExtracted);

//					cv::Mat maskExtracted;
//					extractImage(maskResult, rcRectForSegm, maskExtracted);
//					//QImage* qMaskRes = Mat2QImage(maskExtracted);
//					segm->setMask(maskExtracted/*qMaskRes*/);

                    cv::Mat imgMaskExtracted;
                    extractImage(imgMaskResult, rcRectForSegm, imgMaskExtracted);
                    QImage *qImgMaskRes = Mat2QImage(imgMaskExtracted);

                    ((anno::dt::AnnoBoundingBox *)(segm->shape()))->setImage(qImgRes);
                    ((anno::dt::AnnoBoundingBox *)(segm->shape()))->setMask(qImgMaskRes);
                    ((anno::dt::AnnoBoundingBox *)(segm->shape()))->setRealBoundRect(realRect);

                    _curScene->selectShape(segm->annoId());	// refresh
                }
            }
        }
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
