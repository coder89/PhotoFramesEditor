#include "CanvasCreationDialog.h"
#include "global.h"

// KDE
#include <kcombobox.h>
#include <klocalizedstring.h>
#include <kpushbutton.h>

// Qt
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QButtonGroup>

using namespace KIPIPhotoFramesEditor;

class CanvasCreationDialog::CanvasCreationDialogPriv
{
    public:
        CanvasCreationDialogPriv() :
            xSize(0),
            ySize(0)
        {
            paperSizes.insert("A0 (841 x 1189 mm)",QPrinter::A0);
            paperSizes.insert("A1 (594 x 841 mm)",QPrinter::A1);
            paperSizes.insert("A2 (420 x 594 mm)",QPrinter::A2);
            paperSizes.insert("A3 (297 x 420 mm)",QPrinter::A3);
            paperSizes.insert("A4 (210 x 297 mm, 8.26 x 11.69 inches)",QPrinter::A4);
            paperSizes.insert("A5 (148 x 210 mm)",QPrinter::A5);
            paperSizes.insert("A6 (105 x 148 mm)",QPrinter::A6);
            paperSizes.insert("A7 (74 x 105 mm)",QPrinter::A7);
            paperSizes.insert("A8 (52 x 74 mm)",QPrinter::A8);
            paperSizes.insert("A9 (37 x 52 mm)",QPrinter::A9);
            paperSizes.insert("B0 (1000 x 1414 mm)",QPrinter::B0);
            paperSizes.insert("B1 (707 x 1000 mm)",QPrinter::B1);
            paperSizes.insert("B2 (500 x 707 mm)",QPrinter::B2);
            paperSizes.insert("B3 (353 x 500 mm)",QPrinter::B3);
            paperSizes.insert("B4 (250 x 353 mm)",QPrinter::B4);
            paperSizes.insert("B5 (176 x 250 mm, 6.93 x 9.84 inches)",QPrinter::B5);
            paperSizes.insert("B6 (125 x 176 mm)",QPrinter::B6);
            paperSizes.insert("B7 (88 x 125 mm)",QPrinter::B7);
            paperSizes.insert("B8 (62 x 88 mm)",QPrinter::B8);
            paperSizes.insert("B9 (33 x 62 mm)",QPrinter::B9);
            paperSizes.insert("B10 (31 x 44 mm)",QPrinter::B10);
            paperSizes.insert("C5E (163 x 229 mm)",QPrinter::C5E);
            paperSizes.insert("U.S. Common 10 Envelope (105 x 241 mm)",QPrinter::Comm10E);
            paperSizes.insert("DLE (110 x 220 mm)",QPrinter::DLE);
            paperSizes.insert("Executive (7.5 x 10 inches, 190.5 x 254 mm)",QPrinter::Executive);
            paperSizes.insert("Folio (210 x 330 mm)",QPrinter::Folio);
            paperSizes.insert("Ledger (431.8 x 279.4 mm)",QPrinter::Ledger);
            paperSizes.insert("Legal (8.5 x 14 inches, 215.9 x 355.6 mm)",QPrinter::Legal);
            paperSizes.insert("Letter (8.5 x 11 inches, 215.9 x 279.4 mm)",QPrinter::Letter);
            paperSizes.insert("Tabloid (279.4 x 431.8 mm)",QPrinter::Tabloid);
            paperSizes.insert("Custom",QPrinter::Custom);
        }

        ~CanvasCreationDialogPriv()
        {
        }

        void swapSizes();
        QSize calculateSize();
        void updateSizeLabel();

        QWidget *   sizeWidget;
        QWidget *   advancedWidget;
        KComboBox * paperSize;
        QDoubleSpinBox *  xSize;
        QDoubleSpinBox *  ySize;
        QButtonGroup *  orientationGroup;
        KPushButton *   verticalButton;
        QLabel *    sizeLabel;
        KPushButton *   horizontalButton;
        KComboBox * sizeUnitsWidget;
        QDoubleSpinBox *  xResolution;
        QDoubleSpinBox *  yResolution;
        KComboBox * resolutionUnitsWidget;

        qreal xSize_val;
        qreal ySize_val;

        QSize currentSize;

        QMap<QString,QPrinter::PaperSize> paperSizes;
};

void CanvasCreationDialog::CanvasCreationDialogPriv::swapSizes()
{
    qreal width = xSize->value();
    xSize->setValue(ySize->value());
    ySize->setValue(width);
    qreal xRes = xResolution->value();
    xResolution->setValue(yResolution->value());
    yResolution->setValue(xRes);
}

