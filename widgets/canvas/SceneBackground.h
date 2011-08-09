#ifndef SCENEBACKGROUND_H
#define SCENEBACKGROUND_H

#include <QGraphicsItem>
#include <QGradient>
namespace KIPIPhotoFramesEditor
{
    class SceneBackground : public QGraphicsItem
    {
            QRectF m_rect;
            QBrush m_brush;

            class BackgroundChangedCommand;

        public:

            SceneBackground(QGraphicsScene * scene = 0);
            virtual QRectF boundingRect() const;
            void setBrush(const QBrush & backgroundBrush);

            bool isColor() const;
            bool isGradient() const;
            bool isImage() const;
            bool isPattern() const;

            QColor color() const;

            QBrush fd;

        protected:

            QVariant itemChange(GraphicsItemChange change, const QVariant & value);
            void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

        private:

            void sceneChanged();

        friend class BackgroundChangedCommand;
    };
}

#endif // SCENEBACKGROUND_H
