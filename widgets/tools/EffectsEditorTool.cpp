#include "EffectsEditorTool.h"

#include <QGridLayout>
#include <QLabel>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

EffectsEditorTool::EffectsEditorTool(QWidget * parent) :
    QDockWidget(parent)
{
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);

    QWidget * widget = new QWidget(this);
    QGridLayout * layout = new QGridLayout(widget);

    // Title
    QLabel * title = new QLabel(i18n("Effects editor"), widget);
    QFont titleFont = title->font();
    titleFont.setBold(true);
    title->setFont(titleFont);
    this->setTitleBarWidget(title);

    widget->setLayout(layout);
    this->setWidget(widget);
    widget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
}