QSize CanvasCreationDialog::CanvasCreationDialogPriv::calculateSize()
{
    qreal factor = 1;
    qreal xFactor = 1;
    qreal yFactor = 1;
    SizeUnits sizeUnit = KIPIPhotoFramesEditor::sizeUnitsNames().key(sizeUnitsWidget->currentText());
    if (sizeUnit != KIPIPhotoFramesEditor::Pixels)
    {
        factor /= KIPIPhotoFramesEditor::sizeUnitsFactors().value(sizeUnit);
        ResolutionUnits resolutionUnit = KIPIPhotoFramesEditor::resolutionUnitsNames().key(resolutionUnitsWidget->currentText());
        factor *= KIPIPhotoFramesEditor::resolutionUnitsFactors().value(resolutionUnit);
        xFactor = xResolution->value()*factor;
        yFactor = yResolution->value()*factor;
    }
    return (currentSize = QSize(qRound(xSize->value()*xFactor), qRound(ySize->value()*yFactor)));
}

void CanvasCreationDialog::CanvasCreationDialogPriv::updateSizeLabel()
{
    QSize size = calculateSize();
    sizeLabel->setText(QString::number(size.width()).append(" x ").append(QString::number(size.height()).append(" px")));
}

CanvasCreationDialog::CanvasCreationDialog(QWidget *parent) :
    KDialog(parent),
    d(new CanvasCreationDialogPriv)
{
    setupDialog();
}

CanvasCreationDialog::~CanvasCreationDialog()
{
    delete d;
}

void CanvasCreationDialog::setupDialog()
{
    QWidget * main = new QWidget(this);
    setMainWidget(main);
    QVBoxLayout * vLayout = new  QVBoxLayout(main);
    main->setLayout(vLayout);


    /// ----------------------- TEMPLATES PART -----------------------
    QGridLayout * gridLayout = new QGridLayout();
    d->paperSize = new KComboBox(main);
    connect(d->paperSize, SIGNAL(activated(QString)), this, SLOT(recalculatePaperSize(const QString&)));
    d->paperSize->addItems(d->paperSizes.keys());
    gridLayout->addWidget(new QLabel("Template:", main),0,0);
    gridLayout->addWidget(d->paperSize,0,1);
    vLayout->addLayout(gridLayout);

    /// ----------------------- CANVAS SIZE PART -----------------------
    d->sizeWidget = new QGroupBox(i18n("Canvas size"), main);
    vLayout->addWidget(d->sizeWidget);

    gridLayout = new QGridLayout(d->sizeWidget);
    d->sizeWidget->setLayout(gridLayout);

    // Width widget
    d->xSize = new QDoubleSpinBox(d->sizeWidget);
    d->xSize->setMinimum(0.00001);
    d->xSize->setMaximum(999999);
    d->xSize->setValue(800);
    d->xSize_val = 800;
    d->xSize->setDecimals(3);
    connect(d->xSize,SIGNAL(valueChanged(double)),this,SLOT(widthChanged(double)));
    gridLayout->addWidget(new QLabel("Width:", d->sizeWidget),0,0);
    gridLayout->addWidget(d->xSize,0,1);

    // Height widget
    d->ySize = new QDoubleSpinBox(d->sizeWidget);
    d->ySize->setMinimum(0.00001);
    d->ySize->setMaximum(999999);
    d->ySize->setValue(800);
    d->ySize_val = 800;
    d->ySize->setDecimals(3);
    connect(d->ySize,SIGNAL(valueChanged(double)),this,SLOT(heightChanged(double)));
    gridLayout->addWidget(new QLabel("Height:", d->sizeWidget),1,0);
    gridLayout->addWidget(d->ySize,1,1);

    // Unit widget
    d->sizeUnitsWidget = new KComboBox(d->sizeWidget);
    connect(d->sizeUnitsWidget, SIGNAL(activated(QString)), this, SLOT(sizeUnitsChanged(QString)));
    d->sizeUnitsWidget->addItems(KIPIPhotoFramesEditor::sizeUnitsNames().values());
    gridLayout->addWidget(d->sizeUnitsWidget,1,2);

    // Orientation buttons
    d->orientationGroup = new QButtonGroup(d->sizeWidget);
    d->horizontalButton = new KPushButton(KIcon(":horizontal_orientation.png"),"",d->sizeWidget);
    d->horizontalButton->setCheckable(true);
    d->horizontalButton->setChecked(true);
    d->horizontalButton->setFlat(true);
    d->horizontalButton->setIconSize(QSize(24,24));
    connect(d->horizontalButton, SIGNAL(toggled(bool)), this, SLOT(setHorizontal(bool)));
    d->verticalButton = new KPushButton(KIcon(":vertical_orientation.png"),"",d->sizeWidget);
    d->verticalButton->setCheckable(true);
    d->verticalButton->setFlat(true);
    d->verticalButton->setIconSize(QSize(24,24));
    connect(d->verticalButton, SIGNAL(toggled(bool)), this, SLOT(setVertical(bool)));
    d->orientationGroup->addButton(d->horizontalButton);
    d->orientationGroup->addButton(d->verticalButton);
    QHBoxLayout * hLayout = new QHBoxLayout();
    hLayout->addWidget(d->horizontalButton);
    hLayout->addWidget(d->verticalButton);
    gridLayout->addWidget(new QLabel("Orientation:", d->sizeWidget),2,0);
    gridLayout->addLayout(hLayout,2,1);
    d->sizeLabel = new QLabel(d->sizeWidget);
    gridLayout->addWidget(d->sizeLabel,2, 2);

    /// ----------------------- ADVANCED PART -----------------------
    d->advancedWidget = new QGroupBox(i18n("Advanced"), main);
    vLayout->addWidget(d->advancedWidget);
    gridLayout = new QGridLayout(d->advancedWidget);
    d->advancedWidget->setLayout(gridLayout);

    // x resolution widget
    d->xResolution = new QDoubleSpinBox(d->advancedWidget);
    d->xResolution->setMinimum(0);
    d->xResolution->setMaximum(999999);
    d->xResolution->setValue(800);
    d->xResolution->setDecimals(3);
    gridLayout->addWidget(new QLabel("Resolution X:", d->advancedWidget),0,0);
    gridLayout->addWidget(d->xResolution,0,1);

    // y resolution widget
    d->yResolution = new QDoubleSpinBox(d->advancedWidget);
    d->yResolution->setMinimum(0);
    d->yResolution->setMaximum(999999);
    d->yResolution->setValue(800);
    d->yResolution->setDecimals(3);
    gridLayout->addWidget(new QLabel("Resolution Y:", d->advancedWidget),1,0);
    gridLayout->addWidget(d->yResolution,1,1);

    // Unit widget
    d->resolutionUnitsWidget = new KComboBox(d->sizeWidget);
    connect(d->resolutionUnitsWidget, SIGNAL(currentIndexChanged(QString)), this, SLOT(resolutionUnitsChanged(QString)));
    d->resolutionUnitsWidget->addItems(KIPIPhotoFramesEditor::resolutionUnitsNames().values());
    gridLayout->addWidget(d->resolutionUnitsWidget,1,2);

    d->paperSize->setCurrentItem(d->paperSizes.key(QPrinter::Letter));
    recalculatePaperSize(d->paperSize->currentText());
}

