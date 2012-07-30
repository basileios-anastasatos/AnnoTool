#ifndef GLOBALTOOLMANAGER_H_
#define GLOBALTOOLMANAGER_H_

#include "GraphicsTool.h"
#include "RecentAttrValues.h"
#include <QUuid>
#include <QPainterPath>

#include <cv.h>
#include <cxcore.h>
//#include <highgui.h>
#include <string>
#include <vector>
#include <iostream>
#include <memory>

class QGraphicsView;
class ShapeContextMenu;


using namespace std;

namespace anno {
    namespace graphics {
        class AnnoGraphicsScene;
    }
    namespace dt {
        class Annotation;
    }

    class InteractiveGrabcut {
            cv::Mat src;
            cv::Mat fgd, bgd;
            bool ldrag, rdrag;
            cv::Rect m_rcBoundRect;
            cv::Mat mask;
            cv::Point lstart, rstart;
            cv::Scalar fg_color, bg_color;

        public:

            InteractiveGrabcut()
                : src(), mask(), fgd(), bgd(),
                  ldrag(false), rdrag(false),
                  fg_color(0, 255, 0), bg_color(255, 0, 0) {
            };

            InteractiveGrabcut(const cv::Mat &src_, const cv::Rect &rcBoundRect)
                : src(src_), mask(), fgd(), bgd(),
                  ldrag(false), rdrag(false), fg_color(0, 255, 0), bg_color(255, 0, 0), m_rcBoundRect(rcBoundRect) {
                mask = cv::Mat::ones(src_.size(), CV_8U) * cv::GC_PR_BGD;
            };

            /*	    void setMask(bool bReuseMask, const cv::Mat& prevMask, int xOffset, int yOffset)
		    {
			for(int y = 0; y < mask.rows; y++)
				{
					for(int x = 0; x < mask.cols; x++)
					{
						if (y >= m_rcBoundRect.y && y < (m_rcBoundRect.y + m_rcBoundRect.height) && x >= m_rcBoundRect.x && x < (m_rcBoundRect.x + m_rcBoundRect.width) && bReuseMask)
							mask.at<uchar>(y, x) = prevMask.at<uchar>(y + yOffset, x + xOffset);
						else
							mask.at<uchar>(y, x) = cv::GC_BGD;
					}
				}
		    }*/

            void updateMask(const QPainterPath &fgPath, const QPainterPath &bgPath, int xOffset, int yOffset) {
                for(int y = 0; y < mask.rows; y++) {
                    for(int x = 0; x < mask.cols; x++) {
                        cv::Point pt(x + xOffset, y + yOffset);
                        if(fgPath.contains(QPointF(x + xOffset, y + yOffset))) {
                            mask.at<uchar>(y, x) = cv::GC_FGD;
                        }
                        if(bgPath.contains(QPointF(x + xOffset, y + yOffset))) {
                            mask.at<uchar>(y, x) = cv::GC_BGD;
                        }
                    }
                }
            };

