#ifndef GRIDSETUPDIALOG_H
#define GRIDSETUPDIALOG_H

#include <kdialog.h>
#include <QDoubleSpinBox>

namespace KIPIPhotoFramesEditor
{
    class GridSetupDialog : public KDialog
    {
            Q_OBJECT

            QWidget * centralWidget;
            QDoubleSpinBox * x;
            QDoubleSpinBox * y;

        public:

            GridSetupDialog(QWidget * parent = 0);
            void setHorizontalDistance(qreal value);
            void setVerticalDistance(qreal value);
            qreal horizontalDistance() const;
            qreal verticalDistance() const;
    };
}

#endif // GRIDSETUPDIALOG_H
