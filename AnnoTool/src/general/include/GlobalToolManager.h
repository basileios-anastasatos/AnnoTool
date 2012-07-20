#ifndef GLOBALTOOLMANAGER_H_
#define GLOBALTOOLMANAGER_H_

#include "GraphicsTool.h"
#include "RecentAttrValues.h"
#include <QUuid>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
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
                  fg_color(0, 0, 255), bg_color(255, 0, 0) {
            };

            InteractiveGrabcut(const cv::Mat &src_, const cv::Rect &rcBoundRect)
                : src(src_), mask(), fgd(), bgd(),
                  ldrag(false), rdrag(false), fg_color(0, 0, 255), bg_color(255, 0, 0), m_rcBoundRect(rcBoundRect) {
                mask = cv::Mat::ones(src_.size(), CV_8U) * cv::GC_PR_BGD;
            };

//	    static void events( int e, int x, int y, int flags, void* s )
//	    {
//	        InteractiveGrabcut* self = (InteractiveGrabcut*)s;
//	        cv::Point pt(x, y);
//	        switch(e)
//	        {
//	        case CV_EVENT_LBUTTONDOWN:
//	            self->ldrag = true;
//	            self->lstart = pt;
//	            break;
//	        case CV_EVENT_LBUTTONUP:
//	            self->ldrag = false;
//	            self->show();
//	            break;
//	        case CV_EVENT_RBUTTONDOWN:
//	            self->rdrag = true;
//	            self->rstart = pt;
//	            break;
//	        case CV_EVENT_RBUTTONUP:
//	            self->rdrag = false;
//	            self->show();
//	            break;
//	        case CV_EVENT_MOUSEMOVE:
//	            if(self->ldrag)
//	            {
//	                cv::line(self->mask, self->lstart, pt, cv::Scalar(cv::GC_FGD), 1);
//	                self->lstart = pt;
//	                //self->show();
//	            }
//	            else if(self->rdrag)
//	            {
//	                cv::line(self->mask, self->rstart, pt, cv::Scalar(cv::GC_BGD), 1);
//	                self->rstart = pt;
//	                //self->show();
//	            }
//	            break;
//	        default:
//	            break;
//	        };
//	    };

            cv::Mat getFGImage(cv::Rect &rcRealBoundRect) {
                cv::Mat scribbled_src = src.clone();
                const float alpha = 0.7f;
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
                            cv::circle(scribbled_src, cv::Point(x, y), 2, fg_color, -1);
                        } else if(mask.at<uchar>(y, x) == cv::GC_BGD) {
                            //cv::circle(scribbled_src, cv::Point(x, y), 2, bg_color, -1);
                        } else if(mask.at<uchar>(y, x) == cv::GC_PR_BGD) {
                            //cv::Vec3b& pix = scribbled_src.at<cv::Vec3b>(y, x);
                            //pix[0] = (uchar)(pix[0] * alpha + bg_color[0] * (1-alpha));
                            //pix[1] = (uchar)(pix[1] * alpha + bg_color[1] * (1-alpha));
                            //pix[2] = (uchar)(pix[2] * alpha + bg_color[2] * (1-alpha));
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
                            pix[0] = (uchar)(pix[0] * alpha + fg_color[0] * (1 - alpha));
                            pix[1] = (uchar)(pix[1] * alpha + fg_color[1] * (1 - alpha));
                            pix[2] = (uchar)(pix[2] * alpha + fg_color[2] * (1 - alpha));
                        }
                    }
                }
                rcRealBoundRect.x = minX;
                rcRealBoundRect.y = minY;
                rcRealBoundRect.width = maxX - minX;
                rcRealBoundRect.height = maxY - minY;
                return scribbled_src;
            }

            cv::Mat execute(cv::Rect &rcRealBoundRect) {
                std::cout << "computing..." << std::endl;
//	        cv::Mat tmp(src.size(),CV_8UC3, cv::Scalar(0,0,0));
                cv::grabCut(src, /*tmp*/mask, m_rcBoundRect, bgd, fgd, 1, cv::GC_INIT_WITH_RECT);
//	        mask &= tmp;
//	        for(int y = 0; y < mask.rows; y++)
//	         {
//	        	for(int x = 0; x < mask.cols; x++)
//	        	{
//	        		if (y >= nY && y <= (nY + nHeight) && x >= nX && x <= (nX + nWidth))
//	            		continue;
//
//	        		mask.at<uchar>(y, x) = cv::GC_BGD;
//	        	}
//	         }
//	        cv::grabCut(src, mask, cv::Rect(), bgd, fgd, 1, cv::GC_INIT_WITH_MASK);

                std::cout << "end." << std::endl;

                return getFGImage(rcRealBoundRect);
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
            auto_ptr<anno::InteractiveGrabcut> _grabCut;
            cv::Mat _resultImg;
            QImage *_qImgRes;

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
