#include "abstract_photo.h"

#include <QMenu>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainterPath>
#include <QPolygonF>
#include <QPainter>
#include <QVariant>
#include <qmath.h>

// Local
#include "photo_context_menu.h"

#include <QDebug>
#include <QTime>
using namespace KIPIPhotoFramesEditor;

const QColor AbstractPhoto::SELECTED_ITEM_COLOR(255,0,0,20);

AbstractPhoto::AbstractPhoto(QGraphicsScene * parent) :
    QGraphicsPixmapItem(0,parent),
    m_name("New layer")
{
    //this->setAcceptDrops(true);
    this->setAcceptHoverEvents(true);
}

void AbstractPhoto::setupWidget(const QPainterPath & path)
{
    this->m_path = path.translated(-path.boundingRect().topLeft());
    this->m_path = m_path.simplified();
    m_pixmap = QPixmap(m_path.boundingRect().size().toSize());
    m_pixmap.fill(Qt::transparent);

    QPainter p(&m_pixmap);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(QColor(122, 163, 39), 1, Qt::DotLine));
    p.drawPath(m_path);
    p.fillPath(m_path, QColor(200, 200, 255, 255));

    QGraphicsPixmapItem::setPixmap(m_pixmap);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->updateIcon();
}

void AbstractPhoto::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem unselectedOption = (*option);
    unselectedOption.state &= !QStyle::State_Selected;
    QGraphicsPixmapItem::paint(painter,&unselectedOption,widget);

    // Selecting item
    if ((option->state) & QStyle::State_Selected)
    {
        QPainterPath exposedPath = QPainterPath();
        exposedPath.addRect(option->exposedRect);
        painter->fillPath(m_path.intersected(exposedPath), SELECTED_ITEM_COLOR);
    }

}

void AbstractPhoto::dragEnterEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "dragEnterEvent";
    event->accept();
}

void AbstractPhoto::dragLeaveEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "dragLeaveEvent";
    event->accept();
}

void AbstractPhoto::dragMoveEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "dragMoveEvent";
    event->accept();
}

void AbstractPhoto::dropEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "dropEvent";
    event->accept();
}

void AbstractPhoto::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsPixmapItem::mouseMoveEvent(event);
    if (QGraphicsItem::flags() & ItemIsMovable)
    {
        if (event->modifiers() & Qt::ShiftModifier)
        {
            QPointF p;
            event->accept();
            p = event->scenePos() - event->buttonDownPos(Qt::LeftButton);
            p.setX(x_grid*round(p.rx()/x_grid));
            p.setY(y_grid*round(p.ry()/y_grid));
            this->setPos(p.rx(),p.ry());
        }
        else
            QGraphicsPixmapItem::mouseMoveEvent(event);
//            p = this->pos() + event->pos() - event->lastPos();
    }
}

void AbstractPhoto::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsPixmapItem::mousePressEvent(event);
    if (this->isSelected())
        this->setCursor(QCursor(Qt::ClosedHandCursor));
}

void AbstractPhoto::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsPixmapItem::mouseReleaseEvent(event);
    if (this->isSelected())
        this->setCursor(QCursor(Qt::OpenHandCursor));
}

void AbstractPhoto::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    QGraphicsPixmapItem::hoverEnterEvent(event);
    if (this->isSelected())
        this->setCursor(QCursor(Qt::OpenHandCursor));
}

void AbstractPhoto::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    QGraphicsPixmapItem::hoverLeaveEvent(event);
    this->unsetCursor();
}

void AbstractPhoto::updateIcon()
{
    //this->setIcon(QIcon(m_pixmap.scaled(100,100,Qt::KeepAspectRatioByExpanding)));
}

void AbstractPhoto::selectStateChanged(bool state)
{
    this->setSelected(state);
}

void AbstractPhoto::showStateChanged(bool state)
{
    this->setVisible(state);
}

void AbstractPhoto::lockStateChanged(bool state)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, state);
}
