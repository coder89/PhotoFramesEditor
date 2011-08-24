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
#include <QDebug>

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
        }

        ~CanvasCreationDialogPriv()
        {
        }

        void swapSizes();
        void updateSizeLabel();
        int toPixels(qreal value, qreal resolution);
        void setPaper(QPrinter::PageSize pageSize);

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

        static int WIDTH;
        static int HEIGHT;
        static QString currentSizeUnit;

        static qreal WIDTH_RES;
        static qreal HEIGHT_RES;
        static QString currentResolutionUnit;

        QMap<QString,QPrinter::PaperSize> paperSizes;
};

int CanvasCreationDialog::CanvasCreationDialogPriv::WIDTH = 800;
int CanvasCreationDialog::CanvasCreationDialogPriv::HEIGHT = 800;
QString CanvasCreationDialog::CanvasCreationDialogPriv::currentSizeUnit = KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Pixels);
qreal CanvasCreationDialog::CanvasCreationDialogPriv::WIDTH_RES = 72;
qreal CanvasCreationDialog::CanvasCreationDialogPriv::HEIGHT_RES = 72;
QString CanvasCreationDialog::CanvasCreationDialogPriv::currentResolutionUnit = KIPIPhotoFramesEditor::resolutionUnitsNames().value(KIPIPhotoFramesEditor::PixelsPerInch);

void CanvasCreationDialog::CanvasCreationDialogPriv::swapSizes()
{
    // swap dimensions
    qreal temp = WIDTH;
    WIDTH = HEIGHT;
    HEIGHT = temp;
    temp = xSize->value();
    xSize->setValue( ySize->value() );
    ySize->setValue( temp );

    // Swap resolutions
    temp = WIDTH_RES;
    WIDTH_RES = HEIGHT_RES;
    HEIGHT_RES = temp;
    temp = xResolution->value();
    xResolution->setValue(yResolution->value());
    yResolution->setValue(temp);
}

void CanvasCreationDialog::CanvasCreationDialogPriv::updateSizeLabel()
{
    sizeLabel->setText(QString::number(WIDTH).append(" x ").append(QString::number(HEIGHT).append(" px")));
}

int CanvasCreationDialog::CanvasCreationDialogPriv::toPixels(qreal value, qreal resolution)
{
    SizeUnits sizeUnit = KIPIPhotoFramesEditor::sizeUnitsNames().key(sizeUnitsWidget->currentText());
    if (sizeUnit == KIPIPhotoFramesEditor::Pixels)
        return value;
    ResolutionUnits resolutionUnit = KIPIPhotoFramesEditor::resolutionUnitsNames().key(resolutionUnitsWidget->currentText());
    qreal result = (KIPIPhotoFramesEditor::resolutionUnitsFactors().value(resolutionUnit) * resolution * value)
                   / KIPIPhotoFramesEditor::sizeUnitsFactors().value(sizeUnit);
    return qRound(result);
}

void CanvasCreationDialog::CanvasCreationDialogPriv::setPaper(QPrinter::PageSize pageSize)
{
    QSizeF result;
    switch (pageSize)
    {
    case QPrinter::A0:
        result = QSizeF(841,1189);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::A1:
        result = QSizeF(594,841);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::A2:
        result = QSizeF(420,594);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::A3:
        result = QSizeF(297,420);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::A4:
        result = QSizeF(210,297);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::A5:
        result = QSizeF(148,210);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::A6:
        result = QSizeF(105,148);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::A7:
        result = QSizeF(74,105);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::A8:
        result = QSizeF(52,74);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::A9:
        result = QSizeF(37,52);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::B0:
        result = QSizeF(1030,1456);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::B1:
        result = QSizeF(728,1030);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::B10:
        result = QSizeF(32,45);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::B2:
        result = QSizeF(515,728);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::B3:
        result = QSizeF(364,515);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::B4:
        result = QSizeF(257,364);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::B5:
        result = QSizeF(182,257);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::B6:
        result = QSizeF(128,182);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::B7:
        result = QSizeF(91,128);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::B8:
        result = QSizeF(64,91);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::B9:
        result = QSizeF(45,64);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::C5E:
        result = QSizeF(163,229);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::Comm10E:
        result = QSizeF(105,241);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::DLE:
        result = QSizeF(110,220);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::Executive:
        result = QSizeF(7.5,10);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Inches) );
        break;
    case QPrinter::Folio:
        result = QSizeF(210,330);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::Ledger:
        result = QSizeF(432,279);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::Legal:
        result = QSizeF(8.5,14);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Inches) );
        break;
    case QPrinter::Letter:
        result = QSizeF(8.5,11);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Inches) );
        break;
    case QPrinter::Tabloid:
        result = QSizeF(279,432);
        sizeUnitsWidget->setCurrentItem( KIPIPhotoFramesEditor::sizeUnitsNames().value(KIPIPhotoFramesEditor::Milimeters) );
        break;
    case QPrinter::Custom:
        return;
    }

    xSize->setValue(result.width());
    ySize->setValue(result.height());
}

