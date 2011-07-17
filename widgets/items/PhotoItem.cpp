#include "PhotoItem.h"
#include "PhotoEffectsGroup.h"
#include "PhotoEffectsLoader.h"

using namespace KIPIPhotoFramesEditor;

PhotoItem::PhotoItem(const QImage & photo, QGraphicsScene * scene) :
    AbstractPhoto(scene)
{
    this->setupWidget(QPixmap::fromImage(photo));
}