            cv::Mat getFGImage(cv::Rect &rcRealBoundRect, cv::Mat &binMaskRes) {
                cv::Mat scribbled_src = src.clone();
                binMaskRes = src.clone();
                const float alphaFG = 0.5f;
                const float alphaPFG = 0.8f;
                int minX = mask.cols + 1, maxX = -1, minY = mask.rows + 1, maxY = -1;
                for(int y = 0; y < mask.rows; y++) {
                    for(int x = 0; x < mask.cols; x++) {
                        if(mask.at<uchar>(y, x) == cv::GC_FGD) {
                            if (minX > x) {
                                minX = x;
                            }
                            if (minY > y) {
                                minY = y;
                            }
                            if (maxX < x) {
                                maxX = x;
                            }
                            if (maxY < y) {
                                maxY = y;
                            }

                            cv::Vec3b &pix = scribbled_src.at<cv::Vec3b>(y, x);
                            pix[0] = (uchar)(pix[0] * alphaFG + fg_color[0] * (1 - alphaFG));
                            pix[1] = (uchar)(pix[1] * alphaFG + fg_color[1] * (1 - alphaFG));
                            pix[2] = (uchar)(pix[2] * alphaFG + fg_color[2] * (1 - alphaFG));

                            cv::Vec3b &pix1 = binMaskRes.at<cv::Vec3b>(y, x);
                            pix1[0] = 0;
                            pix1[1] = 0;
                            pix1[2] = 255;
                        } else if(mask.at<uchar>(y, x) == cv::GC_BGD) {
                            cv::Vec3b &pix = binMaskRes.at<cv::Vec3b>(y, x);
                            pix[0] = 255;
                            pix[1] = 0;
                            pix[2] = 0;
                        } else if(mask.at<uchar>(y, x) == cv::GC_PR_BGD) {
                            cv::Vec3b &pix = binMaskRes.at<cv::Vec3b>(y, x);
                            pix[0] = 127;
                            pix[1] = 0;
                            pix[2] = 0;
                        } else if(mask.at<uchar>(y, x) == cv::GC_PR_FGD) {
                            if (minX > x) {
                                minX = x;
                            }
                            if (minY > y) {
                                minY = y;
                            }
                            if (maxX < x) {
                                maxX = x;
                            }
                            if (maxY < y) {
                                maxY = y;
                            }

                            cv::Vec3b &pix = scribbled_src.at<cv::Vec3b>(y, x);
                            pix[0] = (uchar)(pix[0] * alphaPFG + fg_color[0] * (1 - alphaPFG));
                            pix[1] = (uchar)(pix[1] * alphaPFG + fg_color[1] * (1 - alphaPFG));
                            pix[2] = (uchar)(pix[2] * alphaPFG + fg_color[2] * (1 - alphaPFG));

                            cv::Vec3b &pix1 = binMaskRes.at<cv::Vec3b>(y, x);
                            pix1[0] = 0;
                            pix1[1] = 0;
                            pix1[2] = 127;
                        }
                    }
                }
                rcRealBoundRect.x = minX;
                rcRealBoundRect.y = minY;
                rcRealBoundRect.width = maxX - minX;
                rcRealBoundRect.height = maxY - minY;
                return scribbled_src;
            }

            cv::Mat execute(cv::Rect &rcRealBoundRect, /*cv::Mat& maskRes,*/ cv::Mat &binMaskRes, bool bUseMask) {
                std::cout << "computing..." << std::endl;

                if(!bUseMask) {
                    cv::grabCut(src, mask, m_rcBoundRect, bgd, fgd, 1, cv::GC_INIT_WITH_RECT);
                } else {
//	        	cv::Mat tmp(src.size(),CV_8UC3, cv::Scalar(0,0,0));
//	        	cv::grabCut(src, tmp, m_rcBoundRect, bgd, fgd, 1, cv::GC_INIT_WITH_RECT);
//	        	mask &= tmp;
                    for(int y = 0; y < mask.rows; y++) {
                        for(int x = 0; x < mask.cols; x++) {
                            if (y >= m_rcBoundRect.y && y < (m_rcBoundRect.y + m_rcBoundRect.height) && x >= m_rcBoundRect.x && x < (m_rcBoundRect.x + m_rcBoundRect.width)) {
                                continue;
                            } else {
                                mask.at<uchar>(y, x) = cv::GC_BGD;
                            }
                        }
                    }
                    cv::grabCut(src, mask, cv::Rect(), bgd, fgd, 1, cv::GC_INIT_WITH_MASK);
                }

                std::cout << "end." << std::endl;

                //maskRes = mask.clone();

                return getFGImage(rcRealBoundRect, binMaskRes);
            };

//	    cv::Mat getBinMask()
//	    {
//	        cv::Mat binmask(mask.size(), CV_8U);
//	        binmask = mask & 1;
//	        return binmask;
//	    };
//
//	    cv::Mat getFG()
//	    {
//	        cv::Mat fg = cv::Mat::zeros(src.size(), src.type());
//	        src.copyTo(fg, getBinMask());
//	        return fg;
//	    };

    };

    class GlobalToolManager : public QObject {
            Q_OBJECT

            // enum of selectable tools
        public:
            enum SelGraphicsTool {
                GtNone, GtPointer, GtSinglePoint, GtRect, GtPolygon, GtEllipse, GtBoundingBox, GtBrush, GtNone2
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

            void runGrabCut();

        signals:
            void toolSelected(anno::GlobalToolManager::SelGraphicsTool tool, bool reset);


    };

}

#endif /*GLOBALTOOLMANAGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
