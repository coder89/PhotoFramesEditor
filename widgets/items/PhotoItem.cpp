#include "PhotoItem.h"
#include "PhotoEffectsGroup.h"
#include "PhotoEffectsLoader.h"
#include "global.h"

#include <QBuffer>
#include <QStyleOptionGraphicsItem>
#include <QFile>
#include <QImageReader>

#include <kapplication.h>

using namespace KIPIPhotoFramesEditor;

PhotoItem::PhotoItem(const QImage & photo, QGraphicsScene * scene) :
    AbstractPhoto(scene)
{
    this->setupItem(QPixmap::fromImage(photo));
}

QDomElement PhotoItem::toSvg(QDomDocument & document, bool embedAll) const
{
    QDomElement result = AbstractPhoto::toSvg(document, embedAll);
    result.setAttribute("class", "PhotoItem");

    // 'defs' tag
    QDomElement defs = document.createElement("defs");
    defs.setAttribute("class", "data");
    result.appendChild(defs);

    // 'defs'-> pfe:'data'
    QDomElement appNS = document.createElementNS(KIPIPhotoFramesEditor::uri(), "data");
    appNS.setPrefix(KIPIPhotoFramesEditor::name());
    defs.appendChild(appNS);

    if (!m_image_path.isEmpty())
    {
        // 'defs'-> pfe:'data' ->'path'
        QDomElement path = KIPIPhotoFramesEditor::pathToSvg(m_image_path, document);
        path.setAttribute("class", "m_image_path");
        path.setPrefix(KIPIPhotoFramesEditor::name());
        appNS.appendChild(path);
    }

    QDomElement image = document.createElement("image");
    if (embedAll && !m_pixmap_original.isNull())
    {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        m_pixmap_original.save(&buffer, "PNG");
        QString data(byteArray);
        image.setAttribute("data", data);
    }
    else
    {
        /// TODO : saving digiKam file "ID??" or file path
    }

    return result;
}

PhotoItem * PhotoItem::fromSvg(QDomElement & element)
{
    PhotoItem * item = new PhotoItem();
    if (item->AbstractPhoto::fromSvg(element));
    {
        // Gets data field
        QDomElement defs = element.firstChildElement("defs");
        while (!defs.isNull() && defs.attribute("class") != "data")
            defs = defs.nextSiblingElement("defs");
        if (defs.isNull())
            goto _delete;
        QDomElement data = defs.firstChildElement("data");
        if (data.isNull())
            goto _delete;

        // m_image_path
        QDomElement path = data.firstChildElement("path");
        if (path.isNull())
            goto _delete;
        item->m_image_path = KIPIPhotoFramesEditor::pathFromSvg(path);
        if (item->m_image_path.isEmpty())
            goto _delete;

        // m_pixmap_original
        QDomElement image = data.firstChildElement("image");
        QString imageData = image.attribute("data");
        QImage img;
        // Fullsize image is embedded in SVG file!
        if (!imageData.isEmpty())
        {
            QBuffer buf(&imageData.toAscii());
            QImageReader reader(&buf);
            reader.setAutoDetectImageFormat(true);
            if (!reader.canRead())
                goto _delete;
            img = QImage(reader.size(), QImage::Format_ARGB32);
            if (!reader.read(&img))
                goto _delete;
        }
        // Try to find file from path attribute or digiKam's database
        else
        {
            /// TODO
//            QString fileName = image.attribute("path");
//            if (fileName.isEmpty())
//                goto _delete;
//            if (!QFile::exists(fileName))
//            {}
//            QImageReader reader(fileName);
//            if (!reader.canRead())
//                goto _delete;
//            reader.setAutoDetectImageFormat(true);
//            QImage image(reader.size());
//            if (!reader.read(&image))
//                goto _delete;
        }
        item->setPixmap(QPixmap::fromImage(img));

        return item;
    }
_delete:
    delete item;
    return 0;
}

QDomElement PhotoItem::svgVisibleArea(QDomDocument & document) const
{
    // 'defs'->'image'
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    m_pixmap.save(&buffer, "PNG");
    QDomElement img = document.createElement("image");
    img.setAttribute("width",m_pixmap.width());
    img.setAttribute("height",m_pixmap.height());
    img.setAttribute("xlink:href","data:image/png;base64,"+QString::fromLatin1(byteArray.toBase64().data()));
    return img;
}

void PhotoItem::setPixmap(const QPixmap & pixmap)
{
    if (pixmap.isNull())
        return;
    this->m_pixmap_original = pixmap;
    this->refresh();
}

void PhotoItem::updateIcon()
{
    this->setIcon(QIcon(m_pixmap.scaled(100,100,Qt::KeepAspectRatioByExpanding)));
}

void PhotoItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    if (!m_pixmap.isNull())
    {
        QRect myRect = this->boundingRect().toRect();
        painter->drawPixmap(m_pixmap.rect(), m_pixmap, option->rect.intersected(myRect));
    }
}

void PhotoItem::refresh()
{
    if (m_pixmap_original.isNull())
        return;
    this->m_pixmap = effectsGroup()->apply(m_pixmap_original.scaled(this->boundingRect().size().toSize(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    this->updateIcon();
    this->recalcShape();
    this->update(this->boundingRect());
}

QtAbstractPropertyBrowser * PhotoItem::propertyBrowser()
{}

void PhotoItem::setupItem(const QPixmap & photo)
{
    if (photo.isNull())
        return;

    m_pixmap_original = photo;

    // Scaling if to big
    QSize s = photo.size();
    QRect r = photo.rect();
    QSize sceneSize = scene()->sceneRect().size().toSize();
    if (sceneSize.width()<s.width() || sceneSize.height()<s.height())
    {
        s.scale(sceneSize*0.8, Qt::KeepAspectRatio);
        r.setSize(s);
    }

    QPainterPath p;
    p.addRect(r);
    m_image_path = p;
    this->m_image_path = m_image_path.simplified();
    this->recalcShape();

    // Create effective pixmap
    this->refresh();

    this->setFlag(QGraphicsItem::ItemIsSelectable);

    // Default border style
    this->setBorderWidth(0);
    this->setBorderColor(Qt::black);
    this->setBorderCornersStyle(Qt::RoundJoin);
}

void PhotoItem::recalcShape()
{
//    if (m_border_width)
//    {
//        QPainterPathStroker s;
//        s.setWidth(m_border_width);
//        s.setJoinStyle(m_border_corner_style);
//        m_border_path = s.createStroke(m_image_path);
//    }
//    else
//        m_border_path = QPainterPath();
//    m_complete_path = m_border_path.united(m_image_path);
//    prepareGeometryChange();

//    try // I'm not sure how to properly/safely cast from QGraphicsScene* -> Scene*
//    {
//        Scene * m_scene = (Scene*)this->scene();
//        if (m_scene)
//            m_scene->updateSelection();
//    }
//    catch(...)
//    {}
    m_complete_path = m_image_path;
}
