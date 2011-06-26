#ifndef GRIDCONFIGDIALOG_H
#define GRIDCONFIGDIALOG_H

#include "kdialog.h"
#include <QLabel>
#include <QSpinBox>
#include <QFormLayout>

namespace KIPIPhotoFramesEditor
{

    class GridConfigDialog : public KDialog
    {
        public:

            GridConfigDialog(QWidget * parent = 0, int defaultX = 0, int defaultY = 0) :
                QDialog(parent),
                xSpin(this),
                ySpin(this)
            {
                QWidget * main = this->mainWidget();
                QFormLayout * layout = new QFormLayout(main);
                layout->addRow(new QLabel("Horizontal gap:", main), xSpin);
                layout->addRow(new QLabel("Vertical gap:", main), ySpin);
                xSpin.setValue(defaultX);
                ySpin.setValue(defaultY);
                main->setLayout(layout);
                this->setModal(true);
                this->setButtons(Ok | Cancel);
                this->setDefaultButton(Ok);
            }

        private:

            QSpinBox xSpin;
            QSpinBox ySpin;
    };

}

#endif // GRIDCONFIGDIALOG_H
