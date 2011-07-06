#include "GraphicsEditingWidgetMenu.h"
#include "QGraphicsEditionWidget.h"

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

GraphicsEditingWidgetMenu::GraphicsEditingWidgetMenu(QGraphicsEditionWidget * widget, QWidget * parent) :
    KMenu(parent)
{
    // Title
    QAction * title = this->addAction(i18n("Selected item(s)"));
    title->setEnabled(false);
    QFont titleFont = title->font();    titleFont.setBold(true);
    title->setFont(titleFont);

    this->addSeparator();

    // Removing items
    QAction * deleteItems = this->addAction(i18n("Delete"));
    connect(deleteItems, SIGNAL(triggered()), widget, SLOT(deleteSelected()));
}
