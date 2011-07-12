// Local
#include "CanvasWidget.h"
#include "polygon_widget.h"
#include "canvasmouseevent.h"
#include "AbstractPhoto.h"

// Qt
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QVariant>
#include <QImage>
#include <QUrl>
#include <QPixmap>
#include <QRectF>
#include <QRect>
#include <QPainterPath>
#include <QApplication>

// Local
#include "Scene.h"
#include "Canvas.h"
#include "LayersModel.h"
#include "LayersSelectionModel.h"

#define MAX_SCALE_LIMIT 4
#define MIN_SCALE_LIMIT 0.5

using namespace KIPIPhotoFramesEditor;

CanvasWidget::CanvasWidget(QWidget *parent) :
    QGraphicsView(parent),
    m_model(new QStandardItemModel(this))
{
    setupGUI();
}

void CanvasWidget::setupGUI()
{
    this->setAcceptDrops(true);
    this->setAutoFillBackground(true);
    this->viewport()->setAutoFillBackground(false);
    this->setCacheMode(QGraphicsView::CacheBackground);
    this->setBackgroundBrush(QPalette().brush(QPalette::Window));
    /*this->setRenderHint(QPainter::Antialiasing);*/                                /// It causes worst quality!
    /*this->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);*/  /// It causes worst quality!
}

void CanvasWidget::setGridVisible(bool visible)
{
    emit toggleGrid(visible);
}

//    void CanvasWidget::newCanvas(const QRectF & dimension)
//    {
//        closeCanvas();
//        m_canvas = new Canvas(dimension, this);
//        this->setScene(m_canvas);
//        connect(m_canvas,SIGNAL(newItemAdded(AbstractPhoto*)),this,SLOT(addItemToModel(AbstractPhoto*)));
//        connect(m_canvas,SIGNAL(selectionChanged()),this,SLOT(updateSelection()));
//        m_canvas->newItem();
//    }

//    void CanvasWidget::updateSelection()
//    {
//        QList<QGraphicsItem*> temp = m_canvas->selectedItems();
//        QSet<AbstractPhoto*> selected = currentSelectedList;
//        QSet<AbstractPhoto*> deselected = currentSelectedList;
//        foreach(QGraphicsItem * item, temp)
//        {
//            selected.insert(static_cast<AbstractPhoto*>(item));
//            deselected.remove(static_cast<AbstractPhoto*>(item));
//        }
//        foreach(AbstractPhoto * item, currentSelectedList)
//            selected.remove(item);
//        foreach(AbstractPhoto * item, selected)
//            currentSelectedList.insert(item);
//        emit selectionChanged(selected.toList(), deselected.toList());
//    }

void CanvasWidget::dragEnterEvent(QDragEnterEvent * event)
{
    //event->acceptProposedAction();
    //event->setAccepted(true);
    //qDebug() << "dragEnterEvent";
    QGraphicsView::dragEnterEvent(event);
    //        const QMimeData * mime = event->mimeData();
    //        if (mime->hasUrls() || mime->hasImage() || mime->hasColor())
    //        QImage img("/home/coder89/Desktop/routing.jpg");
    //        QSize s = img.size();
    //        QRect r = img.rect();
    //        s.scale(400, s.height()*400/s.width(), Qt::KeepAspectRatio);
    //        r.setSize(s);
    //        QPixmap p(s);
    //        QPainter paint(&p);
    //        paint.drawImage(r, img);
    //        QPainterPath asf;
    //        asf.addRect(QRectF(r));
    //        QGraphicsPixmapItem * it = new PolygonWidget(asf,0,scene());
    //        //it->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    //        it->setPixmap(p);
    //       // scene()->addItem(it);
}

void CanvasWidget::keyPressEvent(QKeyEvent * event)
{
    QGraphicsView::keyPressEvent(event);
    //if (event->modifiers() & Qt::ControlModifier)
    //    this->setDragMode(QGraphicsView::ScrollHandDrag);
}

void CanvasWidget::keyReleaseEvent(QKeyEvent * event)
{
    QGraphicsView::keyReleaseEvent(event);
    //if (event->modifiers() == 0)
    //    this->setDragMode(QGraphicsView::NoDrag);
}

void CanvasWidget::wheelEvent(QWheelEvent * event)
{
    // Scaling limitation
    static double factor = 1;
    if ( !scene() || (factor > MAX_SCALE_LIMIT && event->delta() > 0) || (factor < MIN_SCALE_LIMIT && event->delta() < 0) )
        return;
    //this->sceneRect().width() / mapToScene(this->sceneRect().toRect()).boundingRect().width();

    double scaleFactor;
    if(event->delta() > 0)
        scaleFactor = (factor + 0.1) / factor;
    else
        scaleFactor = (factor - 0.1) / factor;

//    double scaleFactor = 1.05;

//    if (event->modifiers() & Qt::ControlModifier)
//        scaleFactor += 0.3;

//    if(event->delta() < 0)
//        scaleFactor = 1.0 / scaleFactor;

    scale(scaleFactor, scaleFactor);

    centerOn( mapToScene(event->pos()) );

    factor *= scaleFactor;
}

QAbstractItemModel * CanvasWidget::model()
{
    return this->m_canvas->model();
}

//#####################################################################################################

void CanvasWidget::addItemToModel(AbstractPhoto * item)
{
//    QStandardItem * parent = 0;
//    if (item->parentItem())
//    {
//        QModelIndex index = m_model->indexFromItem(item);
//        if (index.isValid())
//        {
//            parent = m_model->itemFromIndex(index);
//            parent->appendRow(item);
//        }
//        else
//            qDebug() << "Indavlid parent item!";
//    }
//    else
//    {
//        item->setText(QString("Item %0").arg(item->zValue()));
//        m_model->appendRow(item);
//    }
}

//#####################################################################################################

void CanvasWidget::setCanvas(Canvas * canvas)
{
    if (canvas == 0)
        return;
    this->setScene(canvas->scene());
    m_canvas = canvas;
    connect(canvas, SIGNAL(destroyed()), this, SLOT(documentClosed()));
    connect(this, SIGNAL(toggleGrid(bool)), canvas->scene(), SLOT(setGridVisible(bool)));

    QImage img("/home/coder89/Desktop/routing.jpg");        /// TODO : Remove after finish
    img.setText("File","routing.jpg");
    m_canvas->addImage(img);                                /// TODO : Remove after finish
    m_canvas->addImage(img);                                /// TODO : Remove after finish
    m_canvas->addImage(img);                                /// TODO : Remove after finish
    m_canvas->addImage(img);                                /// TODO : Remove after finish
    m_canvas->addImage(img);                                /// TODO : Remove after finish
    m_canvas->addImage(img);                                /// TODO : Remove after finish
    m_canvas->addImage(img);                                /// TODO : Remove after finish
    m_canvas->addImage(img);                                /// TODO : Remove after finish
    m_canvas->addImage(img);                                /// TODO : Remove after finish
}

//#####################################################################################################

void CanvasWidget::documentClosed()
{
    m_canvas = 0;
}

//#####################################################################################################

#undef MAX_SCALE_LIMIT
#undef MIN_SCALE_LIMIT
