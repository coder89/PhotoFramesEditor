#include "photo_context_menu.h"

#include <QDebug>
namespace KIPIPhotoFramesEditor
{
    QContextMenu::QContextMenu(AbstractPhoto * parent) :
        m_parent(0)
    {
        resize = addAction("Resize");
        stackMenu = addMenu("Stack");
        stack_up = stackMenu->addAction("Up");
        stack_down = stackMenu->addAction("Down");
        stackMenu->addSeparator();
        stack_top = stackMenu->addAction("Top");
        stack_bottom = stackMenu->addAction("Bottom");
    }

    void QContextMenu::exec(AbstractPhoto * parent, const QPoint & pos)
    {
        this->m_parent = parent;
        QAction * result = QMenu::exec(pos);
        if (result == resize)
            enableResize();
    }

    void QContextMenu::enableResize()
    {
        //m_parent->map.fill(Qt::blue);
        //m_parent->setPixmap(m_parent->map);
    }
}
