#ifndef SCENEBACKGROUND_H
#define SCENEBACKGROUND_H

#include <QBrush>
#include <QGraphicsItem>
#include <QDomDocument>

namespace KIPIPhotoFramesEditor
{
    class SceneBackground : public QObject, public QGraphicsItem
    {
            Q_OBJECT

            QRectF m_rect;
            QBrush m_first_brush;
            QBrush m_second_brush;

            // Image background specific data
            QImage m_image;
            Qt::Alignment m_image_align;
            Qt::AspectRatioMode m_image_aspect_ratio;
            QSize m_image_size;
            bool m_image_repeat;

            // For painting/rendering purpose
            QPixmap m_pixmap;

            class BackgroundFirstColorChangedCommand;
            class BackgroundSecondColorChangedCommand;
            class BackgroundPatternChangedCommand;
            class BackgroundImageChangedCommand;

        public:

            SceneBackground(QGraphicsScene * scene = 0);
            virtual QRectF boundingRect() const;

            void setFirstColor(const QColor & firstColor);
            void setSecondColor(const QColor & firstColor);
            void setPattern(const QColor & firstColor, const QColor & secondColor, Qt::BrushStyle patternStyle);
            void setImage(const QImage & image, Qt::Alignment align, Qt::AspectRatioMode aspectRatio, bool repeat);
            void setImage(const QImage & image, Qt::Alignment align, const QSize & fixedSize, bool repeat);

            bool isColor() const;
            bool isGradient() const;
            bool isImage() const;
            bool isPattern() const;

            QDomElement toSvg(QDomDocument & document) const;
            bool fromSvg(QDomElement & element);

            QColor firstColor() const;
            QColor secondColor() const;
            Qt::BrushStyle pattern() const;
            QImage image() const;
            Qt::Alignment imageAlignment() const;
            Qt::AspectRatioMode imageAspectRatio() const;
            QSize imageSize() const;
            bool imageRepeated() const;

        signals:

            void firstColorChanged(QColor);
            void secondColorChanged(QColor);
            void patternChanged(Qt::BrushStyle);

        protected:

            QVariant itemChange(GraphicsItemChange change, const QVariant & value);
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
            void render();
            void render(QPainter * painter, const QRect & rect);

            void emitFirstColorChanged()
            {
                emit firstColorChanged(m_first_brush.color());
            }
            void emitSecondColorChanged()
            {
                emit secondColorChanged(m_second_brush.color());
            }
            void emitPatternChanged()
            {
                emit patternChanged(m_first_brush.style());
            }

        private:

            void sceneChanged();

        friend class BackgroundFirstColorChangedCommand;
        friend class BackgroundSecondColorChangedCommand;
        friend class BackgroundPatternChangedCommand;
        friend class BackgroundImageChangedCommand;
    };
}

#endif // SCENEBACKGROUND_H
