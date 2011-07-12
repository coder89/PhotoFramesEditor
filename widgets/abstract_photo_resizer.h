#ifndef ABSTRACT_PHOTO_RESIZER_H
#define ABSTRACT_PHOTO_RESIZER_H

#include <QWidget>

// Local
#include "AbstractPhoto.h"

namespace KIPIPhotoFramesEditor
{

    class AbstractPhoto::AbstractPhotoResizer : public QWidget
    {
            Q_OBJECT

        public:

            explicit AbstractPhotoResizer(QWidget *parent = 0);

        signals:

        public slots:

    };

}

#endif // ABSTRACT_PHOTO_RESIZER_H
