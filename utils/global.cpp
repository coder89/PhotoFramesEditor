#include "global.h"
#include "UndoCommandEvent.h"
#include "photoframeseditor.h"

#include <QPrinter>
#include <QQueue>

#include <kapplication.h>

QString KIPIPhotoFramesEditor::name()
{
    return QString("pfe");
}

QString KIPIPhotoFramesEditor::uri()
{
    return QString("http://coder89.pl/pfe");
}

void KIPIPhotoFramesEditor::PFE_PostUndoCommand(QUndoCommand * command)
{
    PhotoFramesEditor::instance()->addUndoCommand(command);
}

QDomElement KIPIPhotoFramesEditor::pathToSvg(const QPainterPath & path, QDomDocument & document)
{
    // If path is empty
    if (path.isEmpty())
        return QDomElement();

    // Conversion loop
    QString str_path_d;
    int elementsCount = path.elementCount();
    for (int i = 0; i < elementsCount; ++i)
    {
        QPainterPath::Element e = path.elementAt(i);
        switch (e.type)
        {
            case QPainterPath::LineToElement:
                str_path_d.append("L " + QString::number(e.x) + " " + QString::number(e.y) + " ");
                break;
            case QPainterPath::MoveToElement:
                str_path_d.append("M " + QString::number(e.x) + " " + QString::number(e.y) + " ");
                break;
            case QPainterPath::CurveToElement:
                str_path_d.append("C " + QString::number(e.x) + " " + QString::number(e.y) + " ");
                break;
            case QPainterPath::CurveToDataElement:
                str_path_d.append(QString::number(e.x) + " " + QString::number(e.y) + " ");
                break;
            default:
                Q_ASSERT(e.type == QPainterPath::CurveToDataElement ||
                         e.type == QPainterPath::CurveToElement ||
                         e.type == QPainterPath::LineToElement ||
                         e.type == QPainterPath::MoveToElement);
        }
    }
    str_path_d.append("z");

    // If path length is empty
    if (str_path_d.isEmpty())
        return QDomElement();

    // Create QDomElement
    QDomElement element = document.createElement("path");
    element.setAttribute("d", str_path_d);
    return element;
}

QPainterPath KIPIPhotoFramesEditor::pathFromSvg(const QDomElement & element)
{
    QPainterPath result;
    if (element.tagName() != "path")
        return result;
    QString d = element.attribute("d");
    QStringList list = d.split(" ", QString::SkipEmptyParts);
    QStringList::const_iterator it = list.begin();
    QQueue<qreal> coordinates;
    QQueue<char> operations;
    while (it != list.end())
    {
        if (*it == "M")
            operations.enqueue('M');
        else if (*it == "L")
            operations.enqueue('L');
        else if (*it == "C")
            operations.enqueue('C');
        else if (*it == "z")
            operations.enqueue('z');
        else
        {
            QString str = *it;
            bool isOK;
            qreal value = str.toDouble(&isOK);
            if (isOK)
                coordinates.enqueue(value);
            else
                return QPainterPath();
        }
        ++it;
    }
    while (operations.count())
    {
        char opc = operations.dequeue();
        switch (opc)
        {
            case 'M':
                if (coordinates.count() < 2)
                    return QPainterPath();
                result.moveTo(coordinates.dequeue(),
                              coordinates.dequeue());
                break;
            case 'L':
                if (coordinates.count() < 2)
                    return QPainterPath();
                result.lineTo(coordinates.dequeue(),
                              coordinates.dequeue());
                break;
            case 'C':
                if (coordinates.count() < 4)
                    return QPainterPath();
                result.cubicTo(coordinates.dequeue(),
                               coordinates.dequeue(),
                               coordinates.dequeue(),
                               coordinates.dequeue(),
                               coordinates.dequeue(),
                               coordinates.dequeue());
                break;
            case 'z':
                result.closeSubpath();
                break;
            default:
                return QPainterPath();
        }
    }
    return result;
}

const QMap<KIPIPhotoFramesEditor::ResolutionUnits,qreal> & KIPIPhotoFramesEditor::resolutionUnitsFactors()
{
    static QMap<ResolutionUnits,qreal> resolutionUnits;
    if (resolutionUnits.isEmpty())
    {
        resolutionUnits.insert(PixelsPerMilimeter, 25.4);
        resolutionUnits.insert(PixelsPerCentimeter, 2.54);
        resolutionUnits.insert(PixelsPerMeter, 0.0254);
        resolutionUnits.insert(PixelsPerInch, 1);
        resolutionUnits.insert(PixelsPerFeet, 0.083333);
        resolutionUnits.insert(PixelsPerYard, 0.027778);
        resolutionUnits.insert(PixelsPerPoint, 72.27);
        resolutionUnits.insert(PixelsPerPicas, 6.0225);
    }
    return resolutionUnits;
}

const QMap<KIPIPhotoFramesEditor::ResolutionUnits,QString> & KIPIPhotoFramesEditor::resolutionUnitsNames()
{
    static QMap<ResolutionUnits,QString> resolutionUnits;
    if (resolutionUnits.isEmpty())
    {
        resolutionUnits.insert(PixelsPerMilimeter, "px/mm");
        resolutionUnits.insert(PixelsPerCentimeter, "px/cm");
        resolutionUnits.insert(PixelsPerMeter, "px/m");
        resolutionUnits.insert(PixelsPerInch, "px/in");
        resolutionUnits.insert(PixelsPerFeet, "px/ft");
        resolutionUnits.insert(PixelsPerYard, "px/yd");
        resolutionUnits.insert(PixelsPerPoint, "px/pt");
        resolutionUnits.insert(PixelsPerPicas, "px/pc");
    }
    return resolutionUnits;
}

const QMap<KIPIPhotoFramesEditor::SizeUnits,qreal> & KIPIPhotoFramesEditor::sizeUnitsFactors()
{
    static QMap<KIPIPhotoFramesEditor::SizeUnits,qreal> sizeUnits;
    if (sizeUnits.isEmpty())
    {
        sizeUnits.insert(Milimeters, 25.4);
        sizeUnits.insert(Centimeters, 2.54);
        sizeUnits.insert(Meters, 0.0254);
        sizeUnits.insert(Inches, 1);
        sizeUnits.insert(Feet, 0.083333);
        sizeUnits.insert(Yards, 0.027778);
        sizeUnits.insert(Points, 72.27);
        sizeUnits.insert(Picas, 6.0225);
    }
    return sizeUnits;
}

const QMap<KIPIPhotoFramesEditor::SizeUnits,QString> & KIPIPhotoFramesEditor::sizeUnitsNames()
{
    static QMap<SizeUnits,QString> sizeUnits;
    if (sizeUnits.isEmpty())
    {
        sizeUnits.insert(Pixels, "px");
        sizeUnits.insert(Milimeters, "mm");
        sizeUnits.insert(Centimeters, "cm");
        sizeUnits.insert(Meters, "m");
        sizeUnits.insert(Inches, "in");
        sizeUnits.insert(Feet, "ft");
        sizeUnits.insert(Yards, "yd");
        sizeUnits.insert(Points, "pt");
        sizeUnits.insert(Picas, "pc");
    }
    return sizeUnits;
}
