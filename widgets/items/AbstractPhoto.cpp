#include "AbstractPhoto.h"
#include "Scene.h"
#include "photo_context_menu.h"
#include "PhotoEffectsGroup.h"
#include "BordersGroup.h"
#include "global.h"

#include <QMenu>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainterPath>
#include <QPolygonF>
#include <QPainter>
#include <QVariant>
#include <qmath.h>

#include <QDebug>
#include <QTime>

using namespace KIPIPhotoFramesEditor;

const QColor AbstractPhoto::SELECTED_ITEM_COLOR(255,0,0,20);

AbstractPhoto::AbstractPhoto() :
    AbstractItemInterface(0,0),
    m_name("New layer"),
    m_border_width(0)
{
    this->setupItem();

    // Effects group
    m_effects_group = new PhotoEffectsGroup(this);

    // Effects group
    m_borders_group = new BordersGroup(this);
    connect(this, SIGNAL(changed()), m_borders_group, SLOT(refresh()));
}

AbstractPhoto::~AbstractPhoto()
{
    m_effects_group->deleteLater();
}

void AbstractPhoto::setupItem()
{
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setFlag(QGraphicsItem::ItemIsMovable);
}

QDomElement AbstractPhoto::toSvg(QDomDocument & document) const
{
    QTransform transform = this->transform();
    qDebug() << transform;
    QString translate = "translate("+
                        QString::number(this->pos().x())+
                        ","+
                        QString::number(this->pos().y())+
                        ")";
    QString matrix = "matrix("+
                     QString::number(transform.m11())+
                     ","+
                     QString::number(transform.m12())+
                     ","+
                     QString::number(transform.m21())+
                     ","+
                     QString::number(transform.m22())+
                     ","+
                     QString::number(transform.m31())+
                     ","+
                     QString::number(transform.m32())+
                     ")";
    QDomElement itemSVG = document.createElement("g");
    itemSVG.setAttribute("transform", translate + " " + matrix);
    itemSVG.setAttribute("id", this->id());
    itemSVG.setAttribute("name", this->name());
    if (!this->isVisible())
        itemSVG.setAttribute("visibility", "hide");

    // 'defs' tag
    QDomElement defs = document.createElement("defs");
    itemSVG.appendChild(defs);

    // 'defs'->'clipPath'
    QDomElement clipPath = document.createElement("clipPath");
    clipPath.setAttribute("id", "clipPath_"+this->id());
    defs.appendChild(clipPath);

    // 'defs'->'pfe:data'
    QDomElement appNS = document.createElementNS(KIPIPhotoFramesEditor::uri(), "data");
    appNS.setPrefix(KIPIPhotoFramesEditor::name());
    defs.appendChild(appNS);
    appNS.appendChild(m_effects_group->toSvg(document));

    // 'defs'->'pfe:data'->'crop_path'
    QDomElement cropPath = document.createElement("crop_path");
    cropPath.appendChild( KIPIPhotoFramesEditor::pathToSvg(this->cropShape(), document) );
    appNS.appendChild(cropPath);

    // Convert visible area to SVG path's 'd' attribute
    QPainterPath visibleArea = this->shape();
    if (!visibleArea.isEmpty())
    {
        // 'defs'->'clipPath'->'path'
        QDomElement path = KIPIPhotoFramesEditor::pathToSvg(visibleArea, document);
        clipPath.appendChild(path);
    }

    QDomElement visibleData = document.createElement("g");
    visibleData.setAttribute("id", "data_" + this->id());
    defs.appendChild(visibleData);
    visibleData.appendChild(this->svgVisibleArea(document));
    visibleData.appendChild(this->m_borders_group->toSvg(document));

    // 'use'
    QDomElement use = document.createElement("use");
    use.setAttribute("xlink:href","#"+visibleData.attribute("id"));
    use.setAttribute("style","clip-path: url(#" + clipPath.attribute("id") + ");");
    itemSVG.appendChild(use);

    // 'g'
    QDomElement g2 = document.createElement("g");
    itemSVG.appendChild(g2);

    // 'g'->'use'
    QDomElement use3 = document.createElement("use");
    use3.setAttribute("xlink:href","#" + clipPath.attribute("id"));
    g2.appendChild(use3);

     /*
      * <g>
      *     <defs>
      *         <clipPath>      // clippingPath == m_image_path
      *             <path />
      *         </clipPath>
      *         <g>
      *             .........     // Children data
      *             .........     // Borders applied to the item
      *         </g>
      *         <pfe:data>
      *             .........     // Effects applied to the item
      *             .........     // Crop path
      *             .........     // Other data from the AbstractPhoto class.
      *         </pfe:data>
      *     </defs>
      *     <use />             // Cuts image
      *     <g>
      *         <use />         // Print cutted image
      *     </g>
      * </g>
      */

    return itemSVG;
}

