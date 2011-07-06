#ifndef GRAPHICSEDITINGWIDGETMENU_H
#define GRAPHICSEDITINGWIDGETMENU_H

#include <kmenu.h>

namespace KIPIPhotoFramesEditor
{
    class QGraphicsEditionWidget;

    class GraphicsEditingWidgetMenu : public KMenu
    {
        public:

            GraphicsEditingWidgetMenu(QGraphicsEditionWidget * widget, QWidget * parent = 0);
    };
}
#endif // GRAPHICSEDITINGWIDGETMENU_H
