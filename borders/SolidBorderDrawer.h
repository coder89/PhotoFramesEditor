#ifndef SOLIDBORDERDRAWER_H
#define SOLIDBORDERDRAWER_H

#include "SolidBorderDrawer_global.h"
#include "BorderDrawerFactoryInterface.h"

using namespace KIPIPhotoFramesEditor;

class SOLIDBORDERDRAWERSHARED_EXPORT SolidBorderDrawerFactory : public BorderDrawerFactoryInterface
{
        Q_OBJECT
        Q_INTERFACES(KIPIPhotoFramesEditor::BorderDrawerFactoryInterface)

    public:

        SolidBorderDrawerFactory(QObject * parent = 0);

        virtual QString drawerName() const;

        virtual BorderDrawerInterface * getDrawerInstance(QObject * parent = 0);
};

#endif // SOLIDBORDERDRAWER_H