CanvasCreationDialog::CanvasCreationDialog(QWidget * parent) :
    KDialog(parent),
    d(new CanvasCreationDialogPriv)
{
    setupDialog(QSize(d->WIDTH, d->HEIGHT), d->currentSizeUnit, QSize(d->WIDTH_RES, d->HEIGHT_RES), d->currentResolutionUnit);
}

CanvasCreationDialog::CanvasCreationDialog(const QSizeF & size, SizeUnits sizeUnits, const QSizeF & resolution, const QString & resolutionUnits, QWidget * parent) :
    KDialog(parent),
    d(new CanvasCreationDialogPriv)
{
    setupDialog(size,
                KIPIPhotoFramesEditor::sizeUnitsNames().value(sizeUnits),
                resolution,
                resolutionUnits);
}

CanvasCreationDialog::~CanvasCreationDialog()
{
    delete d;
}

void CanvasCreationDialog::setupDialog(const QSizeF & size, const QString & sizeUnits, const QSizeF & resolution, const QString & resolutionUnits)
{
    QWidget * main = new QWidget(this);
    setMainWidget(main);
    QVBoxLayout * vLayout = new  QVBoxLayout(main);
    main->setLayout(vLayout);


    /// ----------------------- TEMPLATES PART -----------------------
    QGridLayout * gridLayout = new QGridLayout();
    d->paperSize = new KComboBox(main);
    d->paperSize->addItem( "Custom" );
    d->paperSize->insertSeparator(1);
    d->paperSize->addItems(d->paperSizes.keys());
    d->paperSize->setCurrentIndex(0);
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
    d->xSize->setValue(size.width());
    gridLayout->addWidget(new QLabel("Width:", d->sizeWidget),0,0);
    gridLayout->addWidget(d->xSize,0,1);

    // Height widget
    d->ySize = new QDoubleSpinBox(d->sizeWidget);
    d->ySize->setMinimum(0.00001);
    d->ySize->setMaximum(999999);
    d->ySize->setValue(size.height());
    gridLayout->addWidget(new QLabel("Height:", d->sizeWidget),1,0);
    gridLayout->addWidget(d->ySize,1,1);

    // Unit widget
    d->sizeUnitsWidget = new KComboBox(d->sizeWidget);
    d->sizeUnitsWidget->addItems(KIPIPhotoFramesEditor::sizeUnitsNames().values());
    d->sizeUnitsWidget->setCurrentItem(sizeUnits);
    gridLayout->addWidget(d->sizeUnitsWidget,1,2);

    // Orientation buttons
    d->horizontalButton = new KPushButton(KIcon(":horizontal_orientation.png"),"",d->sizeWidget);
    d->horizontalButton->setCheckable(true);
    d->horizontalButton->setFlat(true);
    d->horizontalButton->setIconSize(QSize(24,24));
    d->verticalButton = new KPushButton(KIcon(":vertical_orientation.png"),"",d->sizeWidget);
    d->verticalButton->setCheckable(true);
    d->verticalButton->setFlat(true);
    d->verticalButton->setIconSize(QSize(24,24));
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
    d->xResolution->setValue(resolution.width());
    d->xResolution->setDecimals(3);
    gridLayout->addWidget(new QLabel("Resolution X:", d->advancedWidget),0,0);
    gridLayout->addWidget(d->xResolution,0,1);

    // y resolution widget
    d->yResolution = new QDoubleSpinBox(d->advancedWidget);
    d->yResolution->setMinimum(0);
    d->yResolution->setMaximum(999999);
    d->yResolution->setValue(resolution.height());
    d->yResolution->setDecimals(3);
    gridLayout->addWidget(new QLabel("Resolution Y:", d->advancedWidget),1,0);
    gridLayout->addWidget(d->yResolution,1,1);

    // Unit widget
    d->resolutionUnitsWidget = new KComboBox(d->sizeWidget);
    d->resolutionUnitsWidget->addItems(KIPIPhotoFramesEditor::resolutionUnitsNames().values());
    d->resolutionUnitsWidget->setCurrentItem(resolutionUnits);
    gridLayout->addWidget(d->resolutionUnitsWidget,1,2);

    this->prepareSignalsConnections();

    d->updateSizeLabel();
}

