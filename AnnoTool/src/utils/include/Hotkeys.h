#ifndef HOTKEYS_H_
#define HOTKEYS_H_ HOTKEYS_H_

#include <iostream>
#include <QMap>
#include <Qt>
#include <QMutex>
#include <QString>

#include "AllAnnoExceptions.h"

namespace util {
    class String2QtKeyEnum {
        private:
            /**
             * The singleton pointer to the only class instance.
             */
            static String2QtKeyEnum *_singletonInstance;

            // other private members
        private:
            /**
             * Mutex for thread safety.
             */
            mutable QMutex _mutex;

            // private singleton stuff
        private:
            /**
             * The private singleton constructor.
             */
            String2QtKeyEnum();

            /**
             * Internal helper for initializing the singleton.
             */
            static void setup();
            void setupMap();

            // destructor
        public:
            /**
             * Virtual destructor.
             */
            virtual ~String2QtKeyEnum();

            // public singleton stuff
        public:
            /**
             * Singleton access method.
             * This method enables the global access to the only class instance.
             *
             * @return The only class instance.
             */
            static String2QtKeyEnum *instance();

            /**
             * Singleton reset method.
             * This method provides the capability to reset the only existing
             * class instance.
             */
            static void reset();

        private:
            QMap<QString, Qt::Key>              keymap;
            QMap<QString, Qt::KeyboardModifier> modmap;

        public:
            Qt::Key               operator[](const QString & combination) throw (anno::exc::FormatException *);
            Qt::KeyboardModifiers modifiers (const QString & combination) throw (anno::exc::FormatException *);

    };
} // namespace util

#endif

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
