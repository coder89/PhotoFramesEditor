#include "BlendBorderDrawer.h"
#include "BlendBorderDrawer_p.h"

#include <klocalizedstring.h>
#include <QtCore>

BlendBorderDrawerFactory::BlendBorderDrawerFactory(QObject * parent) :
    BorderDrawerFactoryInterface(parent)
{
}

QString BlendBorderDrawerFactory::drawerName() const
{
    return i18n("Blend");
}

BorderDrawerInterface * BlendBorderDrawerFactory::getDrawerInstance(QObject * parent)
{
    return new BlendBorderDrawer(this, parent);
}

Q_EXPORT_PLUGIN2(BlendBorderDrawerFactory, BlendBorderDrawerFactory)
