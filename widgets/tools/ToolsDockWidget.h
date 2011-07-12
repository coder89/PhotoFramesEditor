#ifndef TOOLSDOCKWIDGET_H
#define TOOLSDOCKWIDGET_H

#include <QDockWidget>
#include <QLayout>

#include <kpushbutton.h>

namespace KIPIPhotoFramesEditor
{
    class ColorizeTool;

    class ToolsDockWidget : public QDockWidget
    {
            Q_OBJECT

            KPushButton * m_tool_pointer;
            KPushButton * m_tool_hand;
            KPushButton * m_tool_effects;
            KPushButton * m_tool_border;
            KPushButton * m_tool_colorize_button;

            QWidget * m_current_widget;
            ColorizeTool * m_tool_colorize;

            bool m_has_selection;

        public:

            explicit ToolsDockWidget(QWidget * parent = 0);
            void setDefaultTool();

        signals:

            void requireSingleSelection();
            void requireMultiSelection();
            void pointerToolSelected();
            void handToolSelected();
            // Border tool selection signals
            void borderToolSelectionChanged(bool);
            void borderToolSelected();
            // Colorize tool selection signals
            void colorizeToolSelectionChanged(bool);
            void colorizeToolSelected();

        protected slots:

            void selectionChanged(bool hasSelection)
            {
                if (!m_current_widget)
                    return;
                m_current_widget->setEnabled(hasSelection);
                m_has_selection = hasSelection;
            }

            void emitPointerToolSelected(bool isSelected)
            {
                if (isSelected)
                {
                    emit requireMultiSelection();
                    emit pointerToolSelected();
                }
            }

            void emitHandToolSelected(bool isSelected)
            {
                if (isSelected)
                {
                    emit requireMultiSelection();
                    emit handToolSelected();
                }
            }

            void emitBorderToolSelected(bool isSelected)
            {
                emit borderToolSelectionChanged(isSelected);
                if (isSelected)
                {
                    emit requireSingleSelection();
                    emit borderToolSelected();
                }
            }
            void emitColorizeToolSelected(bool isSelected)
            {
                emit colorizeToolSelectionChanged(isSelected);
                if (isSelected)
                {
                    emit requireSingleSelection();
                    emit colorizeToolSelected();
                }
            }
    };
}

#endif // TOOLSDOCKWIDGET_H
