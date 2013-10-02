#include <iostream>

#include "include/GlobalProjectManager.h"
#include "importGlobals.h"
#include "AnnoFileListSorter.h"

#include <QFileInfo>
#include <QDir>
#include <QThread>

// MA
#include <AnnoRectangle.h>

namespace anno {
    GlobalProjectManager *GlobalProjectManager::_me = NULL;

    void GlobalProjectManager::setupManager() {
        _me = new GlobalProjectManager();
    }

    GlobalProjectManager *GlobalProjectManager::instance() {
        if (_me == NULL) {
            setupManager();
        }
        return _me;
    }

    void GlobalProjectManager::onAnnoFileModifyChange(::anno::dt::AnnoFileData *annoFile,
            bool prevState, bool curState) {
        GlobalLogger::instance()->logDebug(QString("GlobalProjectManager::onAnnoFileModifyChange: state is %1, was %2").arg(curState).arg(prevState));
        if (annoFile != NULL) {
            int idx = -1;
            if (curState && !_fileListMod->contains(annoFile)) {
                _fileListMod->append(annoFile);
            } else if (!curState && (idx = _fileListMod->indexOf(annoFile)) >= 0) {
                _fileListMod->removeAt(idx);
            }
        }
    }

    QList<const dt::AnnoFileData *> GlobalProjectManager::files() const {
        QList<const dt::AnnoFileData *> tmp;
        QListIterator<dt::AnnoFileData *> i(*_fileList);
        while (i.hasNext()) {
            tmp.append(i.next());
        }
        return tmp;
    }

    QList<const dt::AnnoFileData *> GlobalProjectManager::modFiles() const {
        QList<const dt::AnnoFileData *> tmp;
        QListIterator<dt::AnnoFileData *> i(*_fileListMod);
        while (i.hasNext()) {
            tmp.append(i.next());
        }
        return tmp;
    }

    bool GlobalProjectManager::isValid() const {
        return (_project != NULL && !_project->uuid().isNull() && _filterMan != NULL && _classList != NULL && _fileList != NULL && _fileListMod != NULL);
    }

