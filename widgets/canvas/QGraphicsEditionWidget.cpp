#include "QGraphicsEditionWidget.h"
#include "QGraphicsRotationItem.h"
#include "QGraphicsSelectionItem.h"

using namespace KIPIPhotoFramesEditor;

QGraphicsEditionWidget::QGraphicsEditionWidget(QGraphicsItem * parent) :
    QGraphicsWidget(parent),
    m_rot(new QGraphicsRotationItem(this)),
    m_sel(new QGraphicsSelectionItem(this))
{
}
