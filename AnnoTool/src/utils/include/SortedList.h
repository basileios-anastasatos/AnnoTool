#ifndef SORTEDLIST_H_
#define SORTEDLIST_H_

#include <QList>
#include <algorithm>

#include "importGlobals.h"


namespace util {
    template<typename T>
    class SortedList {
        public:
            enum SortDirection {
                SortAsc, SortDesc
            };

        private:
            SortDirection _sortDir;
            QList<T> _internalList;

        private:
            int insertIndex(const T &value);
            int insertIndexAsc(const T &value);
            int insertIndexDesc(const T &value);

        public:
            SortedList(SortDirection dir = SortAsc);
            SortedList(const SortedList<T> &other);
            SortedList(const QList<T> &other, SortDirection dir = SortAsc);

            SortDirection sortDir() const {
                return _sortDir;
            }

            const T &at(int i) const;
            const T &back() const;
            void clear();
            bool contains(const T &value) const;
            int count(const T &value) const;
            int count() const;
            const T &first() const;
            int indexOf(const T &value, int from = 0) const;
            void insert(const T &value);
            void insert(const QList<T> &list);
            void insert(const SortedList<T> &list);
            bool isEmpty() const;
            const T &last() const;
            int lastIndexOf(const T &value, int from = -1) const;
            SortedList<T> mid(int pos, int length = -1) const;
            int removeAll(const T &value);
            void removeAt(int i);
            void removeFirst();
            void removeLast();
            bool removeOne(const T &value);
            int size() const;
            T takeAt(int i);
            T takeFirst();
            T takeLast();
            QSet<T> toSet() const;
            std::list<T> toStdList() const;
            QVector<T> toVector() const;
            T value(int i) const;
            T value(int i, const T &defaultValue) const;
            bool operator!=(const SortedList<T> &other) const;
            bool operator!=(const QList<T> &other) const;
            SortedList<T> operator+(const SortedList<T> &other) const;
            SortedList<T> operator+(const QList<T> &other) const;
            SortedList<T> &operator+=(const SortedList<T> &other);
            SortedList<T> &operator+=(const QList<T> &other);
            SortedList<T> &operator+=(const T &value);
            SortedList<T> &operator<<(const SortedList<T> &other);
            SortedList<T> &operator<<(const QList<T> &other);
            SortedList<T> &operator<<(const T &value);
            SortedList<T> &operator=(const SortedList<T> &other);
            SortedList<T> &operator=(const QList<T> &other);
            bool operator==(const SortedList<T> &other) const;
            bool operator==(const QList<T> &other) const;
            const T &operator[](int i) const;
    };


    template<typename T>
    inline SortedList<T>::SortedList(SortDirection dir) :
        _sortDir(dir) {
        // nothing to do here.
    }

    template<typename T>
    inline SortedList<T>::SortedList(const SortedList<T> &other) :
        _sortDir(other._sortDir), _internalList(other._internalList) {
        // nothing to do here.
    }

    template<typename T>
    inline SortedList<T>::SortedList(const QList<T> &other, SortDirection dir) :
        _sortDir(dir) {
        insert(other);
    }

    template<typename T>
    int SortedList<T>::insertIndex(const T &value) {
        //TODO enhance search speed by using binary search!

        if(_sortDir == SortAsc) {
            return insertIndexAsc(value);
        } else if(_sortDir == SortDesc) {
            return insertIndexDesc(value);
        }

        return size();
    }

    template<typename T>
    int SortedList<T>::insertIndexAsc(const T &value) {
        //TODO enhance search speed by using binary search!

        int size = _internalList.size();
        for(int i = 0; i < size; ++i) {
            if(_internalList[i] > value) {
                return i;
            }
        }
        return size;
    }

    template<typename T>
    int SortedList<T>::insertIndexDesc(const T &value) {
        //TODO enhance search speed by using binary search!

        int size = _internalList.size();
        for (int i = 0; i < size; ++i) {
            if (_internalList[i] < value) {
                return i;
            }
        }
        return size;
    }

//	template<typename T>
//	inline SortedList<T>::SortDirection SortedList<T>::sortDir() const
//	{
//		return _sortDir;
//	}

    template<typename T>
    inline const T &SortedList<T>::at(int i) const {
        return _internalList.at(i);
    }

    template<typename T>
    inline const T &SortedList<T>::back() const {
        return _internalList.back();
    }

    template<typename T>
    inline void SortedList<T>::clear() {
        _internalList.clear();
    }

    template<typename T>
    inline bool SortedList<T>::contains(const T &value) const {
        return _internalList.contains(value);
    }

    template<typename T>
    inline int SortedList<T>::count(const T &value) const {
        return _internalList.count(value);
    }

    template<typename T>
    inline int SortedList<T>::count() const {
        return _internalList.count();
    }

    template<typename T>
    inline const T &SortedList<T>::first() const {
        return _internalList.first();
    }

    template<typename T>
    inline int SortedList<T>::indexOf(const T &value, int from) const {
        return _internalList.indexOf(value, from);
    }

