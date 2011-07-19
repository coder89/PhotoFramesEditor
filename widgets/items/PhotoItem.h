#ifndef PHOTOITEM_H
#define PHOTOITEM_H

#include "AbstractPhoto.h"

namespace KIPIPhotoFramesEditor
{
    class PhotoItem : public AbstractPhoto
    {
        public:

            PhotoItem(const QImage & photo = QImage(), QGraphicsScene * scene = 0);
    };
}

#endif // PHOTOITEM_H
