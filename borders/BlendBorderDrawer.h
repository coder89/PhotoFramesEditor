#ifndef BLENDBORDERDRAWER_H
#define BLENDBORDERDRAWER_H

#include "BlendBorderDrawer_global.h"
#include "BorderDrawerFactoryInterface.h"

using namespace KIPIPhotoFramesEditor;

class BLENDBORDERDRAWERSHARED_EXPORT BlendBorderDrawerFactory : public BorderDrawerFactoryInterface
{
        Q_OBJECT
        Q_INTERFACES(KIPIPhotoFramesEditor::BorderDrawerFactoryInterface)

    public:

        BlendBorderDrawerFactory(QObject * parent = 0);

        virtual QString drawerName() const;

        virtual BorderDrawerInterface * getDrawerInstance(QObject * parent = 0);
};

#endif // BLENDBORDERDRAWER_H
