#ifndef SWITCHABLEICON_H
#define SWITCHABLEICON_H

// Qt
#include <QWidget>
#include <QPixmap>
#include <QIcon>
#include <QList>

namespace KIPIPhotoFramesEditor
{
    class LayersModelItem;

    class SwitchableIcon : public QWidget
    {
            Q_OBJECT;

        public:
            SwitchableIcon(const QIcon & icon, LayersModelItem * item, QWidget * parent = 0);
            virtual void paintEvent(QPaintEvent *);
            virtual void mousePressEvent(QMouseEvent * event);

        public Q_SLOTS:

            void toggle()
            {
                toggled = !toggled;
                this->update();
                emit clicked(this);
            }

            LayersModelItem * item()
            {
                return m_item;
            }

        Q_SIGNALS:

            void clicked(SwitchableIcon*);

        private:
            QPixmap m_pixmap;
            LayersModelItem * m_item;
            bool toggled;
    };
}

#endif // SWITCHABLEICON_H
