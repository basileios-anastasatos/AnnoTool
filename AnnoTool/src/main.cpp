#include "AnnoToolMainWindow.h"
#include <QtGui>
#include <QApplication>
#include "importGlobals.h"
#include "IdlImporterPlugin.h"
#include "IdlExporterPlugin.h"

/*#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <string>
#include <vector>
#include <iostream>*/

/*
class InteractiveGrabcut {
    cv::Mat src;
    cv::Mat fgd, bgd;
    bool ldrag, rdrag;
    std::string name;
public:
    cv::Mat mask;
    //cv::Rect rect;
    cv::Point lstart, rstart;
    cv::Scalar fg_color, bg_color;

    InteractiveGrabcut()
        : src(), mask(), fgd(), bgd(),
          ldrag(false), rdrag(false), name("igc"),
          fg_color(0, 0, 255), bg_color(255, 0, 0)
    {};

    InteractiveGrabcut(const cv::Mat& src_)
        : src(src_), mask(), fgd(), bgd(),
          ldrag(false), rdrag(false), name("igc"), fg_color(0, 0, 255), bg_color(255, 0, 0)
    {
        mask = cv::Mat::ones(src_.size(), CV_8U) * cv::GC_PR_BGD;
    };

    void prepareWindow(const std::string name){
        this->name = name;
        cv::namedWindow(name);
        cv::setMouseCallback(name, events, this);
    };

    static void events( int e, int x, int y, int flags, void* s ){
        InteractiveGrabcut* self = (InteractiveGrabcut*)s;
        cv::Point pt(x, y);
        switch(e) {
        case CV_EVENT_LBUTTONDOWN:
            self->ldrag = true;
            self->lstart = pt;
            break;
        case CV_EVENT_LBUTTONUP:
            self->ldrag = false;
            self->show();
            break;
        case CV_EVENT_RBUTTONDOWN:
            self->rdrag = true;
            self->rstart = pt;
            break;
        case CV_EVENT_RBUTTONUP:
            self->rdrag = false;
            self->show();
            break;
        case CV_EVENT_MOUSEMOVE:
            if(self->ldrag) {
                cv::line(self->mask, self->lstart, pt, cv::Scalar(cv::GC_FGD), 1);
                self->lstart = pt;
                self->show();
            }
            else if(self->rdrag) {
                cv::line(self->mask, self->rstart, pt, cv::Scalar(cv::GC_BGD), 1);
                self->rstart = pt;
                self->show();
            }
            break;
        default:
            break;
        };
    };

    void show(){
        cv::Mat scribbled_src = src.clone();
        const float alpha = 0.7f;
        for(int y=0; y < mask.rows; y++){
            for(int x=0; x < mask.cols; x++){
                if(mask.at<uchar>(y, x) == cv::GC_FGD) {
                    cv::circle(scribbled_src, cv::Point(x, y), 2, fg_color, -1);
                } else if(mask.at<uchar>(y, x) == cv::GC_BGD) {
                    cv::circle(scribbled_src, cv::Point(x, y), 2, bg_color, -1);
                } else if(mask.at<uchar>(y, x) == cv::GC_PR_BGD) {
                    cv::Vec3b& pix = scribbled_src.at<cv::Vec3b>(y, x);
                    pix[0] = (uchar)(pix[0] * alpha + bg_color[0] * (1-alpha));
                    pix[1] = (uchar)(pix[1] * alpha + bg_color[1] * (1-alpha));
                    pix[2] = (uchar)(pix[2] * alpha + bg_color[2] * (1-alpha));
                } else if(mask.at<uchar>(y, x) == cv::GC_PR_FGD) {
                    cv::Vec3b& pix = scribbled_src.at<cv::Vec3b>(y, x);
                    pix[0] = (uchar)(pix[0] * alpha + fg_color[0] * (1-alpha));
                    pix[1] = (uchar)(pix[1] * alpha + fg_color[1] * (1-alpha));
                    pix[2] = (uchar)(pix[2] * alpha + fg_color[2] * (1-alpha));
                }
            }
        }
        cv::imshow(name, scribbled_src);
        //cv::imshow(name + "_FG", getFG());
    }

    void execute(){
        std::cout << "computing...";
        cv::grabCut(src, mask, cv::Rect(), bgd, fgd, 1, cv::GC_INIT_WITH_MASK);
        std::cout << "end." << std::endl;
    };

    cv::Mat getBinMask(){
        cv::Mat binmask(mask.size(), CV_8U);
        binmask = mask & 1;
        return binmask;
    };

    cv::Mat getFG(){
        cv::Mat fg = cv::Mat::zeros(src.size(), src.type());
        src.copyTo(fg, getBinMask());
        return fg;
    };

    template <class Func>
    void mainLoop(Func& func){
    	static bool bFirstTime = true;
        while(1){
            int key = cv::waitKey(1);
            if(key == '\x1b')
                break;

            func(this, key);

            if (bFirstTime)
            {
            	bFirstTime = false;
            	show();
            }

            if(' ' == key)
            	show();
        }
    };
};

void func(InteractiveGrabcut* self, int key)
{
	if(key == ' ')
	{
		self->execute();
	}
}*/

int main(int argc, char *argv[]) {
    /*const std::string input_filename("test.jpg");
    cv::Mat input_image = cv::imread(input_filename);

    InteractiveGrabcut igc(input_image);
    igc.prepareWindow("test");
    igc.mainLoop(func);*/


    GlobalLogger::instance()->logInfo(QString("%1 Version %2 is starting up, running on %3").arg(GlobalInfo::appName).arg(GlobalInfo::appVersionString()).arg(GlobalInfo::osName()));

    // Initialize Global Configuration
    // ------------------------------------------
    GlobalConfig::instance()->loadConfig();
    GlobalLogger::instance()->setLogLevel((logging::LogLevel)GlobalConfig::instance()->getInt("logging.loglevel"));
    // ------------------------------------------

    // Application Configuration
    // ------------------------------------------
    QApplication a(argc, argv);
    a.setApplicationName(GlobalInfo::appName);
    a.setApplicationVersion(GlobalInfo::appVersionString());
    a.setOrganizationName(GlobalInfo::orgName);
    a.setOrganizationDomain(GlobalInfo::orgDomain);
    // ------------------------------------------

    // Im- & Exporter Plugins
    //----------------------------------------------------------------
    GlobalImExportManager *ieman = GlobalImExportManager::instance();
    ieman->addImporter(new anno::IdlImporterPlugin());
    ieman->addExporter(new anno::IdlExporterPlugin());
    //----------------------------------------------------------------


    AnnoToolMainWindow w;
    w.show();
    a.connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(onAppClose()));
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    GlobalLogger::instance()->logDebug("entering QApplication loop");
    return a.exec();
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
