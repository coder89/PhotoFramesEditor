#ifndef TOOLSDOCKWIDGET_H
#define TOOLSDOCKWIDGET_H

#include <QDockWidget>
#include <QLayout>
#include <QUndoCommand>
#include <QStackedLayout>

#include <kpushbutton.h>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhoto;
    class AbstractTool;

    class ToolsDockWidget : public QDockWidget
    {
            Q_OBJECT

            KPushButton * m_tool_pointer;
            KPushButton * m_tool_hand;
            KPushButton * m_effects_button;
            KPushButton * m_tool_border;
            KPushButton * m_tool_colorize_button;

            bool m_has_selection;

            QStackedLayout * m_tool_widget_layout;
            AbstractTool * m_effects_widget;
            AbstractTool * m_colorize_widget;

            AbstractPhoto * m_currentPhoto;

        public:

            explicit ToolsDockWidget(QWidget * parent = 0);
            void setDefaultTool();

        signals:

            void undoCommandCreated(QUndoCommand * command);
            void requireSingleSelection();
            void requireMultiSelection();
            void pointerToolSelected();
            void handToolSelected();
            // Effects tool selection signals
            void effectsToolSelectionChanged(bool);
            void effectsToolSelected();
            // Border tool selection signals
            void borderToolSelectionChanged(bool);
            void borderToolSelected();
            // Colorize tool selection signals
            void colorizeToolSelectionChanged(bool);
            void colorizeToolSelected();

        public slots:

            void itemSelected(AbstractPhoto * photo);

        protected slots:

            void selectionChanged(bool hasSelection)
            {
                m_tool_widget_layout->setEnabled(hasSelection);
                m_has_selection = hasSelection;
            }

            void setEffectsWidgetVisible(bool isVisible);

            void emitPointerToolSelected(bool isSelected)
            {
                if (isSelected)
                {
                    m_tool_widget_layout->setCurrentIndex(0);
                    emit requireMultiSelection();
                    emit pointerToolSelected();
                }
            }

            void emitHandToolSelected(bool isSelected)
            {
                if (isSelected)
                {
                    m_tool_widget_layout->setCurrentIndex(0);
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

            void setColorizeWidgetVisible(bool isSelected);
    };
}

#endif // TOOLSDOCKWIDGET_H
