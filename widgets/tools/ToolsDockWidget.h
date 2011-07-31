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
            KPushButton * m_text_button;
            KPushButton * m_rotate_button;
            KPushButton * m_tool_border;
            KPushButton * m_tool_colorize_button;

            bool m_has_selection;

            QStackedLayout * m_tool_widget_layout;
            AbstractTool * m_effects_widget;
            AbstractTool * m_text_widget;
            AbstractTool * m_colorize_widget;

            AbstractPhoto * m_currentPhoto;

        public:

            explicit ToolsDockWidget(QWidget * parent = 0);
            void setDefaultTool();

        signals:

            void undoCommandCreated(QUndoCommand * command);
            void newItemCreated(AbstractPhoto * item);

            void requireSingleSelection();
            void requireMultiSelection();

            void pointerToolSelected();
            void handToolSelected();
            // Effects tool selection signals
            void effectsToolSelectionChanged(bool);
            void effectsToolSelected();
            // Text tool selection signals
            void textToolSelectionChanged(bool);
            void textToolSelected();
            // Rotate tool selection signals
            void rotateToolSelectionChanged(bool);
            void rotateToolSelected();
            // Border tool selection signals
            void borderToolSelectionChanged(bool);
            void borderToolSelected();

        public slots:

            void itemSelected(AbstractPhoto * photo);
            void mousePositionChoosen(const QPointF & position);
            void emitNewItemCreated(AbstractPhoto * item);

        protected slots:

            void selectionChanged(bool hasSelection)
            {
                m_tool_widget_layout->setEnabled(hasSelection);
                m_has_selection = hasSelection;
            }

            void setEffectsWidgetVisible(bool isVisible);
            void setTextWidgetVisible(bool isVisible);
            void setRotateWidgetVisible(bool isVisible);

            void emitPointerToolSelected(bool isSelected)
            {
                if (isSelected)
                {
                    m_tool_widget_layout->setCurrentIndex(0);
                    this->unsetCursor();
                    emit requireMultiSelection();
                    emit pointerToolSelected();
                }
            }

            void emitHandToolSelected(bool isSelected)
            {
                if (isSelected)
                {
                    m_tool_widget_layout->setCurrentIndex(0);
                    this->unsetCursor();
                    emit requireMultiSelection();
                    emit handToolSelected();
                }
            }

            void emitBorderToolSelected(bool isSelected)
            {
                emit borderToolSelectionChanged(isSelected);
                if (isSelected)
                {
                    this->unsetCursor();
                    emit requireSingleSelection();
                    emit borderToolSelected();
                }
            }
    };
}

#endif // TOOLSDOCKWIDGET_H
