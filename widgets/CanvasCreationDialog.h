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
            QSize canvasSize();

        Q_SIGNALS:

        public Q_SLOTS:

        protected Q_SLOTS:

            void recalculatePaperSize(const QString & paperSize);

        private:

            void setupDialog();

            class CanvasCreationDialogPriv;
            CanvasCreationDialogPriv * d;
    };
}

#endif // CANVASCREATIONDIALOG_H