void CanvasCreationDialog::prepareSignalsConnections()
{
    connect(d->paperSize, SIGNAL(activated(QString)), this, SLOT(recalculatePaperSize(const QString&)));
    connect(d->xSize,SIGNAL(valueChanged(double)),this,SLOT(widthChanged(double)));
    connect(d->ySize,SIGNAL(valueChanged(double)),this,SLOT(heightChanged(double)));
    connect(d->sizeUnitsWidget, SIGNAL(activated(QString)), this, SLOT(sizeUnitsChanged(QString)));
    connect(d->horizontalButton, SIGNAL(toggled(bool)), this, SLOT(setHorizontal(bool)));
    connect(d->verticalButton, SIGNAL(toggled(bool)), this, SLOT(setVertical(bool)));
    connect(d->xResolution, SIGNAL(valueChanged(double)), this, SLOT(xResolutionChanged(double)));
    connect(d->yResolution, SIGNAL(valueChanged(double)), this, SLOT(yResolutionChanged(double)));
    connect(d->resolutionUnitsWidget, SIGNAL(currentIndexChanged(QString)), this, SLOT(resolutionUnitsChanged(QString)));
}

QSize CanvasCreationDialog::canvasSize() const
{
    return QSize(d->WIDTH, d->HEIGHT);
}

QSizeF CanvasCreationDialog::paperSize() const
{
    return QSizeF(d->xSize->value(), d->ySize->value());
}

QSizeF CanvasCreationDialog::paperResolution() const
{
    return QSizeF(d->xResolution->value(), d->yResolution->value());
}

SizeUnits CanvasCreationDialog::sizeUnits() const
{
    return KIPIPhotoFramesEditor::sizeUnitsNames().key(d->sizeUnitsWidget->currentText());
}

ResolutionUnits CanvasCreationDialog::resolutionUnits() const
{
    return KIPIPhotoFramesEditor::resolutionUnitsNames().key(d->resolutionUnitsWidget->currentText());
}

void CanvasCreationDialog::recalculatePaperSize(const QString & paperSize)
{
    d->setPaper( d->paperSizes.value(paperSize, QPrinter::Custom) );
    d->updateSizeLabel();
    sizeUnitsChanged(d->sizeUnitsWidget->currentText());
}

void CanvasCreationDialog::sizeUnitsChanged(const QString & unitName)
{
    d->currentSizeUnit = unitName;
    SizeUnits unit = KIPIPhotoFramesEditor::sizeUnitsNames().key(unitName);
    if (unit == KIPIPhotoFramesEditor::Pixels)
    {
        d->xSize->setValue(d->WIDTH);
        d->ySize->setValue(d->HEIGHT);
        d->xSize->setDecimals(0);
        d->ySize->setDecimals(0);
        return;
    }
    d->xSize->setDecimals(5);
    d->ySize->setDecimals(5);
    ResolutionUnits resUnit = KIPIPhotoFramesEditor::resolutionUnitsNames().key(d->resolutionUnitsWidget->currentText());
    qreal sizeFactor = KIPIPhotoFramesEditor::sizeUnitsFactors().value(unit);
    qreal resolutionFactor = KIPIPhotoFramesEditor::resolutionUnitsFactors().value(resUnit);
    qreal WIDTH = (d->WIDTH * sizeFactor) / (d->xResolution->value() * resolutionFactor);
    qreal HEIGHT = (d->HEIGHT * sizeFactor) / (d->yResolution->value() * resolutionFactor);
    d->xSize->setValue(WIDTH);
    d->ySize->setValue(HEIGHT);
}

