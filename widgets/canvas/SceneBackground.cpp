#include "SceneBackground.h"
#include "global.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QUndoCommand>
#include <QStyleOptionGraphicsItem>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::SceneBackground::BackgroundChangedCommand : public QUndoCommand
{
    QBrush m_new_brush;
    SceneBackground * m_backgropund_item;
public:
    BackgroundChangedCommand(const QBrush & newBrush, SceneBackground * backgroundItem, QUndoCommand * parent = 0) :
        QUndoCommand(i18n("Background changed"), parent),
        m_new_brush(newBrush),
        m_backgropund_item(backgroundItem)
    {}
    virtual void redo()
    {
        run();
    }
    virtual void undo()
    {
        run();
    }
    void run()
    {
        QBrush temp = m_backgropund_item->m_brush;
        m_backgropund_item->m_brush = m_new_brush;
        m_new_brush = temp;
        m_backgropund_item->update();
    }
};

SceneBackground::SceneBackground(QGraphicsScene * scene) :
    QGraphicsItem(0, scene),
    m_brush(Qt::white)
{
    this->setZValue(-1.0/0.0);
    this->setFlags(0);
    this->sceneChanged();
}

QRectF SceneBackground::boundingRect() const
{
    return m_rect;
}

void SceneBackground::setBrush(const QBrush &backgroundBrush)
{
    BackgroundChangedCommand * command = new BackgroundChangedCommand(backgroundBrush, this);
    PFE_PostUndoCommand(command);
}

QVariant SceneBackground::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch(change)
    {
        case QGraphicsItem::ItemParentChange:
            return QVariant(0);
        case QGraphicsItem::ItemSceneHasChanged:
            sceneChanged();
        default:
            return QGraphicsItem::itemChange(change, value);
    }
}

void SceneBackground::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
    if (!m_rect.isValid())
        return;
    painter->fillRect(m_rect.intersected(option->rect), m_brush);
}

void SceneBackground::sceneChanged()
{
    if (scene())
        m_rect = scene()->sceneRect();
    else
        m_rect = QRectF();
}
