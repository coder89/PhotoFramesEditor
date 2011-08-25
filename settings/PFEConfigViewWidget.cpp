#include "PFEConfigViewWidget.h"
#include "PFEConfigSkeleton.h"

#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>

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
    QVBoxLayout * layout = new QVBoxLayout();
    this->setLayout(layout);

    PFEConfigSkeleton * skeleton = PFEConfigSkeleton::self();
    QGroupBox * gridBox = new QGroupBox(i18n("Grid"), this);
    layout->addWidget(gridBox);
    QFormLayout * gridLayout = new QFormLayout();
    gridBox->setLayout(gridLayout);
    d->xGrid = new QDoubleSpinBox(gridBox);
    d->xGrid->setMinimum(1);
    d->xGrid->setMaximum(9999);
    d->xGrid->setSingleStep(1.0);
    d->xGrid->setValue(skeleton->horizontalGrid());
    gridLayout->addRow(i18n("Horizontal distance"), d->xGrid);
    d->yGrid = new QDoubleSpinBox(gridBox);
    d->yGrid->setMinimum(1);
    d->yGrid->setMaximum(9999);
    d->yGrid->setSingleStep(1.0);
    d->yGrid->setValue(skeleton->verticalGrid());
    gridLayout->addRow(i18n("Vertical distance"), d->yGrid);
}
