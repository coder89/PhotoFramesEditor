#include "AbstractPhoto.h"
#include "Scene.h"
#include "photo_context_menu.h"
#include "PhotoEffectsGroup.h"
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

AbstractPhoto::AbstractPhoto(QGraphicsScene * scene) :
    QGraphicsItem(0,scene),
    m_name("New layer"),
    m_border_width(0)
{
    this->setupItem();

    // Effects group
    m_effects_group = new PhotoEffectsGroup(this);
}

AbstractPhoto::~AbstractPhoto()
{
    m_effects_group->deleteLater();
}

//void AbstractPhoto::setupWidget(const QPainterPath & path)
//{
//    //this->setAcceptDrops(true);
//    this->setAcceptHoverEvents(true);

//    this->m_image_path = path.translated(-path.boundingRect().topLeft());
//    this->m_image_path = m_image_path.simplified();
//    //m_pixmap_original = QPixmap(m_image_path.boundingRect().size().toSize());
//    //m_pixmap_original.fill(Qt::transparent);
//    //this->recalcShape();

//    // Create effective pixmap
//    //this->refreshPixmap();

//    this->setFlag(QGraphicsItem::ItemIsSelectable);

//    // Default border style
//    this->setBorderWidth(0);
//    this->setBorderColor(Qt::black);
//    this->setBorderCornersStyle(Qt::RoundJoin);
//}

void AbstractPhoto::setupItem()
{
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setFlag(QGraphicsItem::ItemIsMovable);

    // Default border style
    this->setBorderWidth(0);
    this->setBorderColor(Qt::black);
    this->setBorderCornersStyle(Qt::RoundJoin);
}

QDomElement AbstractPhoto::toSvg(QDomDocument & document) const
{
    QString translate = "translate("+
                        QString::number(this->pos().x())+
                        ","+
                        QString::number(this->pos().y())+
                        ")";
    QTransform transform = this->transform();
    QString matrix = "matrix("+
                     QString::number(transform.m11())+
                     ","+
                     QString::number(transform.m21())+
                     ","+
                     QString::number(transform.m12())+
                     ","+
                     QString::number(transform.m22())+
                     ","+
                     QString::number(transform.m13())+
                     ","+
                     QString::number(transform.m23())+
                     ")";
    QDomElement result = document.createElement("g");
    result.setAttribute("transform", translate + " " + matrix);
    result.setAttribute("id", this->id());
    result.setAttribute("name", this->name());

    // 'defs' tag
    QDomElement defs = document.createElement("defs");
    result.appendChild(defs);

    // 'defs'->'clipPath'
    QDomElement clipPath = document.createElement("clipPath");
    clipPath.setAttribute("id", "clipPath_"+this->id());
    defs.appendChild(clipPath);

    // 'defs'->'pfe:data'
    QDomElement appNS = document.createElementNS(KIPIPhotoFramesEditor::uri(), "data");
    appNS.setPrefix(KIPIPhotoFramesEditor::name());
    defs.appendChild(appNS);
    appNS.appendChild(m_effects_group->toSvg(document));

    // Convert visible area to SVG path's 'd' attribute
    QPainterPath visibleArea = this->opaqueArea();
    if (!visibleArea.isEmpty())
    {
        // 'defs'->'clipPath'->'path'
        QDomElement path = KIPIPhotoFramesEditor::pathToSvg(visibleArea, document);
        clipPath.appendChild(path);
    }

    QDomElement visibleData = this->svgVisibleArea(document);
    visibleData.setAttribute("id", "data_" + this->id());
    defs.appendChild(visibleData);

    // 'defs'->'use'
    QDomElement use = document.createElement("use");
    use.setAttribute("xlink:href","#"+visibleData.attribute("id"));
    use.setAttribute("style","clip-path: url(#" + clipPath.attribute("id") + ");");
    result.appendChild(use);

    // 'g'
    QDomElement g2 = document.createElement("g");
    result.appendChild(g2);

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
      *           .........     // Children data
      *         <pfe:data>
      *           .........     // Effects applied to the item
      *           .........     // Borders applied to the item
      *           .........     // Other data from the AbstractPhoto class.
      *         </pfe:data>
      *     </defs>
      *     <use />             // Cuts image
      *     <g>
      *         <use />         // Print cutted image
      *     </g>
      * </g>
      */

    return result;
}

bool AbstractPhoto::fromSvg(QDomElement & element)
{
    if (element.tagName() != "g")
        return false;

    // Position & transformation
    this->setPos(0,0);
    QString transform = element.attribute("transform");
    if (!transform.isEmpty())
    {
        QRegExp tr("translate\\([0-9.]+,[0-9.]+\\)");
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
        QRegExp rot("matrix\\([0-9.]+,[0-9.]+,[0-9.]+,[0-9.]+,[0-9.]+,[0-9.]+\\)");
        if (rot.indexIn(transform) >= 0)
        {
            QStringList list = rot.capturedTexts();
            QString matrix = list.at(0);
            matrix.remove(matrix.length()-1,1).remove(0,7);
            list = matrix.split(',');
            QString a = list.at(0);
            QString b = list.at(1);
            QString c = list.at(2);
            QString d = list.at(3);
            QString e = list.at(4);
            QString f = list.at(5);
            this->setTransform(QTransform(a.toDouble(), c.toDouble(), e.toDouble(),
                                          b.toDouble(), d.toDouble(), f.toDouble(),
                                          0, 0, 1));
        }
    }

    // ID & name
    m_id = element.attribute("id");
    setName(element.attribute("name"));

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
    if (m_effects_group)
        delete m_effects_group;
    m_effects_group = PhotoEffectsGroup::fromSvg(appNS);
    if (!m_effects_group)
        return false;
    else
        m_effects_group->setPhoto(this);

    return true;
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

void AbstractPhoto::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
//    QGraphicsItem::mouseMoveEvent(event);
//    if (QGraphicsItem::flags() & ItemIsMovable)
//    {
//        if (event->modifiers() & Qt::ShiftModifier)
//        {
//            QPointF p;
//            event->accept();
//            p = event->scenePos() - event->buttonDownPos(Qt::LeftButton);
//            p.setX(x_grid*round(p.rx()/x_grid));
//            p.setY(y_grid*round(p.ry()/y_grid));
//            this->setPos(p.rx(),p.ry());
//        }
//        else
//            QGraphicsItem::mouseMoveEvent(event);
////            p = this->pos() + event->pos() - event->lastPos();
//    }
}

void AbstractPhoto::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
}

void AbstractPhoto::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
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

void AbstractPhoto::recalcShape()
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
}
