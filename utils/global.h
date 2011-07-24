#ifndef GLOBAL_H
#define GLOBAL_H

#define Q_DELETE(ptr)   if(ptr) { delete ptr; ptr = 0; }

#include <QUndoCommand>
#include <QPrinter>
#include <QSizeF>
#include <QMap>
#include <QDomDocument>
#include <QPainterPath>

namespace KIPIPhotoFramesEditor
{
    extern QString name();
    extern QString uri();
    extern void PFE_PostUndoCommand(QUndoCommand * command);
    extern QDomElement pathToSvg(const QPainterPath & path, QDomDocument & document);
    extern QPainterPath pathFromSvg(const QDomElement & element);

    enum ResolutionUnits
    {
        PixelsPerMilimeter,
        PixelsPerCentimeter,
        PixelsPerMeter,
        PixelsPerInch,
        PixelsPerFeet,
        PixelsPerYard,
        PixelsPerPoint,
        PixelsPerPicas,
    };

    extern const QMap<ResolutionUnits,qreal> & resolutionUnitsFactors();
    extern const QMap<ResolutionUnits,QString> & resolutionUnitsNames();

    enum SizeUnits
    {
        Pixels,
        Milimeters,
        Centimeters,
        Meters,
        Inches,
        Feet,
        Yards,
        Points,
        Picas,
    };

    extern const QMap<SizeUnits,qreal> & sizeUnitsFactors();
    extern const QMap<SizeUnits,QString> & sizeUnitsNames();
}

#endif // GLOBAL_H
