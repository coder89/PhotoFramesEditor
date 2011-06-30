#ifndef ABSTRACT_PHOTO_H
#define ABSTRACT_PHOTO_H

// Qt
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneDragDropEvent>

// Local
#include "canvasmouseevent.h"
#include "LayersModelItem.h"

namespace KIPIPhotoFramesEditor
{
    class Scene;
    class WidgetsFactory;
    class LayersModelItem;

    class AbstractPhoto : public QGraphicsPixmapItem, public LayersModelItem
    {
        public:

            void setupWidget(const QPainterPath & path);

            virtual bool contains(const QPointF & point) const
            {
                return m_path.contains(point);
            }

            virtual QPainterPath shape() const
            {
                return m_path;
            }

            virtual QPainterPath opaqueArea() const
            {
                return m_path;
            }

            virtual QRectF boundingRect() const
            {
                return m_path.boundingRect();
            }

            static const QColor SELECTED_ITEM_COLOR;

            void setGridLines(qreal x, qreal y)
            {
                if (x && y)
                {
                    x_grid = x;
                    y_grid = y;
                }
            }

        protected:

            explicit AbstractPhoto(QGraphicsScene * parent = 0);

            // For widgets drawing
            static AbstractPhoto * getInstance() { return 0; }
            virtual void mouseDoubleClicked(const QPointF & point) = 0;
            virtual void mouseMoved(const QPointF & point) = 0;
            virtual void mousePressed(const QPointF & point) = 0;

            // Mouse events
            virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
            virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
            virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent * event);
            virtual void dragMoveEvent(QGraphicsSceneDragDropEvent * event);
            virtual void dropEvent(QGraphicsSceneDragDropEvent * event);
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
            virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
            virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

            // Updating items thumbnail
            void updateIcon();

            // Paining method
            virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

            // Pixmap data
            QPixmap m_pixmap;
            QPixmap & pixmap()
            {
                return m_pixmap;
            }
            const QPixmap & pixmap() const
            {
                return m_pixmap;
            }
            void setPixmap(const QPixmap &pixmap)
            {
                this->m_pixmap = pixmap;
                QGraphicsPixmapItem::setPixmap(this->m_pixmap);
            }

            // Widget path
            QPainterPath m_path;

            // Widgets popup menu
            class QContextMenu;
            static QContextMenu * menu;

            // Photo resizer class
            class AbstractPhotoResizer;
            friend class AbstractPhotoResizer;

            // Allows factory to use widgets protected drawing methods
            friend class WidgetsFactory;

        protected Q_SLOTS:

            // Model item synchronization
            virtual void selectStateChanged(bool state);
            virtual void showStateChanged(bool state);
            virtual void lockStateChanged(bool state);

        private:

            qreal x_grid;
            qreal y_grid;

            friend class Scene;
    };
}

#endif // ABSTRACT_PHOTO_H

