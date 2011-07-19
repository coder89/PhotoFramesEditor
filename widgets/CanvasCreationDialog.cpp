#include "CanvasCreationDialog.h"
#include "global.h"

// KDE
#include <kcombobox.h>

// Qt
#include <QFormLayout>
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>

namespace KIPIPhotoFramesEditor
{
    class CanvasCreationDialog::CanvasCreationDialogPriv
    {
        public:
            CanvasCreationDialogPriv() :
                xSize(0),
                ySize(0)
            {
                //values.insert("A4", QPair());
                //values.insert("A4", QPair());
                //values.insert("A4", QPair());
                //values.insert("A4", QPair());
            }

            ~CanvasCreationDialogPriv()
            {
                Q_DELETE(xSize)
                Q_DELETE(ySize)
            }

            QWidget *   sizeWidget;
            KComboBox * paperSize;
            QSpinBox *  xSize;
            QSpinBox *  ySize;
            QMap<QString,QPair<int,int> > values;
    };

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
        QVBoxLayout * vLayout = new  QVBoxLayout(main);

        d->sizeWidget = new QGroupBox("Canvas size", main);
        QFormLayout * formLayout = new QFormLayout(d->sizeWidget);
        //-----------------------------------------------------------------------------------
        d->paperSize = new KComboBox(d->sizeWidget);
        connect(d->paperSize, SIGNAL(activated(QString)), this, SLOT(recalculatePaperSize(const QString&)));
        //d->paperSize->addItems(d->values);
        formLayout->addRow(new QLabel("Paper size:", d->sizeWidget), d->paperSize);
        //-----------------------------------------------------------------------------------
        d->xSize = new QSpinBox(d->sizeWidget);
        d->xSize->setMinimum(1);
        d->xSize->setMaximum(999999);
        d->xSize->setValue(800);
        formLayout->addRow(new QLabel("Width:", d->sizeWidget), d->xSize);
        //-----------------------------------------------------------------------------------
        d->ySize = new QSpinBox(d->sizeWidget);
        d->ySize->setMinimum(1);
        d->ySize->setMaximum(999999);
        d->ySize->setValue(800);
        formLayout->addRow(new QLabel("Height:", d->sizeWidget), d->ySize);
        //-----------------------------------------------------------------------------------
        d->sizeWidget->setLayout(formLayout);

        vLayout->addWidget(d->sizeWidget);
        main->setLayout(vLayout);

        setMainWidget(main);
    }

    QSize CanvasCreationDialog::canvasSize()
    {
        return QSize(d->xSize->value(), d->ySize->value());
    }

    void CanvasCreationDialog::recalculatePaperSize(const QString & paperSize)
    {
        if (paperSize == "Custom")
        {
            d->xSize->setEnabled(true);
            d->ySize->setEnabled(true);
            return;
        }
        d->xSize->setEnabled(false);
        d->ySize->setEnabled(false);

    }
}
