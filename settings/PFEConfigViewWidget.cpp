#include "PFEConfigViewWidget.h"
#include "PFEConfigSkeleton.h"

#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::PFEConfigViewWidgetPrivate
{
    QDoubleSpinBox * xGrid;
    QDoubleSpinBox * yGrid;
    QCheckBox      * showGrid;

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

    d->showGrid = new QCheckBox(gridBox);
    d->showGrid->setChecked(PFEConfigSkeleton::showGrid());
    gridLayout->addRow(i18n("Show grid lines"), d->showGrid);

    d->xGrid = new QDoubleSpinBox(gridBox);
    KConfigSkeletonItem * hgi = skeleton->findItem("HorizontalGrid");
    if (hgi)
    {
        d->xGrid->setMinimum(hgi->minValue().toDouble());
        d->xGrid->setMaximum(hgi->maxValue().toDouble());
    }
    d->xGrid->setSingleStep(1.0);
    d->xGrid->setValue(PFEConfigSkeleton::horizontalGrid());
    gridLayout->addRow(i18n("Horizontal distance"), d->xGrid);

    d->yGrid = new QDoubleSpinBox(gridBox);
    KConfigSkeletonItem * vgi = skeleton->findItem("VerticalGrid");
    if (hgi)
    {
        d->yGrid->setMinimum(vgi->minValue().toDouble());
        d->yGrid->setMaximum(vgi->maxValue().toDouble());
    }
    d->yGrid->setSingleStep(1.0);
    d->yGrid->setValue(PFEConfigSkeleton::verticalGrid());
    gridLayout->addRow(i18n("Vertical distance"), d->yGrid);
}
