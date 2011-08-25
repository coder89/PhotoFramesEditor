#include "PFEConfigViewWidget.h"
#include "PFEConfigSkeleton.h"

#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QLabel>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::PFEConfigViewWidgetPrivate
{
    QDoubleSpinBox * xGrid;
    QDoubleSpinBox * yGrid;

    friend class PFEConfigViewWidget;
};

PFEConfigViewWidget::PFEConfigViewWidget(QWidget * parent, const QString & caption) :
    QWidget(parent),
    d(new PFEConfigViewWidgetPrivate)
{
    this->setupGUI();
    this->setWindowTitle(caption);
}

PFEConfigViewWidget::~PFEConfigViewWidget()
{
    delete d;
}

void PFEConfigViewWidget::setupGUI()
{
    QFormLayout * layout = new QFormLayout();
    this->setLayout(layout);

    PFEConfigSkeleton * skeleton = PFEConfigSkeleton::self();
    d->xGrid = new QDoubleSpinBox(this);
    d->xGrid->setMinimum(1);
    d->xGrid->setMaximum(9999);
    d->xGrid->setSingleStep(1.0);
    d->xGrid->setValue(skeleton->horizontalGrid());
    layout->addRow(i18n("Horizontal grid distance"), d->xGrid);
}
