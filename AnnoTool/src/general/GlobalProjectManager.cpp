#include "include/GlobalProjectManager.h"
#include "importGlobals.h"
#include <QFileInfo>
#include <QDir>

namespace anno {
    GlobalProjectManager *GlobalProjectManager::_me = NULL;

    GlobalProjectManager::GlobalProjectManager() {
        _project = NULL;
        _classList = NULL;
        _fileList = NULL;
        _curSelFile = -1;
    }

    GlobalProjectManager::~GlobalProjectManager() {
        clear();
    }

    void GlobalProjectManager::setupManager() {
        _me = new GlobalProjectManager();
    }

    GlobalProjectManager *GlobalProjectManager::instance() {
        if (_me == NULL) {
            setupManager();
        }
        return _me;
    }

    dt::AnnoProject *GlobalProjectManager::project() {
        return _project;
    }

    dt::AnnoAvClassList *GlobalProjectManager::classes() {
        return _classList;
    }

    QList<dt::AnnoFileData *> *GlobalProjectManager::files() {
        return _fileList;
    }

    bool GlobalProjectManager::isValid() const {
        return (_project != NULL && !_project->uuid().isNull() && _classList != NULL && _fileList != NULL);
    }

    void GlobalProjectManager::clear() {
        if (_project != NULL) {
            delete _project;
            _project = NULL;
        }
        if (_classList != NULL) {
            delete _classList;
            _classList = NULL;
        }
        if (_fileList != NULL) {
            if (!_fileList->isEmpty()) {
                QListIterator<dt::AnnoFileData *> i(*_fileList);
                while (i.hasNext()) {
                    delete i.next();
                }
            }
            delete _fileList;
            _fileList = NULL;
        }
    }

    void GlobalProjectManager::setSelectedFileRow(int index) {
        if(index >= 0 && index < _fileList->size()) {
            _curSelFile = index;
        }
    }

    int GlobalProjectManager::selectedFileRow() const {
        return _curSelFile;
    }

    dt::AnnoFileData *GlobalProjectManager::selectedFile() {
        if(_curSelFile >= 0 && _curSelFile < _fileList->size()) {
            return _fileList->at(_curSelFile);
        } else {
            return NULL;
        }
    }

    QFileInfo GlobalProjectManager::relToAbs(const QFileInfo &file) const
    throw(IllegalStateException *) {
        if (!isValid()) {
            throw new IllegalStateException(__FILE__, __LINE__, "Absolute path cannot be determined. Invalid project data.");
        }

        QDir dir = projectDir();
        QFileInfo ret = QFileInfo(dir.absoluteFilePath(file.filePath()));
        return ret;
    }

    QFileInfo GlobalProjectManager::relToAbs(const QDir &dir) const
    throw(IllegalStateException *) {
        if (!isValid()) {
            throw new IllegalStateException(__FILE__, __LINE__, "Absolute path cannot be determined. Invalid project data.");
        }

        QDir prdir = projectDir();
        QFileInfo ret = QFileInfo(prdir.absoluteFilePath(dir.path()));
        return ret;
    }

    QFileInfo GlobalProjectManager::absToRel(const QFileInfo &file) const
    throw(IllegalStateException *) {
        if (!isValid()) {
            throw new IllegalStateException(__FILE__, __LINE__, "Relative path cannot be determined. Invalid project data.");
        }

        QDir dir = projectDir();
        QFileInfo ret = QFileInfo(dir.relativeFilePath(file.filePath()));
        return ret;
    }

    QFileInfo GlobalProjectManager::absToRel(const QDir &dir) const
    throw(IllegalStateException *) {
        if (!isValid()) {
            throw new IllegalStateException(__FILE__, __LINE__, "Relative path cannot be determined. Invalid project data.");
        }

        QDir prdir = projectDir();
        QFileInfo ret = QFileInfo(prdir.relativeFilePath(dir.path()));
        return ret;
    }

    QDir GlobalProjectManager::projectDir() const throw(IllegalStateException *) {
        if (!isValid()) {
            throw new IllegalStateException(__FILE__, __LINE__, "Project dir cannot be determined. Invalid project data.");
        }

        GlobalLogger::instance()->logDebug("Retrieving project directory");
        QFileInfo file(_project->filePath());
        return file.absoluteDir();
    }

    QString GlobalProjectManager::defAnnoFileName(const QUuid &uuid) {
        QString name("annotations_%1.%2");
        return name.arg(uuid.toString(), GlobalConfig::fileExt.annotations);
    }

