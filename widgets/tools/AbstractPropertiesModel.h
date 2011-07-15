#ifndef ABSTRACTPROPERTIESMODEL_H
#define ABSTRACTPROPERTIESMODEL_H

#include <QAbstractItemModel>

namespace KIPIPhotoFramesEditor
{
    class AbstractPropertiesModel : QAbstractItemModel
    {
        Q_OBJECT

        public:

            AbstractPropertiesModel(QObject * parent = 0);
    };
}

#endif // ABSTRACTPROPERTIESMODEL_H
