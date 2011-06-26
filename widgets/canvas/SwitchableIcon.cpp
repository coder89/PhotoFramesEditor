#include "SwitchableIcon.h"

// Qt
#include <QMouseEvent>
#include <QPainter>

using namespace KIPIPhotoFramesEditor;

SwitchableIcon::SwitchableIcon(const QIcon & icon, QWidget * parent) :
    QWidget(parent),
    toggled(false)
{
    m_pixmap = icon.pixmap(icon.actualSize(QSize(16,16)));
}

void SwitchableIcon::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (toggled)
        painter.setOpacity(0.2);
    painter.drawPixmap(rect().center()-(m_pixmap).rect().center(), (m_pixmap));
}

void SwitchableIcon::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        toggled = !toggled;
        this->update();
        emit clicked(this);
    }
    QWidget::mousePressEvent(event);
}
