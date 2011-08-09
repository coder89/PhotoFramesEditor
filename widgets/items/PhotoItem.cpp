#include "PhotoItem.h"
#include "PhotoEffectsGroup.h"
#include "PhotoEffectsLoader.h"
#include "ImageFileDialog.h"
#include "BordersGroup.h"
#include "global.h"
#include "PFESettings.h"
#include "photoframeseditor.h"

#include <QBuffer>
#include <QStyleOptionGraphicsItem>
#include <QFile>
#include <QImageReader>
#include <QGraphicsScene>

#include <kapplication.h>
#include <kmessagebox.h>
#include <klocalizedstring.h>
#include <kstandarddirs.h>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::PhotoItemPrivate
{
    PhotoItemPrivate(PhotoItem * item) :
        m_item(item)
    {}

    static QString locateFile(const QString & filePath);

    PhotoItem * m_item;

    friend class PhotoItem;
};

QString PhotoItemPrivate::locateFile(const QString & filePath)
{
    QString resultPath = filePath;
    if (!resultPath.isEmpty())
    {
        // Try to open existing file
        if (!QFile::exists(resultPath))
        {
            int result = KMessageBox::questionYesNo(0,
                                                    i18n("Can't find image file in this location:\n %s\n\n"
                                                         "Would you like to set new location of this file?\n"
                                                         "If not this image will be removed from the composition.").append(resultPath.toAscii()),
                                                    i18n("File reading error"));
            if (result != KMessageBox::Yes)
                resultPath = QString();
            else
            {
                KUrl fileUrl(filePath);
                ImageFileDialog dialog(fileUrl);
                result = dialog.exec();
                resultPath = dialog.selectedFile();
                if (result != ImageFileDialog::Accepted || !QFile::exists(resultPath))
                    resultPath = QString();
            }
        }
    }
    return resultPath;
}

PhotoItem::PhotoItem(const QImage & photo) :
    AbstractPhoto(),
    d(new PhotoItemPrivate(this))
{
    this->setupItem(QPixmap::fromImage(photo));
}

PhotoItem::~PhotoItem()
{
    delete d;
}

QDomElement PhotoItem::toSvg(QDomDocument & document) const
{
    QDomElement result = AbstractPhoto::toSvg(document);
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
    // Saving image data
    if (PFESettings::embedImagesData() && !m_pixmap_original.isNull())
    {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        m_pixmap_original.save(&buffer, "PNG");
        QString data(byteArray);
        image.setAttribute("data", data);
    }

    if (PhotoFramesEditor::instance()->hasInterface())
    {
        /// Save with KIPI::Interface
    }

    // Saving image path
    if (!m_file_path.isEmpty())
        image.setAttribute("xlink:href", m_file_path);

    return result;
}

PhotoItem * PhotoItem::fromSvg(QDomElement & element)
{
    PhotoItem * item = new PhotoItem();
    if (item->AbstractPhoto::fromSvg(element))
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
        QString imageAttribute;
        QImage img;
        // Try to find file from path attribute
        if ( !(imageAttribute = PhotoItemPrivate::locateFile( image.attribute("xlink:href") )).isEmpty() )
        {
            QImageReader reader(imageAttribute);
            if (!reader.canRead())
                goto _delete;
            reader.setAutoDetectImageFormat(true);
            img = QImage(reader.size(), QImage::Format_ARGB32_Premultiplied);
            if (!reader.read(&img))
                goto _delete;
        }
        // Try to load from digikam database
        else if (!(imageAttribute = image.attribute("kipi")).isEmpty())
        {
            /// TODO : load using KIPI::Interface
        }
        // Fullsize image is embedded in SVG file!
        else if (!(imageAttribute = image.attribute("data")).isEmpty())
        {
            QByteArray array = imageAttribute.toAscii();
            QBuffer buf(&array);
            QImageReader reader(&buf);
            reader.setAutoDetectImageFormat(true);
            if (!reader.canRead())
                goto _delete;
            img = QImage(reader.size(), QImage::Format_ARGB32_Premultiplied);
            if (!reader.read(&img))
                goto _delete;
        }
        else
            goto _delete;
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
    img.setAttribute("xlink:href",QString("data:image/png;base64,")+byteArray.toBase64());
    return img;
}

QVariant PhotoItem::itemChange(GraphicsItemChange change, const QVariant & value)
{
    switch (change)
    {
        case ItemSceneChange:
            {
                QGraphicsScene * scene = value.value<QGraphicsScene*>();
                if (scene)
                    fitToRect(scene->sceneRect().toRect());
            }
            break;
        default:
            break;
    }
    return AbstractPhoto::itemChange(change, value);
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

void PhotoItem::fitToRect(const QRect & rect)
{
    // Scaling if to big
    QSize s = m_pixmap_original.size();
    QRect r = m_pixmap_original.rect();
    if (rect.isValid() && (rect.width()<s.width() || rect.height()<s.height()))
    {
        s.scale(rect.size()*0.8, Qt::KeepAspectRatio);
        r.setSize(s);
    }

    QPainterPath p;
    p.addRect(r);
    m_image_path = p;
    this->m_image_path = m_image_path.simplified();
    this->recalcShape();

    // Create effective pixmap
    this->refresh();
}

void PhotoItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    if (!m_pixmap.isNull())
    {
        painter->drawPixmap(m_pixmap.rect(),
                            m_pixmap);
    }
    AbstractPhoto::paint(painter, option, widget);
}

void PhotoItem::refreshItem()
{
    if (m_pixmap_original.isNull())
        return;
    this->m_pixmap = effectsGroup()->apply( m_pixmap_original.scaled(this->itemShape().boundingRect().size().toSize(),
                                                                     Qt::IgnoreAspectRatio,
                                                                     Qt::SmoothTransformation));
    this->updateIcon();
    this->recalcShape();
    this->update();
}

QtAbstractPropertyBrowser * PhotoItem::propertyBrowser()
{}

void PhotoItem::setupItem(const QPixmap & photo)
{
    if (photo.isNull())
        return;

    m_pixmap_original = photo;

    // Scaling if to big
    if (scene())
        fitToRect(scene()->sceneRect().toRect());
    else
        fitToRect(photo.rect());

    // Create effective pixmap
    this->refresh();

    this->setFlag(QGraphicsItem::ItemIsSelectable);
}

void PhotoItem::recalcShape()
{
    m_complete_path = m_image_path;
}
