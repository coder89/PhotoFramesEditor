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

    // Moving items on stack
    QAction * upItems = this->addAction(i18n("Move &up"));
    //connect(deleteItems,SIGNAL(triggered()),widget,SLOT(deleteSelected()));
    QAction * downItems = this->addAction(i18n("Mode &down"));
    //connect(deleteItems,SIGNAL(triggered()),widget,SLOT(deleteSelected()));
    QAction * topItems = this->addAction(i18n("Mode &top"));
    //connect(deleteItems,SIGNAL(triggered()),widget,SLOT(deleteSelected()));
    QAction * bottomItems = this->addAction(i18n("Mode &bottom"));
    //connect(deleteItems,SIGNAL(triggered()),widget,SLOT(deleteSelected()));

    this->addSeparator();

    // Removing items
    QAction * deleteItems = this->addAction(i18n("Delete"));
    connect(deleteItems, SIGNAL(triggered()), widget, SLOT(deleteSelected()));
}
