#ifndef COLORIZETOOL_H
#define COLORIZETOOL_H

#include "AbstractTool.h"

#include <QDockWidget>
#include <QSlider>
#include <QSpinBox>
#include <QFormLayout>

#include <kcombobox.h>
#include <kcolorbutton.h>

namespace KIPIPhotoFramesEditor
{
    class ColorizeTool : public AbstractTool
    {
            enum ColorizeType
            {
                Solid,
                LinearGradient,
                RadialGradient,
            };

            Q_OBJECT

            KComboBox * m_colorize_type_widget;
            KColorButton * m_color_button;
            QSlider * m_strength_slider;
            QSpinBox * m_strength_spinbox;

            KColorButton * m_color_button1;
            KColorButton * m_color_button2;

            QFormLayout * m_form_layout;

        public:

            explicit ColorizeTool(QWidget * parent = 0);

        signals:

        public slots:

        protected slots:

            void modeChanged(int index);

    };
}

#endif // COLORIZETOOL_H
