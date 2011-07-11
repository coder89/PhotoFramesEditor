#ifndef BORDEREDITTOOL_H
#define BORDEREDITTOOL_H

#include <QDockWidget>
#include <QDoubleSpinBox>

#include <kcombobox.h>
#include <kcolorbutton.h>
#include <kpushbutton.h>

namespace KIPIPhotoFramesEditor
{
    class BorderEditTool : public QDockWidget
    {
            Q_OBJECT

            QDoubleSpinBox * m_width_widget;
            KComboBox * m_corners_style_widget;
            KColorButton * m_color_widget;
            KPushButton * m_apply_button;
            KPushButton * m_cancel_button;

            qreal m_width;
            Qt::PenJoinStyle m_corner_style;
            QColor m_color;

        public:

            explicit BorderEditTool(QWidget * parent = 0);

        signals:

            void borderStyleChanged(qreal width, Qt::PenJoinStyle cornersStyle, const QColor & color);

        public slots:

            void setInitialValues(qreal width, Qt::PenJoinStyle cornersStyle, const QColor & color);

        protected Q_SLOTS:

            void widthChanged(double width)
            {
                if (m_width != width)
                {
                    m_apply_button->setEnabled(true);
                    m_cancel_button->setEnabled(true);
                }
            }

            void cornerStyleChanged(int cornerStyleIndex)
            {
                if (m_corner_style != (Qt::PenJoinStyle)m_corners_style_widget->itemData(cornerStyleIndex).toInt())
                {
                    m_apply_button->setEnabled(true);
                    m_cancel_button->setEnabled(true);
                }
            }

            void colorChanged(QColor color)
            {
                if (m_color != color)
                {
                    m_apply_button->setEnabled(true);
                    m_cancel_button->setEnabled(true);
                }
            }

            void emitBorderStyleChanged()
            {
                m_apply_button->setEnabled(false);
                m_cancel_button->setEnabled(false);
                emit borderStyleChanged(m_width_widget->value(), (Qt::PenJoinStyle)m_corners_style_widget->itemData(m_corners_style_widget->currentIndex()).toInt(), m_color_widget->color());
            }

            void resetForm()
            {
                setInitialValues(m_width, m_corner_style, m_color);
            }
    };
}

#endif // BORDEREDITTOOL_H