    void GlobalProjectManager::loadClassDefs() throw(IOException *, XmlException *) {
        if (_project != NULL && _project->classPath() != NULL
                && !_project->classPath()->isEmpty()) {
            GlobalLogger::instance()->logDebug("Loading class defs.");
            QListIterator<QFileInfo> i(*_project->classPath());
            while (i.hasNext()) {
                QFileInfo fi = i.next();
                GlobalLogger::instance()->logDebug(QString("Trying load class definitions from [%1].").arg(fi.filePath()));
                if (fi.isSymLink()) {
                    GlobalLogger::instance()->logDebug(QString("Resolving symbolic link [%1].").arg(fi.filePath()));
                    fi = QFileInfo(fi.symLinkTarget());
                    GlobalLogger::instance()->logDebug(QString("Resolved link to [%1].").arg(fi.filePath()));
                }
                if (fi.isRelative()) {
                    GlobalLogger::instance()->logDebug(QString("Resolving relative path [%1].").arg(fi.filePath()));
                    fi = relToAbs(fi);
                    GlobalLogger::instance()->logDebug(QString("Resolved to path [%1].").arg(fi.filePath()));
                }
                if (fi.exists()) {
                    _classList->addFromFile(fi.absoluteFilePath());
                } else {
                    GlobalLogger::instance()->logWarning(QString("Given class definition file [%1] does not exist.").arg(fi.filePath()));
                }
            }
            GlobalLogger::instance()->logDebug("Finished loading class defs.");
        }
    }

    void GlobalProjectManager::loadAnnoFiles() throw(IOException *, XmlException *) {
        if (_project != NULL && _project->searchPath() != NULL
                && !_project->searchPath()->isEmpty()) {
            GlobalLogger::instance()->logDebug("Loading annotation files.");
            QListIterator<QFileInfo> i(*_project->searchPath());
            while (i.hasNext()) {
                QFileInfo fi = i.next();
                GlobalLogger::instance()->logDebug(QString("Trying load annotation files from [%1].").arg(fi.filePath()));
                if (fi.isSymLink()) {
                    GlobalLogger::instance()->logDebug(QString("Resolving symbolic link [%1].").arg(fi.filePath()));
                    fi = QFileInfo(fi.symLinkTarget());
                    GlobalLogger::instance()->logDebug(QString("Resolved link to [%1].").arg(fi.filePath()));
                }
                if (fi.isRelative()) {
                    GlobalLogger::instance()->logDebug(QString("Resolving relative path [%1].").arg(fi.filePath()));
                    fi = relToAbs(fi);
                    GlobalLogger::instance()->logDebug(QString("Resolved to path [%1].").arg(fi.filePath()));
                }
                if (fi.isDir()) {
                    if (fi.exists()) {
                        loadAnnoFilesFromDir(fi.absoluteFilePath());
                    } else {
                        GlobalLogger::instance()->logWarning(QString("Given annotation file directory [%1] does not exist.").arg(fi.filePath()));
                    }
                } else {
                    GlobalLogger::instance()->logWarning(QString("Given annotation search path [%1] does point to a valid directory.").arg(fi.filePath()));
                }
            }
            GlobalLogger::instance()->logDebug("Finished loading annotation files.");
        }
    }

    void GlobalProjectManager::loadAnnoFilesFromDir(QDir dir) throw(IOException *,
            XmlException *) {
        Q_ASSERT(dir.exist() && dir.isAbsolute());

        QStringList filters;
        filters
                << FileExtensions::asFilter(GlobalConfig::fileExt.annotations);
        QFileInfoList lst = dir.entryInfoList(filters, QDir::Files);

        if (!lst.isEmpty()) {
            GlobalLogger::instance()->logDebug(QString("Found %1 annotation files.").arg(lst.size()));
            QListIterator<QFileInfo> i(lst);
            while (i.hasNext()) {
                QFileInfo fi = i.next();
                GlobalLogger::instance()->logDebug(QString("Trying load annotation data from [%1].").arg(fi.filePath()));
                if (fi.exists() && fi.isFile()) {
                    //TODO think of handling symlinks here. further research must be done on this!
                    _fileList->append(dt::AnnoFileData::fromFile(fi.absoluteFilePath()));
                } else {
                    GlobalLogger::instance()->logWarning(QString("Given annotation file path [%1] seems to be invalid.").arg(fi.filePath()));
                }
            }
        } else {
            GlobalLogger::instance()->logWarning(QString("Given annotation search path [%1] does not contain any annotation files.").arg(dir.path()));
        }
    }

    void GlobalProjectManager::loadFromFile(const QString &path, bool loadSub)
    throw(IOException *, XmlException *, IllegalStateException *) {
        if (isValid()) {
            throw new IllegalStateException(__FILE__, __LINE__, "Cannot load from given file. Current data is already valid.");
        }

        _project = dt::AnnoProject::fromFile(path);
        _classList = new dt::AnnoAvClassList();
        _fileList = new QList<dt::AnnoFileData *>();

        if (loadSub) {
            //TODO change rel path handling such that rel paths start at project file dir.
            loadClassDefs();
            loadAnnoFiles();
        }
    }

    void GlobalProjectManager::saveToFile(bool saveSub) const throw(IOException *,
            IllegalStateException *, XmlException *) {
        if (!isValid()) {
            throw new IllegalStateException(__FILE__, __LINE__, "Cannot save current data. Invalid data.");
        }

        _project->writeToFile();
        if(saveSub) {
            QListIterator<dt::AnnoFileData *> i(*_fileList);
            while(i.hasNext()) {
                i.next()->writeToFile();
            }
        }
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
