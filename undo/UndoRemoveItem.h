#ifndef UNDOREMOVEITEM_H
#define UNDOREMOVEITEM_H

#include <QUndoCommand>
#include <QModelIndex>

namespace KIPIPhotoFramesEditor
{
    class Scene;
    class LayersModel;
    class AbstractPhoto;

    class UndoRemoveItem : public QUndoCommand
    {
            AbstractPhoto * m_item;
            AbstractPhoto * m_parentItem;
            Scene * m_scene;
            LayersModel * m_model;
            QModelIndex m_parentIndex;
            QModelIndex m_itemIndex;
            int m_row;

        public:

            UndoRemoveItem(AbstractPhoto * item, Scene * scene, LayersModel * model, QUndoCommand * parent = 0);
            virtual void redo();
            virtual void undo();
    };
}

#endif // UNDOREMOVEITEM_H
