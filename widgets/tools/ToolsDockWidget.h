#ifndef TOOLSDOCKWIDGET_H
#define TOOLSDOCKWIDGET_H

#include <QDockWidget>
#include <QPushButton>

namespace KIPIPhotoFramesEditor
{
    class ToolsDockWidget : public QDockWidget
    {
            Q_OBJECT

            QPushButton * m_tool_pointer;
            QPushButton * m_tool_hand;

        public:

            explicit ToolsDockWidget(QWidget * parent = 0);

        signals:

            void pointerToolSelected();
            void handToolSelected();

        protected slots:

            void emitPointerToolSelected(bool isSelected)
            {
                if (isSelected)
                    emit pointerToolSelected();
            }

            void emitHandToolSelected(bool isSelected)
            {
                if (isSelected)
                    emit handToolSelected();
            }

    };
}

#endif // TOOLSDOCKWIDGET_H
