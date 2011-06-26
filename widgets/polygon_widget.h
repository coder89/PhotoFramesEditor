#ifndef PATH_WIDGET_H
#define PATH_WIDGET_H

// Qt
#include <QPainterPath>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

// Local
#include "abstract_photo.h"

namespace KIPIPhotoFramesEditor
{
    class PolygonWidget : public AbstractPhoto
    {
        public:

            explicit PolygonWidget(const QPainterPath & path, QGraphicsScene * scene);
            explicit PolygonWidget(const QPainterPath & path, QGraphicsItem * parent);
            explicit PolygonWidget(const QPainterPath & path, QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);

        protected:

            static AbstractPhoto * getInstance();
            virtual void mouseDoubleClicked(const QPointF & point);
            virtual void mouseMoved(const QPointF & point);
            virtual void mousePressed(const QPointF & point);
    };
}

#endif // PATH_WIDGET_H
