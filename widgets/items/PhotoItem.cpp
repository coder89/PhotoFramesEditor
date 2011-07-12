#include "PhotoItem.h"
#include "AbstractPhotoEffectsGroup.h"
#include "AbstractPhotoEffect.h"

using namespace KIPIPhotoFramesEditor;

PhotoItem::PhotoItem(const QImage & photo, QGraphicsScene * scene) :
    AbstractPhoto(scene)
{
    this->setupWidget(QPixmap::fromImage(photo));
}
