#ifndef CANVASCREATIONDIALOG_H
#define CANVASCREATIONDIALOG_H

#include "global.h"

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

            explicit CanvasCreationDialog(QWidget * parent = 0);
            explicit CanvasCreationDialog(const QSizeF & size, SizeUnits sizeUnits, const QSizeF & resolution, const QString & resolutionUnits, QWidget * parent = 0);
            ~CanvasCreationDialog();

            Q_PROPERTY(QSize canvas_size READ canvasSize)
            QSize canvasSize() const;

            Q_PROPERTY(QSizeF paper_size READ paperSize)
            QSizeF paperSize() const;

            Q_PROPERTY(QSizeF paper_resolution READ paperResolution)
            QSizeF paperResolution() const;

            Q_PROPERTY(SizeUnits size_units READ sizeUnits)
            SizeUnits sizeUnits() const;

            Q_PROPERTY(ResolutionUnits resolution_units READ resolutionUnits)
            ResolutionUnits resolutionUnits() const;

        protected Q_SLOTS:

            void recalculatePaperSize(const QString & paperSize);
            void sizeUnitsChanged(const QString & unit);
            void resolutionUnitsChanged(const QString & unit);
            void setHorizontal(bool);
            void setVertical(bool);
            void widthChanged(double width);
            void heightChanged(double height);
            void xResolutionChanged(double xResolution);
            void yResolutionChanged(double yResolution);

        private:

            void setupDialog(const QSizeF & size, const QString & sizeUnits, const QSizeF & resolution, const QString & resolutionUnits);
            void prepareSignalsConnections();

            class CanvasCreationDialogPriv;
            CanvasCreationDialogPriv * d;
    };
}

#endif // CANVASCREATIONDIALOG_H