    void GlobalProjectManager::clear() {
        if (_project != NULL) {
            delete _project;
            _project = NULL;
        }
        if(_filterMan != NULL) {
            delete _filterMan;
            _filterMan = NULL;
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
        if (_fileListMod != NULL) {
            delete _fileListMod;
            _fileListMod = NULL;
        }
        _curSelFile = -1;
        _curSelAnno = -1;
        emit projectClosed();
    }

    void GlobalProjectManager::newEmpty(const QString &projectPath, const QUuid &projectUuid)
    throw(IllegalStateException *) {
        if (isValid()) {
            throw new IllegalStateException(__FILE__, __LINE__, "There is aleady a current project loaded.");
        }

        _project = new dt::AnnoProject(projectPath, projectUuid);
        _filterMan = new filter::AnnoFilterManager(_project);
        _classList = new dt::AnnoAvClassList();
        _fileList = new QList<dt::AnnoFileData *>();
        _fileListMod = new QList<dt::AnnoFileData *>();
        _curSelFile = -1;
        _curSelAnno = -1;

        emit projectNew();
    }

    void GlobalProjectManager::setupAllSignals() {
        QMutableListIterator<dt::AnnoFileData *> it(*_fileList);
        while (it.hasNext()) {
            dt::AnnoFileData *annoFile = it.next();
            setupSignals(annoFile);
        }
    }

    void GlobalProjectManager::setupSignals(int index) {
        dt::AnnoFileData *annoFile  = getAnnoFile(index);
        setupSignals(annoFile);
    }

    void GlobalProjectManager::setupSignals(dt::AnnoFileData *annoFile) {
        if(annoFile != NULL) {
            annoFile->setNotify(true);
            annoFile->setNotifyOnChange(true);
            annoFile->setNotifyAnno(true);
            annoFile->setNotifyOnAnnoChange(true);
            annoFile->resetModifiedState(true);
        }
    }

    void GlobalProjectManager::addAnnoFile(dt::AnnoFileData *annoFile, bool newFile) {
        if (annoFile != NULL && isValid()) {
            GlobalLogger::instance()->logDebug("Added anno file.");
            if (!connect(annoFile, SIGNAL(modifyStateChanged(::anno::dt::AnnoFileData *, bool, bool)), this, SLOT(onAnnoFileModifyChange(::anno::dt::AnnoFileData *, bool, bool)))) {
                GlobalLogger::instance()->logError("CONNECT-ERROR: GlobalProjectManager::addAnnoFile");
            }
            _fileList->append(annoFile);
            if(newFile) {
                setupSignals(annoFile);
                annoFile->setModified(true);
            }
        }
    }

    dt::AnnoFileData *GlobalProjectManager::selectedFile() {
        if (_fileList != NULL && _curSelFile >= 0 && _curSelFile < _fileList->size()) {
            return _fileList->at(_curSelFile);
        } else {
            return NULL;
        }
    }

    const dt::AnnoFileData *GlobalProjectManager::selectedFile() const {
        if (_fileList != NULL && _curSelFile >= 0 && _curSelFile < _fileList->size()) {
            return _fileList->at(_curSelFile);
        } else {
            return NULL;
        }
    }

    dt::Annotation *GlobalProjectManager::selectedAnno() {
        if (_curSelAnno >= 0 && selectedFile() != NULL && _curSelAnno < selectedFile()->annoCount()) {
            return selectedFile()->getAnnotation(_curSelAnno);
        } else {
            return NULL;
        }
    }

    const dt::Annotation *GlobalProjectManager::selectedAnno() const {
        if (_curSelAnno >= 0 && selectedFile() != NULL && _curSelAnno < selectedFile()->annoCount()) {
            return selectedFile()->getAnnotation(_curSelAnno);
        } else {
            return NULL;
        }
    }

    void GlobalProjectManager::setSelectedFileRow(int index) {
        std::cout << "GlobalProjectManager::setSelectedFileRow" << std::endl;

        if (_fileList != NULL && index >= 0 && index < _fileList->size()) {
            dt::AnnoFileData *file = _fileList->at(index);
            if (file != NULL) {
                resetSelectedFile();
                bool connectOk = true;
                connectOk = connectOk && connect(file, SIGNAL(modified(::anno::dt::AnnoFileData *)), this, SIGNAL(curAnnoFileModified(::anno::dt::AnnoFileData *)));
                connectOk = connectOk && connect(file, SIGNAL(modifyReset(::anno::dt::AnnoFileData *)), this, SIGNAL(curAnnoFileModifyReset(::anno::dt::AnnoFileData *)));
                connectOk = connectOk && connect(file, SIGNAL(modifyStateChanged(::anno::dt::AnnoFileData *, bool, bool)), this, SIGNAL(curAnnoFileModifyStateChanged(::anno::dt::AnnoFileData *, bool, bool)));
                connectOk = connectOk && connect(file, SIGNAL(annoAdded(::anno::dt::Annotation *)), this, SIGNAL(curAnnoFile_annoAdded(::anno::dt::Annotation *)));
                connectOk = connectOk && connect(file, SIGNAL(annoRemoved(QUuid)), this, SIGNAL(curAnnoFile_annoRemoved(QUuid)));

                if (!connectOk) {
                    GlobalLogger::instance()->logError("CONNECT-ERROR: GlobalProjectManager::setSelectedFileRow(int)");
                }

                _curSelFile = index;
                emit curAnnoFileSelChanged(index, file->imageUuid(), file);


                /* MA: select the annotation closest to the last selection in the previous image
                   (useful when annotating sequences) */

                int lastAnnoRectIdx = -1;
                double bestDist = 1e6;

                for (int aidx = 0; aidx < file->annoCount(); ++aidx) {
                    dt::Annotation *anno = file->getAnnotation(aidx);

                    if (anno->shape()->shapeType() == anno::dt::ASTypeRectangle) {
                        //lastAnnoRectIdx = aidx;
                        anno::dt::AnnoRectangle *rect = (anno::dt::AnnoRectangle *)anno->shape();
                        QRectF boundingRect = rect->boundingRect();

                        QPointF curCenter = boundingRect.center();

                        QPointF curOffset = curCenter - _lastSelRectCenter;

                        double curDist = sqrt(curOffset.x() * curOffset.x() + curOffset.y() * curOffset.y());

                        if (curDist <  bestDist) {
                            lastAnnoRectIdx = aidx;
                            bestDist = curDist;
                        }


                    }
                }

                if (lastAnnoRectIdx != -1) {
                    setSelectedAnnoRow(lastAnnoRectIdx);
                }

                /* MA: select the first annotation */
                //setSelectedAnnoRow(0);

            }
        } else if(index == -1) {
            resetSelectedFile();
            _curSelFile = -1;
            emit curAnnoFileSelChanged(-1, QUuid(), NULL);
        }
    }

    void GlobalProjectManager::setSelectedAnnoRow(int index) {
        //std::cout << "GlobalProjectManager::setSelectedAnnoRow" << std::endl;

        if (index >= 0 && selectedFile() != NULL && index < selectedFile()->annoCount()) {
            dt::Annotation *anno = selectedFile()->getAnnotation(index);
            if (anno != NULL) {
                resetSelectedAnno();
                bool connectOk = true;
                connectOk = connectOk && connect(anno, SIGNAL(modified(::anno::dt::Annotation *)), this, SIGNAL(curAnnoModified(::anno::dt::Annotation *)));
                connectOk = connectOk && connect(anno, SIGNAL(modifyReset(::anno::dt::Annotation *)), this, SIGNAL(curAnnoModifyReset(::anno::dt::Annotation *)));
                connectOk = connectOk && connect(anno, SIGNAL(modifyStateChanged(::anno::dt::Annotation *, bool, bool)), this, SIGNAL(curAnnoModifyStateChanged(::anno::dt::Annotation *, bool, bool)));

                if (!connectOk) {
                    GlobalLogger::instance()->logError("CONNECT-ERROR: GlobalProjectManager::setSelectedAnnoRow(int)");
                }

                _curSelAnno = index;


                /* MA BEGIN: keep track of potential parents for Pose Mode */

                if (anno->shape()) {
                    if (anno->shape()->shapeType() != anno::dt::ASTypeSinglePoint) {
                        _lastSelNotPointAnno = index;
                        //std::cout << "_lastSelNotPointAnno: " << _lastSelNotPointAnno << std::endl;
                    }

                    /* */
                    if (anno->shape()->shapeType() == anno::dt::ASTypeRectangle) {
                        anno::dt::AnnoRectangle *rect = (anno::dt::AnnoRectangle *)anno->shape();
                        QRectF boundingRect = rect->boundingRect();

                        _lastSelRectCenter = boundingRect.center();

                        //std::cout << _lastSelRectCenter.x() <<  " " << _lastSelRectCenter.y() << std::endl;
                    }

                } else {
                    assert(false && "no shape?");
                }
                /* MA END */



                emit curAnnoSelChanged(index, anno->annoId(), anno);
            }
        } else if (index == -1) {
            resetSelectedAnno();
            _curSelAnno = -1;
            emit curAnnoSelChanged(-1, QUuid(), NULL);
        }
    }

    void GlobalProjectManager::setSelectedFileRow(const QUuid &uuid) {
        if (_fileList != NULL) {
            int index = -1;
            int j = 0;
            QListIterator<dt::AnnoFileData *> i(*_fileList);
            while(i.hasNext()) {
                dt::AnnoFileData *cur = i.next();
                if(cur->imageUuid() == uuid) {
                    index = j;
                    break;
                }
                ++j;
            }
            setSelectedFileRow(index);
        }
    }
    void GlobalProjectManager::setSelectedAnnoRow(const QUuid &uuid) {
        dt::AnnoFileData *curFile = selectedFile();
        if (curFile != NULL) {
            int index = -1;
            int j = 0;
            QListIterator<dt::Annotation *> i = curFile->getAnnoIterator();
            while(i.hasNext()) {
                dt::Annotation *cur = i.next();
                if(cur->annoId() == uuid) {
                    index = j;
                    break;
                }
                ++j;
            }
            GlobalLogger::instance()->logDebug(QString("Selected annotation by uuid [%1], has index [%2]").arg(uuid.toString()).arg(j));
            setSelectedAnnoRow(index);
        }
    }

    void GlobalProjectManager::resetSelectedFile() {
        dt::AnnoFileData *file = selectedFile();
        if (file != NULL) {
            bool dconOk = true;
            dconOk &= disconnect(file, SIGNAL(modified(::anno::dt::AnnoFileData *)), this, SIGNAL(curAnnoFileModified(::anno::dt::AnnoFileData *)));
            dconOk &= disconnect(file, SIGNAL(modifyReset(::anno::dt::AnnoFileData *)), this, SIGNAL(curAnnoFileModifyReset(::anno::dt::AnnoFileData *)));
            dconOk &= disconnect(file, SIGNAL(modifyStateChanged(::anno::dt::AnnoFileData *, bool, bool)), this, SIGNAL(curAnnoFileModifyStateChanged(::anno::dt::AnnoFileData *, bool, bool)));
            dconOk &= disconnect(file, SIGNAL(annoAdded(::anno::dt::Annotation *)), this, SIGNAL(curAnnoFile_annoAdded(::anno::dt::Annotation *)));
            dconOk &= disconnect(file, SIGNAL(annoRemoved(QUuid)), this, SIGNAL(curAnnoFile_annoRemoved(QUuid)));

            if (!dconOk) {
                GlobalLogger::instance()->logError("DISCONNECT-ERROR: GlobalProjectManager::resetSelectedFile() - during file discon");
            }

            dconOk = true;
            dt::Annotation *anno = selectedAnno();
            if (anno != NULL) {
                dconOk &= disconnect(anno, SIGNAL(modified(::anno::dt::Annotation *)), this, SIGNAL(curAnnoModified(::anno::dt::Annotation *)));
                dconOk &= disconnect(anno, SIGNAL(modifyReset(::anno::dt::Annotation *)), this, SIGNAL(curAnnoModifyReset(::anno::dt::Annotation *)));
                dconOk &= disconnect(anno, SIGNAL(modifyStateChanged(::anno::dt::Annotation *, bool, bool)), this, SIGNAL(curAnnoModifyStateChanged(::anno::dt::Annotation *, bool, bool)));
            }

            if (!dconOk) {
                GlobalLogger::instance()->logError("DISCONNECT-ERROR: GlobalProjectManager::resetSelectedFile() - during anno dcon");
            }
        }
        _curSelFile = -1;
        _curSelAnno = -1;
    }

    void GlobalProjectManager::resetSelectedAnno() {
        dt::Annotation *anno = selectedAnno();
        if (anno != NULL) {
            bool dconOk = true;
            dconOk &= disconnect(anno, SIGNAL(modified(::anno::dt::Annotation *)), this, SIGNAL(curAnnoModified(::anno::dt::Annotation *)));
            dconOk &= disconnect(anno, SIGNAL(modifyReset(::anno::dt::Annotation *)), this, SIGNAL(curAnnoModifyReset(::anno::dt::Annotation *)));
            dconOk &= disconnect(anno, SIGNAL(modifyStateChanged(::anno::dt::Annotation *, bool, bool)), this, SIGNAL(curAnnoModifyStateChanged(::anno::dt::Annotation *, bool, bool)));

            if (!dconOk) {
                GlobalLogger::instance()->logError("DISCONNECT-ERROR: GlobalProjectManager::resetSelectedAnno()");
            }
        }
        _curSelAnno = -1;
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

    bool GlobalProjectManager::containsInSearchPathAdv(const QFileInfo &dir) const {
        if (_project->searchPath()->isEmpty()) {
            return false;
        }

        QFileInfo cdir = dir;
        if (dir.isRelative()) {
            cdir = relToAbs(dir);
        }

        QListIterator<QFileInfo> i(*_project->searchPath());
        while (i.hasNext()) {
            QFileInfo cur = i.next();
            if (cur.isRelative()) {
                cur = relToAbs(cur);
            }
            if (cdir == cur) {
                return true;
            }
        }
        return false;
    }

    QString GlobalProjectManager::defAnnoFileName(const QUuid &uuid) {
        QString name("annotations_%1.%2");
        return name.arg(uuid.toString(), GlobalConfig::fileExt.annotations);
    }

    void GlobalProjectManager::loadClassDefs() throw(IOException *, XmlException *) {
        if (_project != NULL && _project->classPath() != NULL && !_project->classPath()->isEmpty()) {
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
        if (_project != NULL && _project->searchPath() != NULL && !_project->searchPath()->isEmpty()) {
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

    void GlobalProjectManager::sortAnnoFiles() {
        GlobalLogger::instance()->logDebug("Sorting annotation files.");
        anno::dt::AnnoFileListSorter sorter(_fileList);
        sorter.sort();
        GlobalLogger::instance()->logDebug("Finished sorting annotation files.");
    }

    void GlobalProjectManager::loadAnnoFilesFromDir(QDir dir) throw(IOException *,
            XmlException *) {
        Q_ASSERT(dir.exists() && dir.isAbsolute());

        QStringList filters;
        filters << FileExtensions::asFilter(GlobalConfig::fileExt.annotations);
        QFileInfoList lst = dir.entryInfoList(filters, QDir::Files);

        if (!lst.isEmpty()) {
            GlobalLogger::instance()->logDebug(QString("Found %1 annotation files.").arg(lst.size()));
            QListIterator<QFileInfo> i(lst);
            while (i.hasNext()) {
                QFileInfo fi = i.next();
                GlobalLogger::instance()->logDebug(QString("Trying load annotation data from [%1].").arg(fi.filePath()));
                if (fi.exists() && fi.isFile()) {
                    //TODO think of handling symlinks here. further research must be done on this!

                    if (dt::AnnoFileData::probeFile(fi.absoluteFilePath(), _project->uuid())) {
                        addAnnoFile(dt::AnnoFileData::fromFile(fi.absoluteFilePath()));
                    } else {
                        GlobalLogger::instance()->logDebug(QString("Won't load annotation data from [%1]. Wrong Annotation Complex ID").arg(fi.filePath()));
                    }
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

        /* MA: either use native format, or use importer to load idl/al */

        if (path.endsWith(".idl", Qt::CaseInsensitive) ||
                path.endsWith(".al", Qt::CaseInsensitive)) {

            /* MA: how do we choose the right plugin? */
            GlobalImExportManager *im = GlobalImExportManager::instance();

            assert(im->importerCount() == 1 &&
                   "how to choose the right plugin?");

            ImporterPlugin *importer = im->getImporter(0);
            assert(importer != 0);

            bool silent = true;

            /* MA: init project */

            QFileInfo fi_path(path);
            _project = new AnnoProject(path);
            _project->setUuid(QUuid::createUuid());
            _project->setProjectName(fi_path.fileName());
            _project->setFilePath(path);
            _project->addToSearchPath(fi_path.absolutePath());

            /* MA: init others */

            _filterMan = new filter::AnnoFilterManager(_project);
            _classList = new dt::AnnoAvClassList();
            _fileList = new QList<dt::AnnoFileData *>();
            _fileListMod = new QList<dt::AnnoFileData *>();

            std::cout << "GlobalProjectManager::loadFromFile, importer start" << std::endl;
            importer->exec(QFileInfo(path), silent);
            std::cout << "GlobalProjectManager::loadFromFile, importer end" << std::endl;

            //sortAnnoFiles();
            setupAllSignals();
        } else {

            _project = dt::AnnoProject::fromFile(path);
            _filterMan = new filter::AnnoFilterManager(_project);
            _classList = new dt::AnnoAvClassList();
            _fileList = new QList<dt::AnnoFileData *>();
            _fileListMod = new QList<dt::AnnoFileData *>();

            if (loadSub) {
                loadClassDefs();
                loadAnnoFiles();
                //sortAnnoFiles();
                setupAllSignals();
            }
        }

        emit projectOpened(_project);
    }

    void GlobalProjectManager::saveToFile(bool saveSub) const throw(IOException *,
            IllegalStateException *, XmlException *) {
        if (!isValid()) {
            throw new IllegalStateException(__FILE__, __LINE__, "Cannot save current data. Invalid data.");
        }


        /* MA: need way to decide which plugin supports which format */
        if (_project->filePath().endsWith(".al", Qt::CaseInsensitive) ||
                _project->filePath().endsWith(".idl", Qt::CaseInsensitive)) {

            GlobalImExportManager *im = GlobalImExportManager::instance();

            assert(im->exporterCount() == 1 &&
                   "how to choose the right plugin?");

            ExporterPlugin *exporter = im->getExporter(0);
            assert(exporter != 0);

            bool silent = true;

            std::cout << "GlobalProjectManager::saveToFile, exporter start" << std::endl;
            exporter->exec(QFileInfo(_project->filePath()), silent);
            std::cout << "GlobalProjectManager::saveToFile, exporter end" << std::endl;
        } else {
            /* MA: use native format */

            _project->writeToFile();
            if (saveSub) {
                if(_fileListMod->isEmpty()) {
                    GlobalLogger::instance()->logWarning("Request to save data files, but list of modified data files is empty!");
                }

                QListIterator<dt::AnnoFileData *> i(*_fileListMod);
                while (i.hasNext()) {
                    dt::AnnoFileData *cur = i.next();
                    cur->writeToFile();
                    cur->resetModifiedState(false);
                }
                if(_fileListMod->size() != 0) {
                    GlobalLogger::instance()->logWarning("Modified File List not empty despite of reset!");
                }
                _fileListMod->clear();
            }
        }
    }

}

void GlobalProjectManager::setPoseMode(bool poseMode) {
    _poseMode = poseMode;
}

bool GlobalProjectManager::isPoseMode() {
    return _poseMode;
}

int GlobalProjectManager::removeNonEmptyDir(QDir &dir) {
    int res = 0;
    QStringList lstDirs  = dir.entryList(QDir::Dirs | QDir::AllDirs | QDir::NoDotAndDotDot);
    QStringList lstFiles = dir.entryList(QDir::Files);
    foreach (QString entry, lstFiles) {
        QString entryAbsPath = dir.absolutePath() + "/" + entry;
        QFile::remove(entryAbsPath);
    }
    foreach (QString entry, lstDirs) {
        QString entryAbsPath = dir.absolutePath() + "/" + entry;
        QDir childDir(entryAbsPath);
        removeNonEmptyDir(childDir);
    }
    if (!QDir().rmdir(dir.absolutePath())) {
        res = 1;
    }
    return res;
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
