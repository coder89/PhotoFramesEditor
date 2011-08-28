#include "CanvasSize.h"

using namespace KIPIPhotoFramesEditor;

QMap<CanvasSize::ResolutionUnits,qreal> CanvasSize::resolution_factors;
QMap<CanvasSize::ResolutionUnits,QString> CanvasSize::resolution_names;
QMap<CanvasSize::SizeUnits,qreal> CanvasSize::size_factors;
QMap<CanvasSize::SizeUnits,QString> CanvasSize::size_names;

void CanvasSize::prepare_maps()
{
    if (resolution_factors.isEmpty() || resolution_names.isEmpty())
    {
        resolution_factors.clear();
        resolution_names.clear();

        resolution_factors.insert(UnknownResolutionUnit, 0);
        resolution_factors.insert(PixelsPerMilimeter, 25.4);
        resolution_factors.insert(PixelsPerCentimeter, 2.54);
        resolution_factors.insert(PixelsPerMeter, 0.0254);
        resolution_factors.insert(PixelsPerInch, 1);
        resolution_factors.insert(PixelsPerFeet, 0.083333);
        resolution_factors.insert(PixelsPerYard, 0.027778);
        resolution_factors.insert(PixelsPerPoint, 72.27);
        resolution_factors.insert(PixelsPerPicas, 6.0225);

        resolution_names.insert(PixelsPerMilimeter, "px/mm");
        resolution_names.insert(PixelsPerCentimeter, "px/cm");
        resolution_names.insert(PixelsPerMeter, "px/m");
        resolution_names.insert(PixelsPerInch, "px/in");
        resolution_names.insert(PixelsPerFeet, "px/ft");
        resolution_names.insert(PixelsPerYard, "px/yd");
        resolution_names.insert(PixelsPerPoint, "px/pt");
        resolution_names.insert(PixelsPerPicas, "px/pc");
    }

    if (size_factors.isEmpty() || size_names.isEmpty())
    {
        size_factors.clear();
        size_names.clear();

        size_factors.insert(UnknownSizeUnit, 0);
        size_factors.insert(Milimeters, 25.4);
        size_factors.insert(Centimeters, 2.54);
        size_factors.insert(Meters, 0.0254);
        size_factors.insert(Inches, 1);
        size_factors.insert(Feet, 0.083333);
        size_factors.insert(Yards, 0.027778);
        size_factors.insert(Points, 72.27);
        size_factors.insert(Picas, 6.0225);

        size_names.insert(Pixels, "px");
        size_names.insert(Milimeters, "mm");
        size_names.insert(Centimeters, "cm");
        size_names.insert(Meters, "m");
        size_names.insert(Inches, "in");
        size_names.insert(Feet, "ft");
        size_names.insert(Yards, "yd");
        size_names.insert(Points, "pt");
        size_names.insert(Picas, "pc");
    }
}

QList<qreal> CanvasSize::resolutionUnitsFactors()
{
    prepare_maps();
    return resolution_factors.values();
}

QList<QString> CanvasSize::resolutionUnitsNames()
{
    prepare_maps();
    return resolution_names.values();
}

QList<CanvasSize::ResolutionUnits> CanvasSize::resolutionUnits()
{
    prepare_maps();
    return resolution_factors.keys();
}

qreal CanvasSize::resolutionUnitFactor(CanvasSize::ResolutionUnits unit)
{
    prepare_maps();
    return resolution_factors.value(unit, 0);
}

qreal CanvasSize::resolutionUnitFactor(QString unitName)
{
    prepare_maps();
    return resolution_factors.value( resolution_names.key(unitName, UnknownResolutionUnit) );
}

QString CanvasSize::resolutionUnitName(CanvasSize::ResolutionUnits unit)
{
    prepare_maps();
    return resolution_names.value(unit);
}

QString CanvasSize::resolutionUnitName(qreal factor)
{
    prepare_maps();
    return resolution_names.value(resolution_factors.key(factor));
}

QList<qreal> CanvasSize::sizeUnitsFactors()
{
    prepare_maps();
    return size_factors.values();
}

QList<QString> CanvasSize::sizeUnitsNames()
{
    prepare_maps();
    return size_names.values();
}

QList<CanvasSize::SizeUnits> CanvasSize::sizeUnits()
{
    prepare_maps();
    return size_factors.keys();
}

qreal CanvasSize::sizeUnitFactor(CanvasSize::SizeUnits unit)
{
    prepare_maps();
    return size_factors.value(unit);
}

qreal CanvasSize::sizeUnitFactor(QString unitName)
{
    prepare_maps();
    return size_factors.value(size_names.key(unitName, UnknownSizeUnit));
}

QString CanvasSize::sizeUnitName(CanvasSize::SizeUnits unit)
{
    prepare_maps();
    return size_names.value(unit);
}

QString CanvasSize::sizeUnitName(qreal factor)
{
    prepare_maps();
    return size_names.value(size_factors.key(factor));
}
