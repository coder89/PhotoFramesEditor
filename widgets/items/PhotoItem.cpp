#include "PhotoItem.h"
#include "PhotoEffectsGroup.h"
#include "PhotoEffectsLoader.h"

#include <QBuffer>
#include <QStyleOptionGraphicsItem>

using namespace KIPIPhotoFramesEditor;

PhotoItem::PhotoItem(const QImage & photo, QGraphicsScene * scene) :
    AbstractPhoto(scene)
{
    this->setupItem(QPixmap::fromImage(photo));
}

QDomNode PhotoItem::toSvg(QDomDocument & document)
{
    QDomNode result = AbstractPhoto::toSvg(document);

    // 'defs' tag
    QDomElement defs = document.createElement("defs");
    result.appendChild(defs);

    // 'defs'->'clipPath'
    QDomElement clipPath = document.createElement("clipPath");
    clipPath.setAttribute("id",this->name().simplified().replace(" ","_"));
    defs.appendChild(clipPath);

    if (!m_image_path.isEmpty())
    {
        // 'defs'->'clipPath'->'path'
        QDomElement path = document.createElement("path");
        path.setAttribute("d", pathToString(m_image_path));
        path.setAttribute("class", "m_image_path");
        defs.appendChild(path);
    }

    return result;
}

PhotoItem * PhotoItem::fromSvg(QDomElement & element)
{
    if (element.tagName() == "g")
    {
    }
    return 0;
}

void PhotoItem::setPixmap(const QPixmap & pixmap)
{
    if (pixmap.isNull())
        return;
    this->m_pixmap_original = pixmap;
    this->refresh();
}

QDomElement PhotoItem::svgVisibleArea(QDomDocument & document) const
{
    const QString imageID = "img_" + this->name().simplified().replace(" ","_");

    // 'defs'->'image'
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    m_pixmap.save(&buffer, "PNG");
    QDomElement img = document.createElement("image");
    img.setAttribute("width",m_pixmap.width());
    img.setAttribute("height",m_pixmap.height());
    img.setAttribute("xlink:href","data:image/png;base64,"+QString::fromLatin1(byteArray.toBase64().data()));
    img.setAttribute("id",imageID);

    return img;
}

void PhotoItem::updateIcon()
{
    this->setIcon(QIcon(m_pixmap.scaled(100,100,Qt::KeepAspectRatioByExpanding)));
}

void PhotoItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QRect myRect = this->boundingRect().toRect();
    painter->drawPixmap(m_pixmap.rect(), m_pixmap, option->rect.intersected(myRect));
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
