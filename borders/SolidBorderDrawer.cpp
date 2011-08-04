#include "SolidBorderDrawer.h"
#include "SolidBorderDrawer_p.h"

#include <klocalizedstring.h>
#include <QtCore>

SolidBorderDrawerFactory::SolidBorderDrawerFactory(QObject * parent) :
    BorderDrawerFactoryInterface(parent)
{
}

QString SolidBorderDrawerFactory::drawerName() const
{
    return i18n("Solid");
}

BorderDrawerInterface * SolidBorderDrawerFactory::getDrawerInstance(QObject * parent)
{
    return new SolidBorderDrawer(this, parent);
}

Q_EXPORT_PLUGIN2(SolidBorderDrawerFactory, SolidBorderDrawerFactory)
