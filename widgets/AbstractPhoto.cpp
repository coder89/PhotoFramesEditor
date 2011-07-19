#include "AbstractPhoto.h"
#include "Scene.h"
#include "photo_context_menu.h"
#include "PhotoEffectsGroup.h"

#include <QMenu>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainterPath>
#include <QPolygonF>
#include <QPainter>
#include <QVariant>
#include <qmath.h>
#include <QBuffer>

#include <QDebug>
#include <QTime>

using namespace KIPIPhotoFramesEditor;

const QColor AbstractPhoto::SELECTED_ITEM_COLOR(255,0,0,20);

AbstractPhoto::AbstractPhoto(QGraphicsScene * scene) :
    QGraphicsPixmapItem(0,scene),
    m_name("New layer"),
    m_pixmap_original(),
    m_border_width(0)
{
    //this->setAcceptDrops(true);
    this->setAcceptHoverEvents(true);

    // Effects group
    m_effects_group = new PhotoEffectsGroup(this);
}

AbstractPhoto::~AbstractPhoto()
{
    m_effects_group->deleteLater();
}

void AbstractPhoto::setupWidget(const QPainterPath & path)
{
    this->m_image_path = path.translated(-path.boundingRect().topLeft());
    this->m_image_path = m_image_path.simplified();
    m_pixmap_original = QPixmap(m_image_path.boundingRect().size().toSize());
    m_pixmap_original.fill(Qt::transparent);
    this->recalcShape();

    // Create effective pixmap
    this->refreshPixmap();

    this->setFlag(QGraphicsItem::ItemIsSelectable);

    // Default border style
    this->setBorderWidth(0);
    this->setBorderColor(Qt::black);
    this->setBorderCornersStyle(Qt::RoundJoin);
}

void AbstractPhoto::setupWidget(const QPixmap & photo)
{
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
    this->refreshPixmap();

    this->setFlag(QGraphicsItem::ItemIsSelectable);

    // Default border style
    this->setBorderWidth(0);
    this->setBorderColor(Qt::black);
    this->setBorderCornersStyle(Qt::RoundJoin);
}

QDomNode AbstractPhoto::toSvg(QDomDocument & document)
{
    const QString imageID = "img_" + this->name().simplified().replace(" ","_");
    const QString pathID = this->name().simplified().replace(" ","_");

    QDomElement result = document.createElement("g");
    result.setAttribute("transform","translate("+QString::number(this->pos().x())+","+QString::number(this->pos().y())+")");

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
        QString str_path_d;
        int elementsCount = m_image_path.elementCount();
        for (int i = 0; i < elementsCount; ++i)
        {
            QPainterPath::Element e = m_image_path.elementAt(i);
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
        path.setAttribute("d", str_path_d);
        clipPath.appendChild(path);
    }

    // 'defs'->'image'
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    m_pixmap.save(&buffer, "PNG");
    QDomElement img = document.createElement("image");
    img.setAttribute("width",m_pixmap.width());
    img.setAttribute("height",m_pixmap.height());
    img.setAttribute("xlink:href","data:image/png;base64,"+QString::fromLatin1(byteArray.toBase64().data()));
    img.setAttribute("id",imageID);
    defs.appendChild(img);

    // 'defs'->'use'
    QDomElement use = document.createElement("use");
    use.setAttribute("xlink:href","#"+imageID);
    use.setAttribute("style","clip-path: url(#" + pathID + ");");
    result.appendChild(use);

    // 'g'
    QDomElement g2 = document.createElement("g");
    result.appendChild(g2);

    // 'g'->'use'
    QDomElement use3 = document.createElement("use");
    use3.setAttribute("xlink:href","#"+pathID);
    g2.appendChild(use3);

     /*
      * <g>
      *     <defs>
      *         <clipPath>      // clippingPath == m_image_path
      *             <path />
      *         </clipPath>
      *         <image />       // Image for view in SVG
      *     </defs>
      *     <use />             // Cuts image
      *     <g>
      *         <use />         // Print cutted image
      *     </g>
      * </g>
      */

    return result;
}

AbstractPhoto * AbstractPhoto::fromSvg(QDomElement & element)
{
    if (element.tagName() == "g")
    {

    }
    return 0;
}

void AbstractPhoto::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem unselectedOption = (*option);
    unselectedOption.state &= !QStyle::State_Selected;
    QGraphicsPixmapItem::paint(painter,&unselectedOption,widget);

//    // Selecting item
//    if ((option->state) & QStyle::State_Selected)
//    {
//        QPainterPath exposedPath = QPainterPath();
//        exposedPath.addRect(option->exposedRect);
//        painter->fillPath(m_complete_path.intersected(exposedPath), SELECTED_ITEM_COLOR);
//    }

    if (this->borderWidth())
        painter->fillPath(m_border_path, this->borderColor());
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

void AbstractPhoto::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsPixmapItem::mouseMoveEvent(event);
    if (QGraphicsItem::flags() & ItemIsMovable)
    {
        if (event->modifiers() & Qt::ShiftModifier)
        {
            QPointF p;
            event->accept();
            p = event->scenePos() - event->buttonDownPos(Qt::LeftButton);
            p.setX(x_grid*round(p.rx()/x_grid));
            p.setY(y_grid*round(p.ry()/y_grid));
            this->setPos(p.rx(),p.ry());
        }
        else
            QGraphicsPixmapItem::mouseMoveEvent(event);
//            p = this->pos() + event->pos() - event->lastPos();
    }
}

void AbstractPhoto::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsPixmapItem::mousePressEvent(event);
    if (this->isSelected())
        this->setCursor(QCursor(Qt::ClosedHandCursor));
}

void AbstractPhoto::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsPixmapItem::mouseReleaseEvent(event);
    if (this->isSelected())
        this->setCursor(QCursor(Qt::OpenHandCursor));
}

void AbstractPhoto::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    QGraphicsPixmapItem::hoverEnterEvent(event);
    if (this->isSelected())
        this->setCursor(QCursor(Qt::OpenHandCursor));
}

void AbstractPhoto::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    QGraphicsPixmapItem::hoverLeaveEvent(event);
    this->unsetCursor();
}

void AbstractPhoto::updateIcon()
{
    m_icon = QIcon(m_pixmap.scaled(100,100,Qt::KeepAspectRatioByExpanding));
}

void AbstractPhoto::selectStateChanged(bool state)
{
    this->setSelected(state);
}

void AbstractPhoto::showStateChanged(bool state)
{
    this->setVisible(state);
}

void AbstractPhoto::lockStateChanged(bool state)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, state);
}

void AbstractPhoto::recalcShape()
{
    if (m_border_width)
    {
        QPainterPathStroker s;
        s.setWidth(m_border_width);
        s.setJoinStyle(m_border_corner_style);
        m_border_path = s.createStroke(m_image_path);
    }
    else
        m_border_path = QPainterPath();
    m_complete_path = m_border_path.united(m_image_path);
    prepareGeometryChange();

    try // I'm not sure how to properly/safely cast from QGraphicsScene* -> Scene*
    {
        Scene * m_scene = (Scene*)this->scene();
        if (m_scene)
            m_scene->updateSelection();
    }
    catch(...)
    {}
}

void AbstractPhoto::refreshPixmap()
{
    if (m_pixmap_original.isNull())
        return;
    this->m_pixmap = m_effects_group->apply(m_pixmap_original.scaled(this->boundingRect().size().toSize(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    this->updateIcon();
    QGraphicsPixmapItem::setPixmap(m_pixmap);
}
