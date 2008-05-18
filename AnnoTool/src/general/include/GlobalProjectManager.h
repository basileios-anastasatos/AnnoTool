#ifndef GLOBALPROJECTMANAGER_H_
#define GLOBALPROJECTMANAGER_H_

#include "AllAnnoExceptions.h"
#include "AnnoProject.h"
#include "AnnoAvClassList.h"
#include "AnnoFileData.h"
#include <QList>

class QDir;

namespace anno {
    using namespace exc;

    class GlobalProjectManager {
        private:
            static GlobalProjectManager *_me;

        private:
            dt::AnnoProject *_project;
            dt::AnnoAvClassList *_classList;
            QList<dt::AnnoFileData *> *_fileList;
            int _curSelFile;
            int _curSelAnno;

        private:
            GlobalProjectManager();
            static void setupManager();
            void loadClassDefs() throw(IOException *, XmlException *);
            void loadAnnoFiles() throw(IOException *, XmlException *);
            void loadAnnoFilesFromDir(QDir dir) throw(IOException *, XmlException *);

        public:
            virtual ~GlobalProjectManager();
            static GlobalProjectManager *instance();

        public:
            dt::AnnoProject *project();
            dt::AnnoAvClassList *classes();
            QList<dt::AnnoFileData *> *files();
            bool isValid() const;
            void clear();
            void newEmpty(const QString &projectPath, const QUuid &projectUuid) throw(IllegalStateException *);

        public:
            void resetSelectedFile();
            void resetSelectedAnno();
            void setSelectedFileRow(int index);
            void setSelectedAnnoRow(int index);
            int selectedFileRow() const;
            int selectedAnnoRow() const;
            dt::AnnoFileData *selectedFile();
            dt::Annotation *selectedAnno();
            QUuid selectedAnnoUuid();

        public:
            QFileInfo relToAbs(const QFileInfo &file) const
            throw(IllegalStateException *);
            QFileInfo relToAbs(const QDir &dir) const throw(IllegalStateException *);
            QFileInfo absToRel(const QFileInfo &file) const
            throw(IllegalStateException *);
            QFileInfo absToRel(const QDir &dir) const throw(IllegalStateException *);
            QDir projectDir() const throw(IllegalStateException *);
            bool containsInSearchPathAdv(const QFileInfo &dir) const;
            static QString defAnnoFileName(const QUuid &uuid);

        public:
            void loadFromFile(const QString &path, bool loadSub = true)
            throw(IOException *, XmlException *, IllegalStateException *);
            void saveToFile(bool saveSub = false) const throw(IOException *,
                    IllegalStateException *, XmlException *);

    };

}

#endif /*GLOBALPROJECTMANAGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
