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

AbstractPhoto::QContextMenu * AbstractPhoto::menu = 0;
const QColor AbstractPhoto::SELECTED_ITEM_COLOR(255,0,0,20);

AbstractPhoto::AbstractPhoto(QGraphicsScene * parent) :
    QGraphicsPixmapItem(0,parent),
    LayersModelItem(),
    x_grid(1),
    y_grid(1)
{
    if (menu == 0)
        menu = new QContextMenu();

    QList<QVariant> data;
    data << "sfa" << QIcon(":eye.png") << QIcon(":padlock.png") << QString().append("Name=").append(QString::number(QTime::currentTime().msec()));
    LayersModelItem::setData(data);

    //this->setAcceptDrops(true);
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
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

void AbstractPhoto::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    menu->exec(this, event->screenPos());
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
    if (QGraphicsItem::flags() & ItemIsMovable)
    {
        event->accept();
        QPointF p;
        if (event->modifiers() & Qt::ShiftModifier)
        {
            p = event->scenePos() - event->buttonDownPos(Qt::LeftButton);
            p.setX(x_grid*round(p.rx()/x_grid));
            p.setY(y_grid*round(p.ry()/y_grid));
        }
        else
            p = this->pos() + event->pos() - event->lastPos();
        this->setPos(p.rx(),p.ry());
    }
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
