#include "UndoRemoveItem.h"
#include "Scene.h"
#include "abstract_photo.h"
#include "LayersModel.h"
#include "LayersModelItem.h"

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
//    m_parentIndex = m_model->findIndex(m_parentItem);
//    qDebug() << m_parentIndex.isValid();
//    m_itemIndex = m_model->findIndex(m_item, m_parentIndex);
//    qDebug() << m_itemIndex.isValid();
//    m_row = m_itemIndex.row();
//    if (m_itemIndex.isValid())
//        m_model->removeRow(0,m_parentIndex);
//    //m_scene->removeItem(m_item);
//    qDebug() << "redo";
}

void UndoRemoveItem::undo()
{
//    qDebug() << "undo";
//    //m_parentIndex = m_model->findIndex(m_parentItem);

//   // qDebug() << m_parentIndex.isValid();
//   // m_scene->addItem(m_item);
//    //m_item->setParentItem(m_parentItem);
//    m_model->prependItem(m_item);
//    //if (m_model->insertRows(m_row,10,m_parentIndex))
//    //    static_cast<LayersModelItem*>(m_model->index(m_row,0,m_parentIndex).internalPointer())->setPhoto(m_item);
//    qDebug() << m_model->rowCount();
}
