#ifndef CANVASSIZE_H
#define CANVASSIZE_H

#include <QString>
#include <QMap>

namespace KIPIPhotoFramesEditor
{
    class CanvasSize
    {
        public:

            enum ResolutionUnits
            {
                UnknownResolutionUnit,
                PixelsPerMilimeter,
                PixelsPerCentimeter,
                PixelsPerMeter,
                PixelsPerInch,
                PixelsPerFeet,
                PixelsPerYard,
                PixelsPerPoint,
                PixelsPerPicas,
            };

            enum SizeUnits
            {
                UnknownSizeUnit,
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

            static QList<qreal> resolutionUnitsFactors();
            static QList<QString> resolutionUnitsNames();
            static QList<ResolutionUnits> resolutionUnits();
            static qreal resolutionUnitFactor(ResolutionUnits unit);
            static qreal resolutionUnitFactor(QString unitName);
            static QString resolutionUnitName(ResolutionUnits unit);
            static QString resolutionUnitName(qreal factor);

            static QList<qreal> sizeUnitsFactors();
            static QList<QString> sizeUnitsNames();
            static QList<SizeUnits> sizeUnits();
            static qreal sizeUnitFactor(SizeUnits unit);
            static qreal sizeUnitFactor(QString unitName);
            static QString sizeUnitName(SizeUnits unit);
            static QString sizeUnitName(qreal factor);

        private:

            static QMap<ResolutionUnits,qreal> resolution_factors;
            static QMap<ResolutionUnits,QString> resolution_names;

            static QMap<SizeUnits,qreal> size_factors;
            static QMap<SizeUnits,QString> size_names;

            static void prepare_maps();
    };
}

#endif // CANVASSIZE_H