/*  public:
      virtual void 	advance ( int phase )
     {qDebug() << "advance"; QGraphicsPixmapItem::advance(phase) ; }

      virtual QRectF 	boundingRect () const
     {qDebug() << "boundingRect"; return QGraphicsPixmapItem::boundingRect() ; }

      QRegion 	boundingRegion ( const QTransform & itemToDeviceTransform ) const
     {qDebug() << "boundingRegion"; return QGraphicsPixmapItem::boundingRegion(itemToDeviceTransform) ; }

      qreal 	boundingRegionGranularity () const
     {qDebug() << "boundingRegionGranularity"; return QGraphicsPixmapItem::boundingRegionGranularity() ; }

      QList<QGraphicsItem *> 	childItems () const
     {qDebug() << "childItems"; return QGraphicsPixmapItem::childItems() ; }

      QRectF 	childrenBoundingRect () const
     {qDebug() << "childrenBoundingRect"; return QGraphicsPixmapItem::childrenBoundingRect() ; }

      void 	clearFocus ()
     {qDebug() << "clearFocus"; QGraphicsPixmapItem::clearFocus() ; }

      QPainterPath 	clipPath () const
     {qDebug() << "clipPath"; return QGraphicsPixmapItem::clipPath() ; }

      virtual bool 	collidesWithItem ( const QGraphicsItem * other, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape ) const
     {qDebug() << "collidesWithItem"; return QGraphicsPixmapItem::collidesWithItem(other,mode) ; }

      virtual bool 	collidesWithPath ( const QPainterPath & path, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape ) const
     {qDebug() << "collidesWithPath"; return QGraphicsPixmapItem::collidesWithPath(path,mode) ; }

      QList<QGraphicsItem *> 	collidingItems ( Qt::ItemSelectionMode mode = Qt::IntersectsItemShape ) const
     {qDebug() << "collidingItems"; return QGraphicsPixmapItem::collidingItems(mode) ; }

      QGraphicsItem * 	commonAncestorItem ( const QGraphicsItem * other ) const
     {qDebug() << "commonAncestorItem"; return QGraphicsPixmapItem::commonAncestorItem(other) ; }

      virtual bool 	contains ( const QPointF & point ) const
     {qDebug() << "contains"; return QGraphicsPixmapItem::contains(point) ; }

      QVariant 	data ( int key ) const
     {qDebug() << "data"; return QGraphicsPixmapItem::data(key) ; }

      QTransform 	deviceTransform ( const QTransform & viewportTransform ) const
     {qDebug() << "deviceTransform"; return QGraphicsPixmapItem::deviceTransform(viewportTransform) ; }

      qreal 	effectiveOpacity () const
     {qDebug() << "effectiveOpacity"; return QGraphicsPixmapItem::effectiveOpacity() ; }

      void 	ensureVisible ( const QRectF & rect = QRectF(), int xmargin = 50, int ymargin = 50 )
     {qDebug() << "ensureVisible"; QGraphicsPixmapItem::ensureVisible(rect,xmargin,ymargin) ; }

      void 	ensureVisible ( qreal x, qreal y, qreal w, qreal h, int xmargin = 50, int ymargin = 50 )
     {qDebug() << "ensureVisible"; QGraphicsPixmapItem::ensureVisible(x,y,w,h,xmargin,ymargin) ; }

      bool 	filtersChildEvents () const
     {qDebug() << "filtersChildEvents"; return QGraphicsPixmapItem::filtersChildEvents() ; }

      GraphicsItemFlags 	flags () const
     {qDebug() << "flags"; return QGraphicsPixmapItem::flags() ; }

      QGraphicsItem * 	focusItem () const
     {qDebug() << "focusItem"; return QGraphicsPixmapItem::focusItem() ; }

      QGraphicsItem * 	focusProxy () const
     {qDebug() << "focusProxy"; return QGraphicsPixmapItem::focusProxy() ; }

      void 	grabKeyboard ()
     {qDebug() << "grabKeyboard"; QGraphicsPixmapItem::grabKeyboard() ; }

      void 	grabMouse ()
     {qDebug() << "grabMouse"; QGraphicsPixmapItem::grabMouse() ; }

      QGraphicsEffect * 	graphicsEffect () const
     {qDebug() << "graphicsEffect"; return QGraphicsPixmapItem::graphicsEffect() ; }

      QGraphicsItemGroup * 	group () const
     {qDebug() << "group"; return QGraphicsPixmapItem::group() ; }

      bool 	hasCursor () const
     {qDebug() << "hasCursor"; return QGraphicsPixmapItem::hasCursor() ; }

      bool 	hasFocus () const
     {qDebug() << "hasFocus"; return QGraphicsPixmapItem::hasFocus() ; }

      void 	hide ()
     {qDebug() << "hide"; QGraphicsPixmapItem::hide() ; }

      Qt::InputMethodHints 	inputMethodHints () const
     {qDebug() << "inputMethodHints"; return QGraphicsPixmapItem::inputMethodHints() ; }

      void 	installSceneEventFilter ( QGraphicsItem * filterItem )
     {qDebug() << "installSceneEventFilter"; QGraphicsPixmapItem::installSceneEventFilter(filterItem) ; }

      bool 	isActive () const
     {qDebug() << "isActive"; return QGraphicsPixmapItem::isActive() ; }

      bool 	isAncestorOf ( const QGraphicsItem * child ) const
     {qDebug() << "isAncestorOf"; return QGraphicsPixmapItem::isAncestorOf(child) ; }

      bool 	isBlockedByModalPanel ( QGraphicsItem ** blockingPanel = 0 ) const
     {qDebug() << "isBlockedByModalPanel"; return QGraphicsPixmapItem::isBlockedByModalPanel(blockingPanel) ; }

      bool 	isClipped () const
     {qDebug() << "isClipped"; return QGraphicsPixmapItem::isClipped() ; }

      bool 	isEnabled () const
     {qDebug() << "isEnabled"; return QGraphicsPixmapItem::isEnabled() ; }

      bool 	isObscured () const
     {qDebug() << "isObscured"; return QGraphicsPixmapItem::isObscured() ; }

      bool 	isObscured ( qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "isObscured"; return QGraphicsPixmapItem::isObscured(x,y,w,h) ; }

      bool 	isObscured ( const QRectF & rect ) const
     {qDebug() << "isObscured"; return QGraphicsPixmapItem::isObscured(rect) ; }

      virtual bool 	isObscuredBy ( const QGraphicsItem * item ) const
     {qDebug() << "isObscuredBy"; return QGraphicsPixmapItem::isObscuredBy(item) ; }

      bool 	isPanel () const
     {qDebug() << "isPanel"; return QGraphicsPixmapItem::isPanel() ; }

      bool 	isSelected () const
     {qDebug() << "isSelected"; return QGraphicsPixmapItem::isSelected() ; }

      bool 	isUnderMouse () const
     {qDebug() << "isUnderMouse"; return QGraphicsPixmapItem::isUnderMouse() ; }

      bool 	isVisible () const
     {qDebug() << "isVisible"; return QGraphicsPixmapItem::isVisible() ; }

      bool 	isVisibleTo ( const QGraphicsItem * parent ) const
     {qDebug() << "isVisibleTo"; return QGraphicsPixmapItem::isVisibleTo(parent) ; }

      bool 	isWidget () const
     {qDebug() << "isWidget"; return QGraphicsPixmapItem::isWidget() ; }

      bool 	isWindow () const
     {qDebug() << "isWindow"; return QGraphicsPixmapItem::isWindow() ; }

      QTransform 	itemTransform ( const QGraphicsItem * other, bool * ok = 0 ) const
     {qDebug() << "itemTransform"; return QGraphicsPixmapItem::itemTransform(other,ok) ; }

      QPointF 	mapFromItem ( const QGraphicsItem * item, const QPointF & point ) const
     {qDebug() << "mapFromItem"; return QGraphicsPixmapItem::mapFromItem(item,point) ; }

      QPolygonF 	mapFromItem ( const QGraphicsItem * item, const QRectF & rect ) const
     {qDebug() << "mapFromItem"; return QGraphicsPixmapItem::mapFromItem(item,rect) ; }

      QPolygonF 	mapFromItem ( const QGraphicsItem * item, const QPolygonF & polygon ) const
     {qDebug() << "mapFromItem"; return QGraphicsPixmapItem::mapFromItem(item,polygon) ; }

      QPainterPath 	mapFromItem ( const QGraphicsItem * item, const QPainterPath & path ) const
     {qDebug() << "mapFromItem"; return QGraphicsPixmapItem::mapFromItem(item,path) ; }

      QPolygonF 	mapFromItem ( const QGraphicsItem * item, qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "mapFromItem"; return QGraphicsPixmapItem::mapFromItem(item,x,y,w,h) ; }

      QPointF 	mapFromItem ( const QGraphicsItem * item, qreal x, qreal y ) const
     {qDebug() << "mapFromItem"; return QGraphicsPixmapItem::mapFromItem(item,x,y) ; }

      QPointF 	mapFromParent ( const QPointF & point ) const
     {qDebug() << "mapFromParent"; return QGraphicsPixmapItem::mapFromParent(point) ; }

      QPolygonF 	mapFromParent ( const QRectF & rect ) const
     {qDebug() << "mapFromParent"; return QGraphicsPixmapItem::mapFromParent(rect) ; }

      QPolygonF 	mapFromParent ( const QPolygonF & polygon ) const
     {qDebug() << "mapFromParent"; return QGraphicsPixmapItem::mapFromParent(polygon) ; }

      QPainterPath 	mapFromParent ( const QPainterPath & path ) const
     {qDebug() << "mapFromParent"; return QGraphicsPixmapItem::mapFromParent(path) ; }

      QPolygonF 	mapFromParent ( qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "mapFromParent"; return QGraphicsPixmapItem::mapFromParent(x,y,w,h) ; }

      QPointF 	mapFromParent ( qreal x, qreal y ) const
     {qDebug() << "mapFromParent"; return QGraphicsPixmapItem::mapFromParent(x,y) ; }

      QPointF 	mapFromScene ( const QPointF & point ) const
     {qDebug() << "mapFromScene"; return QGraphicsPixmapItem::mapFromScene(point) ; }

      QPolygonF 	mapFromScene ( const QRectF & rect ) const
     {qDebug() << "mapFromScene"; return QGraphicsPixmapItem::mapFromScene(rect) ; }

      QPolygonF 	mapFromScene ( const QPolygonF & polygon ) const
     {qDebug() << "mapFromScene"; return QGraphicsPixmapItem::mapFromScene(polygon) ; }

      QPainterPath 	mapFromScene ( const QPainterPath & path ) const
     {qDebug() << "mapFromScene"; return QGraphicsPixmapItem::mapFromScene(path) ; }

      QPolygonF 	mapFromScene ( qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "mapFromScene"; return QGraphicsPixmapItem::mapFromScene(x,y,w,h) ; }

      QPointF 	mapFromScene ( qreal x, qreal y ) const
     {qDebug() << "mapFromScene"; return QGraphicsPixmapItem::mapFromScene(x,y) ; }

      QRectF 	mapRectFromItem ( const QGraphicsItem * item, const QRectF & rect ) const
     {qDebug() << "mapRectFromItem"; return QGraphicsPixmapItem::mapRectFromItem(item,rect) ; }

      QRectF 	mapRectFromItem ( const QGraphicsItem * item, qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "mapRectFromItem"; return QGraphicsPixmapItem::mapRectFromItem(item,x,y,w,h) ; }

      QRectF 	mapRectFromParent ( const QRectF & rect ) const
     {qDebug() << "mapRectFromParent"; return QGraphicsPixmapItem::mapRectFromParent(rect) ; }

      QRectF 	mapRectFromParent ( qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "mapRectFromParent"; return QGraphicsPixmapItem::mapRectFromParent(x,y,w,h) ; }

      QRectF 	mapRectFromScene ( const QRectF & rect ) const
     {qDebug() << "mapRectFromScene"; return QGraphicsPixmapItem::mapRectFromScene(rect) ; }

      QRectF 	mapRectFromScene ( qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "mapRectFromScene"; return QGraphicsPixmapItem::mapRectFromScene(x,y,w,h) ; }

      QRectF 	mapRectToItem ( const QGraphicsItem * item, const QRectF & rect ) const
     {qDebug() << "mapRectToItem"; return QGraphicsPixmapItem::mapRectToItem(item,rect) ; }

      QRectF 	mapRectToItem ( const QGraphicsItem * item, qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "mapRectToItem"; return QGraphicsPixmapItem::mapRectToItem(item,x,y,w,h) ; }

      QRectF 	mapRectToParent ( const QRectF & rect ) const
     {qDebug() << "mapRectToParent"; return QGraphicsPixmapItem::mapRectToParent(rect) ; }

      QRectF 	mapRectToParent ( qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "mapRectToParent"; return QGraphicsPixmapItem::mapRectToParent(x,y,w,h) ; }

      QRectF 	mapRectToScene ( const QRectF & rect ) const
     {qDebug() << "mapRectToScene"; return QGraphicsPixmapItem::mapRectToScene(rect) ; }

      QRectF 	mapRectToScene ( qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "mapRectToScene"; return QGraphicsPixmapItem::mapRectToScene(x,y,w,h) ; }

      QPointF 	mapToItem ( const QGraphicsItem * item, const QPointF & point ) const
     {qDebug() << "mapToItem"; return QGraphicsPixmapItem::mapToItem(item,point) ; }

      QPolygonF 	mapToItem ( const QGraphicsItem * item, const QRectF & rect ) const
     {qDebug() << "mapToItem"; return QGraphicsPixmapItem::mapToItem(item,rect) ; }

      QPolygonF 	mapToItem ( const QGraphicsItem * item, const QPolygonF & polygon ) const
     {qDebug() << "mapToItem"; return QGraphicsPixmapItem::mapToItem(item,polygon) ; }

      QPainterPath 	mapToItem ( const QGraphicsItem * item, const QPainterPath & path ) const
     {qDebug() << "mapToItem"; return QGraphicsPixmapItem::mapToItem(item,path) ; }

      QPolygonF 	mapToItem ( const QGraphicsItem * item, qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "mapToItem"; return QGraphicsPixmapItem::mapToItem(item,x,y,w,h) ; }

      QPointF 	mapToItem ( const QGraphicsItem * item, qreal x, qreal y ) const
     {qDebug() << "mapToItem"; return QGraphicsPixmapItem::mapToItem(item,x,y) ; }

      QPointF 	mapToParent ( const QPointF & point ) const
     {qDebug() << "mapToParent"; return QGraphicsPixmapItem::mapToParent(point) ; }

      QPolygonF 	mapToParent ( const QRectF & rect ) const
     {qDebug() << "mapToParent"; return QGraphicsPixmapItem::mapToParent(rect) ; }

      QPolygonF 	mapToParent ( const QPolygonF & polygon ) const
     {qDebug() << "mapToParent"; return QGraphicsPixmapItem::mapToParent(polygon) ; }

      QPainterPath 	mapToParent ( const QPainterPath & path ) const
     {qDebug() << "mapToParent"; return QGraphicsPixmapItem::mapToParent(path) ; }

      QPolygonF 	mapToParent ( qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "mapToParent"; return QGraphicsPixmapItem::mapToParent(x,y,w,h) ; }

      QPointF 	mapToParent ( qreal x, qreal y ) const
     {qDebug() << "mapToParent"; return QGraphicsPixmapItem::mapToParent(x,y) ; }

      QPointF 	mapToScene ( const QPointF & point ) const
     {qDebug() << "mapToScene"; return QGraphicsPixmapItem::mapToScene(point) ; }

      QPolygonF 	mapToScene ( const QRectF & rect ) const
     {qDebug() << "mapToScene"; return QGraphicsPixmapItem::mapToScene(rect) ; }

      QPolygonF 	mapToScene ( const QPolygonF & polygon ) const
     {qDebug() << "mapToScene"; return QGraphicsPixmapItem::mapToScene(polygon) ; }

      QPainterPath 	mapToScene ( const QPainterPath & path ) const
     {qDebug() << "mapToScene"; return QGraphicsPixmapItem::mapToScene(path) ; }

      QPolygonF 	mapToScene ( qreal x, qreal y, qreal w, qreal h ) const
     {qDebug() << "mapToScene"; return QGraphicsPixmapItem::mapToScene(x,y,w,h) ; }

      QPointF 	mapToScene ( qreal x, qreal y ) const
     {qDebug() << "mapToScene"; return QGraphicsPixmapItem::mapToScene(x,y) ; }

      void 	moveBy ( qreal dx, qreal dy )
     {qDebug() << "moveBy"; QGraphicsPixmapItem::moveBy(dx,dy) ; }

      qreal 	opacity () const
     {qDebug() << "opacity"; return QGraphicsPixmapItem::opacity() ; }

      virtual QPainterPath 	opaqueArea () const
     {qDebug() << "opaqueArea"; return QGraphicsPixmapItem::opaqueArea() ; }

      virtual void 	paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 )
     {qDebug() << "paint"; QGraphicsPixmapItem::paint(painter,option,widget) ; }

      QGraphicsItem * 	panel () const
     {qDebug() << "panel"; return QGraphicsPixmapItem::panel() ; }

      PanelModality 	panelModality () const
     {qDebug() << "panelModality"; return QGraphicsPixmapItem::panelModality() ; }

      QGraphicsItem * 	parentItem () const
     {qDebug() << "parentItem"; return QGraphicsPixmapItem::parentItem() ; }

      QGraphicsObject * 	parentObject () const
     {qDebug() << "parentObject"; return QGraphicsPixmapItem::parentObject() ; }

      QGraphicsWidget * 	parentWidget () const
     {qDebug() << "parentWidget"; return QGraphicsPixmapItem::parentWidget() ; }

      QPointF 	pos () const
     {qDebug() << "pos"; return QGraphicsPixmapItem::pos() ; }

      void 	removeSceneEventFilter ( QGraphicsItem * filterItem )
     {qDebug() << "removeSceneEventFilter"; QGraphicsPixmapItem::removeSceneEventFilter(filterItem) ; }

      void 	resetTransform ()
     {qDebug() << "resetTransform"; QGraphicsPixmapItem::resetTransform() ; }

      qreal 	rotation () const
     {qDebug() << "rotation"; return QGraphicsPixmapItem::rotation() ; }

      qreal 	scale () const
     {qDebug() << "scale"; return QGraphicsPixmapItem::scale() ; }

      QGraphicsScene * 	scene () const
     {qDebug() << "scene"; return QGraphicsPixmapItem::scene() ; }

      QRectF 	sceneBoundingRect () const
     {qDebug() << "sceneBoundingRect"; return QGraphicsPixmapItem::sceneBoundingRect() ; }

      QPointF 	scenePos () const
     {qDebug() << "scenePos"; return QGraphicsPixmapItem::scenePos() ; }

      QTransform 	sceneTransform () const
     {qDebug() << "sceneTransform"; return QGraphicsPixmapItem::sceneTransform() ; }

      void 	scroll ( qreal dx, qreal dy, const QRectF & rect = QRectF() )
     {qDebug() << "scroll"; QGraphicsPixmapItem::scroll(dx,dy,rect) ; }

      void 	setAcceptDrops ( bool on )
     {qDebug() << "setAcceptDrops"; QGraphicsPixmapItem::setAcceptDrops(on) ; }

      void 	setAcceptHoverEvents ( bool enabled )
     {qDebug() << "setAcceptHoverEvents"; QGraphicsPixmapItem::setAcceptHoverEvents(enabled) ; }

      void 	setAcceptTouchEvents ( bool enabled )
     {qDebug() << "setAcceptTouchEvents"; QGraphicsPixmapItem::setAcceptTouchEvents(enabled) ; }

      void 	setAcceptedMouseButtons ( Qt::MouseButtons buttons )
     {qDebug() << "setAcceptedMouseButtons"; QGraphicsPixmapItem::setAcceptedMouseButtons(buttons) ; }

      void 	setActive ( bool active )
     {qDebug() << "setActive"; QGraphicsPixmapItem::setActive(active) ; }

      void 	setBoundingRegionGranularity ( qreal granularity )
     {qDebug() << "setBoundingRegionGranularity"; QGraphicsPixmapItem::setBoundingRegionGranularity(granularity) ; }

      void 	setCacheMode ( CacheMode mode, const QSize & logicalCacheSize = QSize() )
     {qDebug() << "setCacheMode"; QGraphicsPixmapItem::setCacheMode(mode,logicalCacheSize) ; }

      void 	setCursor ( const QCursor & cursor )
     {qDebug() << "setCursor"; QGraphicsPixmapItem::setCursor(cursor) ; }

      void 	setData ( int key, const QVariant & value )
     {qDebug() << "setData"; QGraphicsPixmapItem::setData(key,value) ; }

      void 	setEnabled ( bool enabled )
     {qDebug() << "setEnabled"; QGraphicsPixmapItem::setEnabled(enabled) ; }

      void 	setFiltersChildEvents ( bool enabled )
     {qDebug() << "setFiltersChildEvents"; QGraphicsPixmapItem::setFiltersChildEvents(enabled) ; }

      void 	setFlag ( GraphicsItemFlag flag, bool enabled = true )
     {qDebug() << "setFlag"; QGraphicsPixmapItem::setFlag(flag,enabled) ; }

      void 	setFlags ( GraphicsItemFlags flags )
     {qDebug() << "setFlags"; QGraphicsPixmapItem::setFlags(flags) ; }

      void 	setFocus ( Qt::FocusReason focusReason = Qt::OtherFocusReason )
     {qDebug() << "setFocus"; QGraphicsPixmapItem::setFocus(focusReason) ; }

      void 	setFocusProxy ( QGraphicsItem * item )
     {qDebug() << "setFocusProxy"; QGraphicsPixmapItem::setFocusProxy(item) ; }

      void 	setGraphicsEffect ( QGraphicsEffect * effect )
     {qDebug() << "setGraphicsEffect"; QGraphicsPixmapItem::setGraphicsEffect(effect) ; }

      void 	setGroup ( QGraphicsItemGroup * group )
     {qDebug() << "setGroup"; QGraphicsPixmapItem::setGroup(group) ; }

      void 	setInputMethodHints ( Qt::InputMethodHints hints )
     {qDebug() << "setInputMethodHints"; QGraphicsPixmapItem::setInputMethodHints(hints) ; }

      void 	setOpacity ( qreal opacity )
     {qDebug() << "setOpacity"; QGraphicsPixmapItem::setOpacity(opacity) ; }

      void 	setPanelModality ( PanelModality panelModality )
     {qDebug() << "setPanelModality"; QGraphicsPixmapItem::setPanelModality(panelModality) ; }

      void 	setParentItem ( QGraphicsItem * newParent )
     {qDebug() << "setParentItem"; QGraphicsPixmapItem::setParentItem(newParent) ; }

      void 	setPos ( const QPointF & pos )
     {qDebug() << "setPos"; QGraphicsPixmapItem::setPos(pos) ; }

      void 	setPos ( qreal x, qreal y )
     {qDebug() << "setPos"; QGraphicsPixmapItem::setPos(x,y) ; }

      void 	setRotation ( qreal angle )
     {qDebug() << "setRotation"; QGraphicsPixmapItem::setRotation(angle) ; }

      void 	setScale ( qreal factor )
     {qDebug() << "setScale"; QGraphicsPixmapItem::setScale(factor) ; }

      void 	setSelected ( bool selected )
     {qDebug() << "setSelected"; QGraphicsPixmapItem::setSelected(selected) ; }

      void 	setToolTip ( const QString & toolTip )
     {qDebug() << "setToolTip"; QGraphicsPixmapItem::setToolTip(toolTip) ; }

      void 	setTransform ( const QTransform & matrix, bool combine = false )
     {qDebug() << "setTransform"; QGraphicsPixmapItem::setTransform(matrix,combine) ; }

      void 	setTransformOriginPoint ( const QPointF & origin )
     {qDebug() << "setTransformOriginPoint"; QGraphicsPixmapItem::setTransformOriginPoint(origin) ; }

      void 	setTransformOriginPoint ( qreal x, qreal y )
     {qDebug() << "setTransformOriginPoint"; QGraphicsPixmapItem::setTransformOriginPoint(x,y) ; }

      void 	setTransformations ( const QList<QGraphicsTransform *> & transformations )
     {qDebug() << "setTransformations"; QGraphicsPixmapItem::setTransformations(transformations) ; }

      void 	setVisible ( bool visible )
     {qDebug() << "setVisible"; QGraphicsPixmapItem::setVisible(visible) ; }

      void 	setX ( qreal x )
     {qDebug() << "setX"; QGraphicsPixmapItem::setX(x) ; }

      void 	setY ( qreal y )
     {qDebug() << "setY"; QGraphicsPixmapItem::setY(y) ; }

      void 	setZValue ( qreal z )
     {qDebug() << "setZValue"; QGraphicsPixmapItem::setZValue(z) ; }

      virtual QPainterPath 	shape () const
     {qDebug() << "shape"; return QGraphicsPixmapItem::shape() ; }

      void 	show ()
     {qDebug() << "show"; QGraphicsPixmapItem::show() ; }

      void 	stackBefore ( const QGraphicsItem * sibling )
     {qDebug() << "stackBefore"; QGraphicsPixmapItem::stackBefore(sibling) ; }

      QGraphicsObject * 	toGraphicsObject ()
     {qDebug() << "toGraphicsObject"; return QGraphicsPixmapItem::toGraphicsObject() ; }

      const QGraphicsObject * 	toGraphicsObject () const
     {qDebug() << "toGraphicsObject"; return QGraphicsPixmapItem::toGraphicsObject() ; }

      QString 	toolTip () const
     {qDebug() << "toolTip"; return QGraphicsPixmapItem::toolTip() ; }

      QGraphicsItem * 	topLevelItem () const
     {qDebug() << "topLevelItem"; return QGraphicsPixmapItem::topLevelItem() ; }

      QGraphicsWidget * 	topLevelWidget () const
     {qDebug() << "topLevelWidget"; return QGraphicsPixmapItem::topLevelWidget() ; }

      QTransform 	transform () const
     {qDebug() << "transform"; return QGraphicsPixmapItem::transform() ; }

      QPointF 	transformOriginPoint () const
     {qDebug() << "transformOriginPoint"; return QGraphicsPixmapItem::transformOriginPoint() ; }

      QList<QGraphicsTransform *> 	transformations () const
     {qDebug() << "transformations"; return QGraphicsPixmapItem::transformations() ; }

      virtual int 	type () const
     {qDebug() << "type"; return QGraphicsPixmapItem::type() ; }

      void 	ungrabKeyboard ()
     {qDebug() << "ungrabKeyboard"; QGraphicsPixmapItem::ungrabKeyboard() ; }

      void 	ungrabMouse ()
     {qDebug() << "ungrabMouse"; QGraphicsPixmapItem::ungrabMouse() ; }

      void 	unsetCursor ()
     {qDebug() << "unsetCursor"; QGraphicsPixmapItem::unsetCursor() ; }

      void 	update ( const QRectF & rect = QRectF() )
     {qDebug() << "update"; QGraphicsPixmapItem::update(rect) ; }

      void 	update ( qreal x, qreal y, qreal width, qreal height )
     {qDebug() << "update"; QGraphicsPixmapItem::update(x,y,width,height) ; }

      QGraphicsWidget * 	window () const
     {qDebug() << "window"; return QGraphicsPixmapItem::window() ; }

      qreal 	x () const
     {qDebug() << "x"; return QGraphicsPixmapItem::x() ; }

      qreal 	y () const
     {qDebug() << "y"; return QGraphicsPixmapItem::y() ; }

      qreal 	zValue () const
     {qDebug() << "zValue"; return QGraphicsPixmapItem::zValue() ; }


*/
