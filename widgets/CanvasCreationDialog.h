#ifndef CANVASCREATIONDIALOG_H
#define CANVASCREATIONDIALOG_H

// KDE
#include <kdialog.h>

// Qt
#include <QSize>

namespace KIPIPhotoFramesEditor
{
    class CanvasCreationDialog : public KDialog
    {
            Q_OBJECT

        public:

            explicit CanvasCreationDialog(QWidget *parent = 0);
            ~CanvasCreationDialog();
            QSize canvasSize();

        Q_SIGNALS:

        public Q_SLOTS:

        protected Q_SLOTS:

            void recalculatePaperSize(const QString & paperSize);
            void sizeUnitsChanged(const QString & unit);
            void resolutionUnitsChanged(const QString & unit);
            void setHorizontal(bool);
            void setVertical(bool);
            void widthChanged(double width);
            void heightChanged(double height);

        private:

            void setupDialog();

            class CanvasCreationDialogPriv;
            CanvasCreationDialogPriv * d;
    };
}

#endif // CANVASCREATIONDIALOG_H
