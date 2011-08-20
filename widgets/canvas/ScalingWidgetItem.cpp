#include "ScalingWidgetItem.h"

using namespace KIPIPhotoFramesEditor;

ScalingWidgetItem::ScalingWidgetItem(QGraphicsItem * parent, QGraphicsScene * scene) :
    AbstractItemInterface(parent, scene)
{
}

QRectF ScalingWidgetItem::boundingRect() const
{
    return QRectF();
}

void ScalingWidgetItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{}
