#ifndef GLOBALINFO_H_
#define GLOBALINFO_H_

#include <QObject>
#include <QString>

class GlobalInfo {
        // public static infos
    public:
        static const QString compileQtVersion;
        static const QString compileDate;
        static const QString compileTime;

        static const bool isWin;
        static const bool isUnix;
        static const bool isMac;
        static const bool isRelease;
        static const bool isDebug;
        static const bool isExperimental;

        static const quint8 appVersionMajor;
        static const quint8 appVersionMid;
        static const quint16 appVersionMinor;

        static const QString appName;
        static const QString appSubtitle;
        static const QString appAuthor;
        static const QString appBuildInfo;
        static const QString orgName;
        static const QString orgDep;
        static const QString orgUni;
        static const QString orgUniShort;
        static const QString orgDomain;
        static const QString orgWebsite;

        static QString osName();
        static QString compileDateTime();
        static QString appVersionString();

};


// inlining
// -------------------------------------------------------------
inline QString GlobalInfo::compileDateTime() {
    return QString("%1 %2").arg(compileDate).arg(compileTime);
}

inline QString GlobalInfo::appVersionString() {
    return QString("%1.%2.%3").arg(appVersionMajor).arg(appVersionMid).arg(appVersionMinor);
}
// -------------------------------------------------------------

#endif /*GLOBALINFO_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
