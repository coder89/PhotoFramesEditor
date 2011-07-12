#ifndef PHOTO_CONTEXT_MENU_H
#define PHOTO_CONTEXT_MENU_H

#include <QMenu>
#include <QAction>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhoto;

    class QContextMenu : public QMenu
    {
        public:

            explicit QContextMenu(AbstractPhoto * parent = 0);

            void exec(AbstractPhoto * parent, const QPoint & pos);

            AbstractPhoto * parent() const
            {
                return m_parent;
            }

            void setParent(AbstractPhoto * parent, Qt::WindowFlags /*f*/ = Qt::Widget)
            {
                this->m_parent = parent;
            }

            void enableResize();

        private:

            AbstractPhoto * m_parent;
            QAction * resize;
            QMenu * stackMenu;
            QAction * stack_up;
            QAction * stack_down;
            QAction * stack_top;
            QAction * stack_bottom;
    };
}

#endif // PHOTO_CONTEXT_MENU_H
