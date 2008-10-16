#include "include/GlobalInfo.h"
#include <QSysInfo>

const QString GlobalInfo::compileQtVersion(QT_VERSION_STR);
const QString GlobalInfo::compileDate(__DATE__);
const QString GlobalInfo::compileTime(__TIME__);

#if defined(Q_OS_MAC) && !defined(Q_OS_WIN32) && !defined(Q_OS_LINUX)
const bool GlobalInfo::isWin = false;
const bool GlobalInfo::isUnix = false;
const bool GlobalInfo::isMac = true;
#elif defined(Q_OS_WIN32) && !defined(Q_OS_MAC) && !defined(Q_OS_UNIX)
const bool GlobalInfo::isWin = true;
const bool GlobalInfo::isUnix = false;
const bool GlobalInfo::isMac = false;
#elif defined(Q_OS_UNIX) && !defined(Q_OS_WIN32) && !defined(Q_OS_MAC)
const bool GlobalInfo::isWin = false;
const bool GlobalInfo::isUnix = true;
const bool GlobalInfo::isMac = false;
#else
#error "OS configuration is invalid!"
#endif



#if defined(QT_DEBUG)
const bool GlobalInfo::isRelease = false;
const bool GlobalInfo::isDebug = true;
#else
const bool GlobalInfo::isRelease = true;
const bool GlobalInfo::isDebug = false;
#endif

const quint8 GlobalInfo::appVersionMajor		= 0;
const quint8 GlobalInfo::appVersionMid			= 3;
const quint16 GlobalInfo::appVersionMinor		= 5;

const QString GlobalInfo::appName("AnnoTool");
const QString GlobalInfo::appSubtitle(QObject::tr("Advanced Image Annotation"));
const QString GlobalInfo::appAuthor("Oliver Schwahn");
const QString GlobalInfo::orgName(QObject::tr("Multimodal Interactive Systems (MIS)"));
const QString GlobalInfo::orgDep(QObject::tr("Department of Computer Science"));
const QString GlobalInfo::orgUni(QObject::tr("Darmstadt University of Technology"));
const QString GlobalInfo::orgUniShort("TU-Darmstadt");
const QString GlobalInfo::orgDomain("mis.informatik.tu-darmstadt.de");
const QString GlobalInfo::orgWebsite("http://www.mis.informatik.tu-darmstadt.de");


QString GlobalInfo::osName() {
#if defined(Q_OS_MAC)
    switch(QSysInfo::MacintoshVersion) {
        case QSysInfo::MV_9:
            return QString("Mac OS 9");
        case QSysInfo::MV_CHEETAH:
            return QString("Mac OS X 10.0 - Cheetah");
        case QSysInfo::MV_PUMA:
            return QString("Mac OS X 10.1 - Puma");
        case QSysInfo::MV_JAGUAR:
            return QString("Mac OS X 10.2 - Jaguar");
        case QSysInfo::MV_PANTHER:
            return QString("Mac OS X 10.3 - Panther");
        case QSysInfo::MV_TIGER:
            return QString("Mac OS X 10.4 - Tiger");
        case QSysInfo::MV_LEOPARD:
            return QString("Mac OS X 10.5 - Leopard");
        default:
            return QString("Mac OS");
    }
#elif defined(Q_OS_WIN32) || defined(Q_OS_WINCE)
    switch(QSysInfo::WindowsVersion) {
        case QSysInfo::WV_32s:
            return QString("Windows 3.1");
        case QSysInfo::WV_95:
            return QString("Windows 95");
        case QSysInfo::WV_98:
            return QString("Windows 98");
        case QSysInfo::WV_Me:
            return QString("Windows ME");
        case QSysInfo::WV_NT:
            return QString("Windows NT");
        case QSysInfo::WV_2000:
            return QString("Windows 2000");
        case QSysInfo::WV_XP:
            return QString("Windows XP");
        case QSysInfo::WV_2003:
            return QString("Windows 2003");
        case QSysInfo::WV_VISTA:
            return QString("Windows Vista");
        case QSysInfo::WV_CE:
            return QString("Windows CE");
        case QSysInfo::WV_CENET:
            return QString("Windows CE .NET");
        case QSysInfo::WV_CE_5:
            return QString("Windows CE 5");
        case QSysInfo::WV_CE_6:
            return QString("Windows CE 6");
        default:
            return QString("Windows");
    }
#elif defined(Q_OS_LINUX)
    return QString("Linux System");
#elif defined(Q_OS_UNIX)
    return QString("Unix System");
#else
    return QString("Unknown System");
#endif
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
