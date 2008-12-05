#ifndef GLOBALPROJECTMANAGER_H_
#define GLOBALPROJECTMANAGER_H_

#include <QObject>
#include "AllAnnoExceptions.h"
#include "AnnoProject.h"
#include "AnnoFilterManager.h"
#include "AnnoAvClassList.h"
#include "AnnoFileData.h"
#include <QList>

class QDir;

namespace anno {
    using namespace exc;

    using dt::AnnoFileData;
    using dt::Annotation;
    using dt::AnnoProject;

    class GlobalProjectManager : public QObject {
            Q_OBJECT

            // Singleton members
        private:
            /**
             * The singleton pointer to the only class instance.
             */
            static GlobalProjectManager *_me;

            // private vars
        private:
            dt::AnnoProject *_project;
            filter::AnnoFilterManager *_filterMan;
            dt::AnnoAvClassList *_classList;
            QList<dt::AnnoFileData *> *_fileList;
            QList<dt::AnnoFileData *> *_fileListMod;
            int _curSelFile;
            int _curSelAnno;

            // private singleton stuff
        private:
            /**
             * The private singleton constructor.
             */
            GlobalProjectManager();
            /**
             * Internal helper for initializing the singleton.
             */
            static void setupManager();

            // private loading helpers
        private:
            void loadClassDefs() throw(IOException *, XmlException *);
            void loadAnnoFiles() throw(IOException *, XmlException *);
            void loadAnnoFilesFromDir(QDir dir) throw(IOException *, XmlException *);
            void sortAnnoFiles();

        private slots:
            void onAnnoFileModifyChange(::anno::dt::AnnoFileData *annoFile, bool prevState, bool curState);

            // destructor
        public:
            /**
             * Virtual destructor.
             */
            virtual ~GlobalProjectManager();

            // public singleton stuff
        public:
            /**
             * Singleton access method.
             * This method enables the global access to the only class instance.
             *
             * @return The only class instance.
             */
            static GlobalProjectManager *instance();

            // member access interface
        public:
            dt::AnnoProject *project();
            const dt::AnnoProject *project() const;
            filter::AnnoFilterManager *filterMan();
            const filter::AnnoFilterManager *filterMan() const;
            dt::AnnoAvClassList *classes();
            const dt::AnnoAvClassList *classes() const;

            QList<dt::AnnoFileData *> files();
            QList<const dt::AnnoFileData *> files() const;
            QList<dt::AnnoFileData *> modFiles();
            QList<const dt::AnnoFileData *> modFiles() const;
            QListIterator<dt::AnnoFileData *> getFileIterator() const;
            QListIterator<dt::AnnoFileData *> getModFileIterator() const;
            int classCount() const;
            int fileCount() const;
            int modFileCount() const;
            dt::AnnoFileData *getAnnoFile(int index);
            const dt::AnnoFileData *getAnnoFile(int index) const;

            void addAnnoFile(dt::AnnoFileData *annoFile, bool newFile = false);

            //TODO implement remove anno file
            void removeAnnoFile(int index);
            void removeAnnoFile(const QUuid &uuid);

            //TODO get by UUID implementieren falls gebraucht
            // dt::AnnoFileData* getAnnoFile(const QUuid& uuid);
            // const dt::AnnoFileData* getAnnoFile(const QUuid& uuid) const;

            //TODO contains implementieren falls gebraucht!
            // bool containsAnnoFile(const QUuid& uuid) const;
            // bool containsAnnoFile(const dt::AnnoFileData* anno) const;

            bool isValid() const;
            void clear();
            void newEmpty(const QString &projectPath, const QUuid &projectUuid)
            throw(IllegalStateException *);
            void setupAllSignals();
            void setupSignals(int index);
            void setupSignals(dt::AnnoFileData *annoFile);

            //selection access
        public:
            void resetSelectedFile();
            void resetSelectedAnno();
            void resetSelectionState();
            void setSelectedFileRow(int index);
            void setSelectedAnnoRow(int index);
            void setSelectedFileRow(const QUuid &uuid);
            void setSelectedAnnoRow(const QUuid &uuid);
            int selectedFileRow() const;
            int selectedAnnoRow() const;
            dt::AnnoFileData *selectedFile();
            const dt::AnnoFileData *selectedFile() const;
            dt::Annotation *selectedAnno();
            const dt::Annotation *selectedAnno() const;
            QUuid selectedFileUuid() const;
            QUuid selectedAnnoUuid() const;

            bool isAnnoSelected(const QUuid &uuid) const;
            bool isAnnoSelected(const dt::Annotation *anno) const;

        public:
            QFileInfo relToAbs(const QFileInfo &file) const throw(IllegalStateException *);
            QFileInfo relToAbs(const QDir &dir) const throw(IllegalStateException *);
            QFileInfo absToRel(const QFileInfo &file) const throw(IllegalStateException *);
            QFileInfo absToRel(const QDir &dir) const throw(IllegalStateException *);
            QDir projectDir() const throw(IllegalStateException *);
            bool containsInSearchPathAdv(const QFileInfo &dir) const;
            static QString defAnnoFileName(const QUuid &uuid);