void CanvasCreationDialog::resolutionUnitsChanged(const QString & unitName)
{
    d->currentResolutionUnit = unitName;
    ResolutionUnits unit = KIPIPhotoFramesEditor::resolutionUnitsNames().key(unitName);
    if (unit == KIPIPhotoFramesEditor::PixelsPerInch)
    {
        d->xResolution->setValue(d->WIDTH_RES);
        d->yResolution->setValue(d->HEIGHT_RES);
        return;
    }
    qreal factor = KIPIPhotoFramesEditor::resolutionUnitsFactors().value(unit);
    d->xResolution->setValue(d->WIDTH_RES / factor);
    d->yResolution->setValue(d->HEIGHT_RES / factor);
}

void CanvasCreationDialog::setHorizontal(bool isHorizontal)
{
    if (isHorizontal)
    {
        if (d->WIDTH < d->HEIGHT)
        {
            d->swapSizes();
            d->updateSizeLabel();
        }
    }
    d->horizontalButton->setChecked(d->WIDTH > d->HEIGHT);
    d->verticalButton->setChecked(d->WIDTH < d->HEIGHT);
}

void CanvasCreationDialog::setVertical(bool isVertical)
{
    if (isVertical)
    {
        if (d->HEIGHT < d->WIDTH)
        {
            d->swapSizes();
            d->updateSizeLabel();
        }
    }
    d->horizontalButton->setChecked(d->WIDTH > d->HEIGHT);
    d->verticalButton->setChecked(d->WIDTH < d->HEIGHT);
}

void CanvasCreationDialog::widthChanged(double width)
{
    width = d->toPixels(width, d->xResolution->value());
    d->WIDTH = width;
    d->horizontalButton->setChecked(d->WIDTH > d->HEIGHT);
    d->verticalButton->setChecked(d->WIDTH < d->HEIGHT);
    d->updateSizeLabel();
}

void CanvasCreationDialog::heightChanged(double height)
{
    height = d->toPixels(height, d->yResolution->value());
    d->HEIGHT = height;
    d->horizontalButton->setChecked(d->WIDTH > d->HEIGHT);
    d->verticalButton->setChecked(d->WIDTH < d->HEIGHT);
    d->updateSizeLabel();
}

void CanvasCreationDialog::xResolutionChanged(double xResolution)
{
    SizeUnits sizeUnit = KIPIPhotoFramesEditor::sizeUnitsNames().key(d->sizeUnitsWidget->currentText());
    if (sizeUnit == KIPIPhotoFramesEditor::Pixels)
        return;
    ResolutionUnits resolutionUnit = KIPIPhotoFramesEditor::resolutionUnitsNames().key(d->resolutionUnitsWidget->currentText());
    qreal resolutionFactor = KIPIPhotoFramesEditor::resolutionUnitsFactors().value(resolutionUnit);
    qreal width = d->toPixels(d->xSize->value(), xResolution);
    d->WIDTH = width;
    d->WIDTH_RES = xResolution * resolutionFactor;
    d->updateSizeLabel();
}

void CanvasCreationDialog::yResolutionChanged(double yResolution)
{
    SizeUnits sizeUnit = KIPIPhotoFramesEditor::sizeUnitsNames().key(d->sizeUnitsWidget->currentText());
    if (sizeUnit == KIPIPhotoFramesEditor::Pixels)
        return;
    ResolutionUnits resolutionUnit = KIPIPhotoFramesEditor::resolutionUnitsNames().key(d->resolutionUnitsWidget->currentText());
    qreal resolutionFactor = KIPIPhotoFramesEditor::resolutionUnitsFactors().value(resolutionUnit);
    qreal height = d->toPixels(d->ySize->value(), yResolution);
    d->HEIGHT = height;
    d->HEIGHT_RES = yResolution * resolutionFactor;
    d->updateSizeLabel();
}
