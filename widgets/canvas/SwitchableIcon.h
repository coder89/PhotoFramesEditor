#ifndef SWITCHABLEICON_H
#define SWITCHABLEICON_H

// Qt
#include <QWidget>
#include <QPixmap>
#include <QIcon>
#include <QList>

namespace KIPIPhotoFramesEditor
{
    class SwitchableIcon : public QWidget
    {
            Q_OBJECT;

        public:
            SwitchableIcon(const QIcon & icon, QWidget * parent = 0);
            virtual void paintEvent(QPaintEvent *);
            virtual void mousePressEvent(QMouseEvent * event);

        public Q_SLOTS:

            void toggle()
            {
                toggled = !toggled;
                this->update();
                emit clicked(this);
            }

        Q_SIGNALS:

            void clicked(SwitchableIcon*);

        private:
            QPixmap m_pixmap;
            bool toggled;
    };
}

#endif // SWITCHABLEICON_H
