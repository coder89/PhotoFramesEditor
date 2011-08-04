#ifndef BORDERDRAWERSLOADER_H
#define BORDERDRAWERSLOADER_H

#include <QObject>
#include <QtProperty>

#include "BorderDrawerFactoryInterface.h"

namespace KIPIPhotoFramesEditor
{
    class BorderDrawerFactoryInterface;

    class BorderDrawersLoaderPrivate;
    class BorderDrawersLoader : public QObject
    {
            Q_OBJECT

            BorderDrawersLoaderPrivate * d;
            static BorderDrawersLoader * m_instance;

        public:

            ~BorderDrawersLoader();
            static BorderDrawersLoader * instance(QObject * parent = 0);
            static void registerDrawer(BorderDrawerFactoryInterface * factory);
            static QStringList registeredDrawers();
            static BorderDrawerFactoryInterface * getFactoryByName(const QString & name);
            static BorderDrawerInterface * getDrawerByName(const QString & name);
            static QWidget * createEditor(BorderDrawerInterface * drawer, bool createCommands);

        private:

            explicit BorderDrawersLoader(QObject * parent = 0);
    };
}

#endif // BORDERDRAWERSLOADER_H