        public:
            void loadFromFile(const QString &path, bool loadSub = true) throw(IOException *,
                    XmlException *, IllegalStateException *);
            void saveToFile(bool saveSub = false) const throw(IOException *,
                    IllegalStateException *, XmlException *);

        signals:
            void curAnnoFileSelChanged(int curIndex, QUuid uuid, ::anno::dt::AnnoFileData *curAnnoFile);
            void curAnnoSelChanged(int curIndex, QUuid annoId, ::anno::dt::Annotation *curAnno);

            void projectOpened(::anno::dt::AnnoProject *project);
            void projectClosed();

            void curAnnoFileModified(::anno::dt::AnnoFileData *annoFile);
            void curAnnoFileModifyReset(::anno::dt::AnnoFileData *annoFile);
            void curAnnoFileModifyStateChanged(::anno::dt::AnnoFileData *annoFile, bool prevState,
                                               bool curState);
            void curAnnoFile_annoAdded(::anno::dt::Annotation *anno);
            void curAnnoFile_annoRemoved(QUuid annoUuid);

            void curAnnoModified(::anno::dt::Annotation *anno);
            void curAnnoModifyReset(::anno::dt::Annotation *anno);
            void curAnnoModifyStateChanged(::anno::dt::Annotation *anno, bool prevState, bool curState);

    };

    // inlining
    //-----------------------------------------------------------
    inline GlobalProjectManager::GlobalProjectManager() :
        QObject(NULL), _project(NULL), _filterMan(NULL), _classList(NULL), _fileList(NULL), _fileListMod(NULL),
        _curSelFile(-1), _curSelAnno(-1) {
    }

    inline GlobalProjectManager::~GlobalProjectManager() {
        clear();
    }

    inline dt::AnnoProject *GlobalProjectManager::project() {
        return _project;
    }

    inline const dt::AnnoProject *GlobalProjectManager::project() const {
        return _project;
    }

    inline filter::AnnoFilterManager *GlobalProjectManager::filterMan() {
        return _filterMan;
    }

    inline const filter::AnnoFilterManager *GlobalProjectManager::filterMan() const {
        return _filterMan;
    }

    inline dt::AnnoAvClassList *GlobalProjectManager::classes() {
        return _classList;
    }

    inline const dt::AnnoAvClassList *GlobalProjectManager::classes() const {
        return _classList;
    }

    inline QList<dt::AnnoFileData *> GlobalProjectManager::files() {
        return *_fileList;
    }

    inline QList<dt::AnnoFileData *> GlobalProjectManager::modFiles() {
        return *_fileListMod;
    }

    inline QListIterator<dt::AnnoFileData *> GlobalProjectManager::getFileIterator() const {
        return QListIterator<dt::AnnoFileData *>(*_fileList);
    }

    inline QListIterator<dt::AnnoFileData *> GlobalProjectManager::getModFileIterator() const {
        return QListIterator<dt::AnnoFileData *>(*_fileListMod);
    }

    inline int GlobalProjectManager::classCount() const {
        return (_classList != NULL) ? _classList->classCount() : 0;
    }

    inline int GlobalProjectManager::fileCount() const {
        return (_fileList != NULL) ? _fileList->size() : 0;
    }

    inline int GlobalProjectManager::modFileCount() const {
        return (_fileListMod != NULL) ? _fileListMod->size() : 0;
    }

    inline dt::AnnoFileData *GlobalProjectManager::getAnnoFile(int index) {
        if (_fileList != NULL && index >= 0 && index < _fileList->size()) {
            return _fileList->at(index);
        }
        return NULL;
    }

    inline const dt::AnnoFileData *GlobalProjectManager::getAnnoFile(int index) const {
        if (_fileList != NULL && index >= 0 && index < _fileList->size()) {
            return _fileList->at(index);
        }
        return NULL;
    }

    inline void GlobalProjectManager::resetSelectionState() {
        resetSelectedFile();
    }

    inline int GlobalProjectManager::selectedFileRow() const {
        return _curSelFile;
    }

    inline int GlobalProjectManager::selectedAnnoRow() const {
        return _curSelAnno;
    }

    inline QUuid GlobalProjectManager::selectedFileUuid() const {
        if (selectedFile() != NULL) {
            return selectedFile()->imageUuid();
        }
        return QUuid();
    }

    inline QUuid GlobalProjectManager::selectedAnnoUuid() const {
        if (selectedAnno() != NULL) {
            return selectedAnno()->annoId();
        }
        return QUuid();
    }

    inline bool GlobalProjectManager::isAnnoSelected(const QUuid &uuid) const {
        const dt::Annotation *sel = selectedAnno();
        if(sel != NULL) {
            return (sel->annoId() == uuid);
        }
        return false;
    }

    inline bool GlobalProjectManager::isAnnoSelected(const dt::Annotation *anno) const {
        const dt::Annotation *sel = selectedAnno();
        if (sel != NULL) {
            return (sel->annoId() == anno->annoId());
        }
        return false;
    }

//-----------------------------------------------------------

}

#endif /*GLOBALPROJECTMANAGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
