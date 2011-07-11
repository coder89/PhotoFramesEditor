#ifndef TOOLSDOCKWIDGET_H
#define TOOLSDOCKWIDGET_H

#include <QDockWidget>

#include <kpushbutton.h>

namespace KIPIPhotoFramesEditor
{
    class ToolsDockWidget : public QDockWidget
    {
            Q_OBJECT

            KPushButton * m_tool_pointer;
            KPushButton * m_tool_hand;
            KPushButton * m_tool_border;

        public:

            explicit ToolsDockWidget(QWidget * parent = 0);
            void setDefaultTool();

        signals:

            void pointerToolSelected();
            void handToolSelected();
            void borderToolSelectionChanged(bool);
            void borderToolSelected();

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

            void emitBorderToolSelected(bool isSelected)
            {
                emit borderToolSelectionChanged(isSelected);
                if (isSelected)
                    emit borderToolSelected();
            }
    };
}

#endif // TOOLSDOCKWIDGET_H