bool AbstractPhoto::fromSvg(QDomElement & element)
{
    if (element.tagName() != "g")
        return false;

    if (element.attribute("visibility") == "hide")
        this->setVisible(false);

    // Position & transformation
    this->setPos(0,0);
    QString transform = element.attribute("transform");
    if (!transform.isEmpty())
    {
        QRegExp tr("translate\\([-0-9.]+,[-0-9.]+\\)");
        if (tr.indexIn(transform) >= 0)
        {
            QStringList list = tr.capturedTexts();
            QString translate = list.at(0);
            list = translate.split(',');
            QString x = list.at(0);
            QString y = list.at(1);
            this->setPos( x.right(x.length()-10).toDouble(),
                          y.left(y.length()-1).toDouble());
        }

        QRegExp rot("matrix\\([-0-9.]+,[-0-9.]+,[-0-9.]+,[-0-9.]+,[-0-9.]+,[-0-9.]+\\)");
        if (rot.indexIn(transform) >= 0)
        {
            QStringList list = rot.capturedTexts();
            QString matrix = list.at(0);
            matrix.remove(matrix.length()-1,1).remove(0,7);
            list = matrix.split(',');
            QString m11 = list.at(0);
            QString m12 = list.at(1);
            QString m21 = list.at(2);
            QString m22 = list.at(3);
            QString m31 = list.at(4);
            QString m32 = list.at(5);
            this->setTransform(QTransform(m11.toDouble(), m12.toDouble(), 0,
                                          m21.toDouble(), m22.toDouble(), 0,
                                          m31.toDouble(), m32.toDouble(), 1));
        }
    }

    // ID & name
    m_id = element.attribute("id");
    setName(element.attribute("name"));

    // Borders
    QDomNodeList children = element.childNodes();
    for (int i = children.count()-1; i >= 0; --i)
    {
        if (!children.at(i).isElement())
            continue;

        QDomElement itemDataElement = children.at(i).toElement();
        if (itemDataElement.attribute("id") != "data_"+m_id)
            continue;

        QDomElement cropPath = itemDataElement.firstChildElement("crop_path");
        if (!cropPath.isNull())
            this->setCropShape( KIPIPhotoFramesEditor::pathFromSvg( cropPath.firstChildElement("path") ) );

        // Borders
        if (m_borders_group)
            delete m_borders_group;
        m_borders_group = BordersGroup::fromSvg(itemDataElement, this);
        if (!m_borders_group)
            return false;
    }

    // Validation purpose
    QDomElement defs = element.firstChildElement("defs");
    if (defs.isNull())
        return false;
    QDomElement clipPath = defs.firstChildElement("clipPath");
    if (clipPath.isNull() || clipPath.attribute("id") != "clipPath_"+this->id())
        return false;

    // Other application specific data
    QDomElement appNS = defs.firstChildElement("data");
    if (appNS.isNull() || appNS.prefix() != KIPIPhotoFramesEditor::name())
        return false;

    // Effects
    if (m_effects_group)
        delete m_effects_group;
    m_effects_group = PhotoEffectsGroup::fromSvg(appNS, this);
    if (!m_effects_group)
        return false;

    return true;
}

void AbstractPhoto::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
    bordersGroup()->paint(painter, option);
}

void AbstractPhoto::dragEnterEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "dragEnterEvent";
    event->accept();
}

void AbstractPhoto::dragLeaveEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "dragLeaveEvent";
    event->accept();
}

void AbstractPhoto::dragMoveEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "dragMoveEvent";
    event->accept();
}

void AbstractPhoto::dropEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "dropEvent";
    event->accept();
}

void AbstractPhoto::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * /*event*/)
{
}

void AbstractPhoto::mouseMoveEvent(QGraphicsSceneMouseEvent * /*event*/)
{
}

void AbstractPhoto::mousePressEvent(QGraphicsSceneMouseEvent * /*event*/)
{
}

void AbstractPhoto::mouseReleaseEvent(QGraphicsSceneMouseEvent * /*event*/)
{
}

void AbstractPhoto::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    QGraphicsItem::hoverEnterEvent(event);
}

void AbstractPhoto::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    QGraphicsItem::hoverLeaveEvent(event);
    this->unsetCursor();
}

QString AbstractPhoto::id() const
{
    if (m_id.isEmpty())
        m_id = QString::number((long long)this, 16);
    return m_id;
}

void AbstractPhoto::setCropShape(const QPainterPath & cropShape)
{
    m_crop_shape = cropShape;
    qDebug () << "crop shape:" << m_crop_shape.boundingRect();
    this->refresh();
}

QPainterPath AbstractPhoto::cropShape() const
{
    return m_crop_shape;
}
