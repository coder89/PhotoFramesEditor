#include "GridSetupDialog.h"

#include <QFormLayout>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

GridSetupDialog::GridSetupDialog(QWidget * parent) :
    KDialog(parent)
{
    this->setCaption(i18n("Setup grid lines"));
    this->setModal(true);

    centralWidget = new QWidget(this);
    setMainWidget(centralWidget);

    QFormLayout * layout = new QFormLayout();
    layout->setSizeConstraint( QLayout::SetFixedSize );

    x = new QDoubleSpinBox(centralWidget);
    x->setMinimum(1.0);
    x->setMaximum(100.0);
    x->setSingleStep(1.0);
    x->setValue(25);
    layout->addRow(i18n("Horizontal distance"), x);

    y = new QDoubleSpinBox(centralWidget);
    y->setMinimum(1.0);
    y->setMaximum(100.0);
    y->setSingleStep(1.0);
    y->setValue(25);
    layout->addRow(i18n("Vertical distance"), y);

    centralWidget->setLayout(layout);

    this->setFixedSize( this->sizeHint() );
}

void GridSetupDialog::setHorizontalDistance(qreal value)
{
    x->setValue(value);
}

void GridSetupDialog::setVerticalDistance(qreal value)
{
    y->setValue(value);
}

qreal GridSetupDialog::horizontalDistance() const
{
    return x->value();
}

qreal GridSetupDialog::verticalDistance() const
{
    return y->value();
}
