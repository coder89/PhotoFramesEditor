#include "UndoRemoveItem.h"
#include "Scene.h"
#include "abstract_photo.h"
#include "LayersModel.h"

using namespace KIPIPhotoFramesEditor;

UndoRemoveItem::UndoRemoveItem(AbstractPhoto * item, Scene * scene, LayersModel * model, QUndoCommand * parent) :
    QUndoCommand("Remove item", parent),
    m_item(item),
    m_parentItem(static_cast<AbstractPhoto*>(item->QGraphicsItem::parentItem())),
    m_scene(scene),
    m_model(model)
{
}

void UndoRemoveItem::redo()
{
    m_parentIndex = m_model->findIndex(m_parentItem);
    m_itemIndex = m_model->findIndex(m_item,m_parentIndex);
    m_row = m_itemIndex.row();
    if (m_itemIndex.isValid())
        m_model->removeRow(m_row,m_parentIndex);
    m_scene->removeItem(m_item);
}

void UndoRemoveItem::undo()
{
    m_scene->addItem(m_item);
    m_item->setParent(m_parentItem);
    m_model->insertRow(m_row,m_parentIndex);
}
