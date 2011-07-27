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
    PhotoFramesEditor::instancePhotoFramesEditor()->addUndoCommand(command);
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
        resolutionUnits.insert(PixelsPerFeet, 0.83333);
        resolutionUnits.insert(PixelsPerYard, 0.027778);
        resolutionUnits.insert(PixelsPerPoint, 72);
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
        sizeUnits.insert(Feet, 0.83333);
        sizeUnits.insert(Yards, 0.027778);
        sizeUnits.insert(Points, 72);
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

QSizeF PFE_paperSize(QPrinter::PaperSize paperSize, QPrinter::Unit unit)
{
    QSizeF tempSize;
    switch (paperSize)
    {
        case QPrinter::A0:
            tempSize = QSizeF(841,1189);
            break;
        case QPrinter::A1:
            tempSize = QSizeF(594,841);
            break;
        case QPrinter::A2:
            tempSize = QSizeF(420,594);
            break;
        case QPrinter::A3:
            tempSize = QSizeF(297,420);
            break;
        case QPrinter::A4:
            tempSize = QSizeF(210,297);
            break;
        case QPrinter::A5:
            tempSize = QSizeF(148,210);
            break;
        case QPrinter::A6:
            tempSize = QSizeF(105,148);
            break;
        case QPrinter::A7:
            tempSize = QSizeF(74,105);
            break;
        case QPrinter::A8:
            tempSize = QSizeF(52,74);
            break;
        case QPrinter::A9:
            tempSize = QSizeF(37,52);
            break;
        case QPrinter::B0:
            tempSize = QSizeF(1000,1414);
            break;
        case QPrinter::B1:
            tempSize = QSizeF(707,1000);
            break;
        case QPrinter::B2:
            tempSize = QSizeF(500,707);
            break;
        case QPrinter::B3:
            tempSize = QSizeF(353,500);
            break;
        case QPrinter::B4:
            tempSize = QSizeF(250,353);
            break;
        case QPrinter::B5:
            tempSize = QSizeF(176,250);
            break;
        case QPrinter::B6:
            tempSize = QSizeF(125,176);
            break;
        case QPrinter::B7:
            tempSize = QSizeF(88,125);
            break;
        case QPrinter::B8:
            tempSize = QSizeF(62,88);
            break;
        case QPrinter::B9:
            tempSize = QSizeF(33,62);
            break;
        case QPrinter::B10:
            tempSize = QSizeF(31,44);
            break;
        case QPrinter::C5E:
            tempSize = QSizeF(163,229);
            break;
        case QPrinter::Comm10E:
            tempSize = QSizeF(105,241);
            break;
        case QPrinter::DLE:
            tempSize = QSizeF(110,220);
            break;
        case QPrinter::Executive:
            tempSize = QSizeF(190.5,254);
            break;
        case QPrinter::Folio:
            tempSize = QSizeF(210,330);
            break;
        case QPrinter::Ledger:
            tempSize = QSizeF(431.8,279.4);
            break;
        case QPrinter::Legal:
            tempSize = QSizeF(215.9,355.6);
            break;
        case QPrinter::Letter:
            tempSize = QSizeF(215.9,279.4);
            break;
        case QPrinter::Tabloid:
            tempSize = QSizeF(279.4,431.8);
            break;
        default:
            return QSizeF();
    }
    return tempSize;
}
