#ifndef SCALINGWIDGETITEM_H
#define SCALINGWIDGETITEM_H

#include "AbstractItemInterface.h"

namespace KIPIPhotoFramesEditor
{
    class ScalingWidgetItem : public AbstractItemInterface
    {
        public:

            ScalingWidgetItem(QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);

            virtual QRectF boundingRect() const;
            virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    };
}

#endif // SCALINGWIDGETITEM_H
