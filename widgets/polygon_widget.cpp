#include "polygon_widget.h"
#include <QDebug>
#include <QPainter>
namespace KIPIPhotoFramesEditor
{

    PolygonWidget::PolygonWidget(const QPainterPath & path, QGraphicsScene * scene) :
        AbstractPhoto(scene)
    {
        this->setupWidget(path);
    }

    PolygonWidget::PolygonWidget(const QPainterPath & path, QGraphicsItem * parent) :
        AbstractPhoto(0)
    {
        this->setupWidget(path);
    }

    PolygonWidget::PolygonWidget(const QPainterPath & path, QGraphicsItem * parent, QGraphicsScene * scene) :
        AbstractPhoto(scene)
    {
        this->setupWidget(path);
    }

    AbstractPhoto * PolygonWidget::getInstance()
    {
        return new PolygonWidget(QPainterPath());
    }


    void PolygonWidget::mouseDoubleClicked(const QPointF & point)
    {
        m_path.lineTo(point);
        m_path.closeSubpath();
        this->setupWidget(m_path);
    }

    void PolygonWidget::mouseMoved(const QPointF & point)
    {
    }

    void PolygonWidget::mousePressed(const QPointF & point)
    {
        if (m_path.elementCount())
            m_path.lineTo(point);
        else
            m_path.moveTo(point);
    }
}