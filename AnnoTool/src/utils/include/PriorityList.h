#ifndef PRIORITYLIST_H_
#define PRIORITYLIST_H_

#include "SortedList.h"

namespace util {

    template<typename T>
    class PriorityList {
        private:
            class Entry {
                public:
                    quint32 priority;
                    T value;

                public:
                    Entry();
                    Entry(T val, quint32 pr);

                    bool equals(const Entry &e) const;

                public:
                    bool operator== (const Entry &e) const;
                    bool operator< (const Entry &e) const;
                    bool operator> (const Entry &e) const;
                    bool operator<= (const Entry &e) const;
                    bool operator>= (const Entry &e) const;
            };

        private:
            SortedList<Entry> _sortedList;

        public:
            PriorityList();
            PriorityList(const PriorityList<T> &lst);

        public:
            const T &at(int i) const;
            quint32 priorityAt(int i) const;
            const T &first() const;
            quint32 priorityFirst() const;
            const T &last() const;
            quint32 priorityLast() const;

            void removeAt(int i);
            void removeFirst();
            void removeLast();
            T takeAt(int i);
            T takeFirst();
            T takeLast();

            void insert(const T &value, quint32 priority = 0);
            void updatePriorityAt(int i, quint32 priority);
            void incPriorityAt(int i);
            void decPriorityAt(int i);

            bool isEmpty() const;
            void clear();
            int count() const;
            int size() const;


            // TODO think about these methods, are they needed?
            // int indexOf(const T& value, int from = 0) const;
            // int lastIndexOf(const T& value, int from = -1) const;

//
//
//			void insert(const QList<T>& list);
//			void insert(const SortedList<T>& list);
//			SortedList<T> mid(int pos, int length = -1) const;
//			int removeAll(const T& value);
//			bool removeOne(const T& value);
//			QSet<T> toSet() const;
//			std::list<T> toStdList() const;
//			QVector<T> toVector() const;
//			T value(int i) const;
//			T value(int i, const T& defaultValue) const;
//			bool operator!=(const SortedList<T>& other) const;
//			bool operator!=(const QList<T>& other) const;
//			SortedList<T> operator+(const SortedList<T>& other) const;
//			SortedList<T> operator+(const QList<T>& other) const;
//			SortedList<T>& operator+=(const SortedList<T>& other);
//			SortedList<T>& operator+=(const QList<T>& other);
//			SortedList<T>& operator+=(const T& value);
//			SortedList<T>& operator<<(const SortedList<T>& other);
//			SortedList<T>& operator<<(const QList<T>& other);
//			SortedList<T>& operator<<(const T& value);
//			SortedList<T>& operator=(const SortedList<T>& other);
//			SortedList<T>& operator=(const QList<T>& other);
//			bool operator==(const SortedList<T>& other) const;
//			bool operator==(const QList<T>& other) const;
//			const T& operator[](int i) const;
    };


    // Entry helper class methods
    //---------------------------------------------------------------------------------
    template<typename T>
    inline PriorityList<T>::Entry::Entry() : priority(0) {
    }

    template<typename T>
    inline PriorityList<T>::Entry::Entry(T val, quint32 pr = 0) : priority(pr) {
        value = val;
    }

    template<typename T>
    bool PriorityList<T>::Entry::equals(const Entry &e) const {
        return (priority == e.priority && value == e.value);
    }

    template<typename T>
    bool PriorityList<T>::Entry::operator==(const Entry &e) const {
        return (priority == e.priority);
    }

    template<typename T>
    bool PriorityList<T>::Entry::operator<(const Entry &e) const {
        return (priority < e.priority);
    }

    template<typename T>
    bool PriorityList<T>::Entry::operator>(const Entry &e) const {
        return (priority > e.priority);
    }

    template<typename T>
    bool PriorityList<T>::Entry::operator<=(const Entry &e) const {
        return (priority <= e.priority);
    }

    template<typename T>
    bool PriorityList<T>::Entry::operator>=(const Entry &e) const {
        return (priority >= e.priority);
    }
    //---------------------------------------------------------------------------------


    // PriorityList class methods
    //---------------------------------------------------------------------------------
    template<typename T>
    inline PriorityList<T>::PriorityList() : _sortedList(SortedList<Entry>::SortDesc) {
        // nothing to do here.
    }

    template<typename T>
    PriorityList<T>::PriorityList(const PriorityList<T> &lst) : _sortedList(lst._sortedList, SortedList<Entry>::SortDesc) {
        // nothing to do here.
    }

    template<typename T>
    inline const T &PriorityList<T>::at(int i) const {
        return _sortedList.at(i).value;
    }

    template<typename T>
    inline quint32 PriorityList<T>::priorityAt(int i) const {
        return _sortedList.at(i).priority;
    }

    template<typename T>
    inline const T &PriorityList<T>::first() const {
        return at(0);
    }

    template<typename T>
    inline quint32 PriorityList<T>::priorityFirst() const {
        return priorityAt(0);
    }

    template<typename T>
    inline const T &PriorityList<T>::last() const {
        return at(size() - 1);
    }

    template<typename T>
    inline quint32 PriorityList<T>::priorityLast() const {
        return priorityAt(size() - 1);
    }

    template<typename T>
    inline void PriorityList<T>::removeAt(int i) {
        _sortedList.removeAt(i);
    }

    template<typename T>
    inline void PriorityList<T>::removeFirst() {
        removeAt(0);
    }

    template<typename T>
    inline void PriorityList<T>::removeLast() {
        removeAt(size() - 1);
    }

    template<typename T>
    inline T PriorityList<T>::takeAt(int i) {
        return _sortedList.takeAt(i);
    }

    template<typename T>
    inline T PriorityList<T>::takeFirst() {
        return takeAt(0);
    }

    template<typename T>
    inline T PriorityList<T>::takeLast() {
        return takeAt(size() - 1);
    }

    template<typename T>
    inline void PriorityList<T>::insert(const T &value, quint32 priority) {
        _sortedList.insert(Entry(value, priority));
    }

    template<typename T>
    void PriorityList<T>::updatePriorityAt(int i, quint32 priority) {
        Entry e = _sortedList.takeAt(i);
        e.priority = priority;
        _sortedList.insert(e);
    }

    template<typename T>
    inline void PriorityList<T>::incPriorityAt(int i) {
        updatePriorityAt(i, priorityAt(i) + 1);
    }

    template<typename T>
    void PriorityList<T>::decPriorityAt(int i) {
        quint32 p = priorityAt(i);
        if(p > 0) {
            updatePriorityAt(i, (p - 1));
        }
    }

    template<typename T>
    inline bool PriorityList<T>::isEmpty() const {
        return _sortedList.isEmpty();
    }

    template<typename T>
    inline void PriorityList<T>::clear() {
        _sortedList.clear();
    }

    template<typename T>
    inline int PriorityList<T>::count() const {
        return _sortedList.size();
    }

    template<typename T>
    inline int PriorityList<T>::size() const {
        return _sortedList.size();
    }


    //---------------------------------------------------------------------------------


}

#endif /* PRIORITYLIST_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