QSize CanvasCreationDialog::canvasSize()
{
    return d->calculateSize();
}

void CanvasCreationDialog::recalculatePaperSize(const QString & paperSize)
{
    QPrinter::PaperSize printerPaperSize = d->paperSizes.value(paperSize);
    if (printerPaperSize != QPrinter::Custom)
    {}
    d->updateSizeLabel();
    sizeUnitsChanged(d->sizeUnitsWidget->currentText());
}
#include <QDebug>
void CanvasCreationDialog::sizeUnitsChanged(const QString & unitName)
{
    QSize size = d->currentSize;
    SizeUnits unit = KIPIPhotoFramesEditor::sizeUnitsNames().key(unitName);
    if (unit == KIPIPhotoFramesEditor::Pixels)
    {
        d->xSize->setValue(size.width());
        d->ySize->setValue(size.height());
        return;
    }
    ResolutionUnits resUnit = KIPIPhotoFramesEditor::resolutionUnitsNames().key(d->resolutionUnitsWidget->currentText());
    qreal factor = KIPIPhotoFramesEditor::sizeUnitsFactors().value(unit) / KIPIPhotoFramesEditor::resolutionUnitsFactors().value(resUnit);
    qDebug() << size.width() / d->xResolution->value() * factor;
    d->xSize_val = size.width() / d->xResolution->value() * factor;
    d->ySize_val = size.height() / d->yResolution->value() * factor;
    d->xSize->setValue(d->xSize_val);
    d->ySize->setValue(d->ySize_val);
}

void CanvasCreationDialog::resolutionUnitsChanged(const QString & unitName)
{
    QSize size = d->currentSize;
    SizeUnits unit = KIPIPhotoFramesEditor::sizeUnitsNames().key(d->resolutionUnitsWidget->currentText());
    if (unit == KIPIPhotoFramesEditor::Pixels)
        return;
}

void CanvasCreationDialog::setHorizontal(bool isHorizontal)
{
    if (isHorizontal && d->xSize_val < d->ySize_val)
        d->swapSizes();
}

void CanvasCreationDialog::setVertical(bool isVertical)
{
    if (isVertical && d->ySize_val < d->xSize_val)
        d->swapSizes();
}

void CanvasCreationDialog::widthChanged(double width)
{
    d->updateSizeLabel();
    if (d->currentSize.height() < d->currentSize.width())
        d->horizontalButton->setChecked(true);
    d->updateSizeLabel();
}

void CanvasCreationDialog::heightChanged(double height)
{
    d->updateSizeLabel();
    if (d->currentSize.width() < d->currentSize.height())
        d->verticalButton->setChecked(true);
    d->updateSizeLabel();
}