    template<typename T>
    inline void SortedList<T>::insert(const T &value) {
        int idx = insertIndex(value);
        GlobalLogger::instance()->logInfo(QString("insertIndex: %1").arg(idx));
        _internalList.insert(idx, value);
    }

    template<typename T>
    void SortedList<T>::insert(const QList<T> &list) {
        foreach(T val, list) {
            insert(val);
        }
    }

    template<typename T>
    inline void SortedList<T>::insert(const SortedList<T> &list) {
        insert(list._internalList);
    }

    template<typename T>
    inline bool SortedList<T>::isEmpty() const {
        return _internalList.isEmpty();
    }

    template<typename T>
    inline const T &SortedList<T>::last() const {
        return _internalList.last();
    }

    template<typename T>
    inline int SortedList<T>::lastIndexOf(const T &value, int from) const {
        return _internalList.lastIndexOf(value, from);
    }

    template<typename T>
    inline SortedList<T> SortedList<T>::mid(int pos, int length) const {
        return _internalList.mid(pos, length);
    }

    template<typename T>
    inline int SortedList<T>::removeAll(const T &value) {
        return _internalList.removeAll(value);
    }

    template<typename T>
    inline void SortedList<T>::removeAt(int i) {
        _internalList.removeAt(i);
    }

    template<typename T>
    inline void SortedList<T>::removeFirst() {
        _internalList.removeFirst();
    }

    template<typename T>
    inline void SortedList<T>::removeLast() {
        _internalList.removeLast();
    }

    template<typename T>
    inline bool SortedList<T>::removeOne(const T &value) {
        return _internalList.removeOne(value);
    }

    template<typename T>
    inline int SortedList<T>::size() const {
        return _internalList.size();
    }

    template<typename T>
    inline T SortedList<T>::takeAt(int i) {
        return _internalList.takeAt(i);
    }

    template<typename T>
    inline T SortedList<T>::takeFirst() {
        return _internalList.takeFirst();
    }

    template<typename T>
    inline T SortedList<T>::takeLast() {
        return _internalList.takeLast();
    }

    template<typename T>
    inline QSet<T> SortedList<T>::toSet() const {
        return _internalList.toSet();
    }

    template<typename T>
    inline std::list<T> SortedList<T>::toStdList() const {
        return _internalList.toStdList();
    }

    template<typename T>
    inline QVector<T> SortedList<T>::toVector() const {
        return _internalList.toVector();
    }

    template<typename T>
    inline T SortedList<T>::value(int i) const {
        return _internalList.value(i);
    }

    template<typename T>
    inline T SortedList<T>::value(int i, const T &defaultValue) const {
        return _internalList.value(i, defaultValue);
    }

    template<typename T>
    inline bool SortedList<T>::operator!=(const SortedList<T> &other) const {
        return (_internalList != other._internalList);
    }

    template<typename T>
    inline bool SortedList<T>::operator!=(const QList<T> &other) const {
        return (_internalList != other);
    }

    template<typename T>
    inline SortedList<T> SortedList<T>::operator+(const SortedList<T> &other) const {
        return operator+(other._internalList);
    }

    template<typename T>
    SortedList<T> SortedList<T>::operator+(const QList<T> &other) const {
        SortedList<T> newList(*this);
        newList.insert(other);
        return newList;
    }

    template<typename T>
    inline SortedList<T> &SortedList<T>::operator+=(const SortedList<T> &other) {
        return operator+=(other._internalList);
    }

    template<typename T>
    inline SortedList<T> &SortedList<T>::operator+=(const QList<T> &other) {
        insert(other);
        return *this;
    }

    template<typename T>
    inline SortedList<T> &SortedList<T>::operator+=(const T &value) {
        insert(value);
        return *this;
    }

    template<typename T>
    inline SortedList<T> &SortedList<T>::operator<<(const SortedList<T> &other) {
        return operator+=(other);
    }

    template<typename T>
    inline SortedList<T> &SortedList<T>::operator<<(const QList<T> &other) {
        return operator+=(other);
    }

    template<typename T>
    inline SortedList<T> &SortedList<T>::operator<<(const T &value) {
        return operator+=(value);
    }

    template<typename T>
    SortedList<T> &SortedList<T>::operator=(const SortedList<T> &other) {
        _sortDir = other._sortDir;
        _internalList = other._internalList;
        return *this;
    }

    template<typename T>
    SortedList<T> &SortedList<T>::operator=(const QList<T> &other) {
        insert(other);
        return *this;
    }

    template<typename T>
    inline bool SortedList<T>::operator==(const SortedList<T> &other) const {
        return (_internalList == other._internalList);
    }

    template<typename T>
    inline bool SortedList<T>::operator==(const QList<T> &other) const {
        return (_internalList == other);
    }

    template<typename T>
    inline const T &SortedList<T>::operator[](int i) const {
        return _internalList.operator[](i);
    }

}

#endif /* SORTEDLIST_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
