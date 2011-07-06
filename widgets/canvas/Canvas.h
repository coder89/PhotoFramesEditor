#ifndef DOCUMENT_H
#define DOCUMENT_H

// Qt
#include <QObject>
#include <QSizeF>
#include <QRectF>
#include <QGraphicsScene>
#include <QItemSelection>
#include <QUndoStack>

// KDE
#include <klocalizedstring.h>

namespace KIPIPhotoFramesEditor
{
    class Scene;
    class LayersModel;
    class LayersSelectionModel;
    class AbstractPhoto;

    class Canvas : public QObject
    {
            Q_OBJECT

        public:

            explicit Canvas(const QSizeF & dimension, QObject * parent = 0);

            Scene * scene() const
            {
                return m_scene;
            }

            LayersModel * model() const
            {
                return m_model;
            }

            LayersSelectionModel * selectionModel() const
            {
                return m_selmodel;
            }

            QUndoStack * undoStack() const
            {
                return m_undo_stack;
            }

            operator Scene*()
            {
                return m_scene;
            }

            operator LayersModel*()
            {
                return m_model;
            }

            operator QUndoStack*()
            {
                return m_undo_stack;
            }

            operator LayersSelectionModel*()
            {
                return m_selmodel;
            }

        public slots:

            void addImage(const QImage & image);
            void addItemToModel(AbstractPhoto * item);

            /// Creates move rows command and pushes it onto the stack
            void moveRowsCommand(const QModelIndex & startIndex, int count, const QModelIndex & parentIndex, int move, const QModelIndex & destinationParent);

            /// Move selected items up on scene & model. (Called by layers tree)
            void moveSelectedRowsUp();

            /// Move selected items down on scene & model. (Called by layers tree)
            void moveSelectedRowsDown();

            /// Create undo/redo command
            void removeComand(AbstractPhoto * item);

            /// Remove item selected on scene (remove from scene & model => calls removeComand())
            void removeItem(AbstractPhoto * item);

            /// Remove items selected on scene (remove from scene & model => calls removeComand())
            void removeItems(const QList<AbstractPhoto*> & items);

            /// Remove items selected on model (remove from model & scene => calls removeComand())
            void removeSelectedRows();

            /// Shows confirm dialog
            bool askAboutRemoving(int count);

            /// Select items on model (synchronize model with scene)
            void selectionChanged();

            /// Select items on scene (synchronize scene with model)
            void selectionChanged(const QItemSelection & newSelection, const QItemSelection & oldSelection);

            /// Groups operations into one undo operation
            void beginRowsRemoving()
            {
                m_undo_stack->beginMacro(i18n("Remove items"));
            }

            /// Finish group of undo operations
            void endRowsRemoving()
            {
                m_undo_stack->endMacro();
            }

        private:

            Scene * m_scene;
            LayersModel * m_model;
            LayersSelectionModel * m_selmodel;
            QGraphicsItemGroup * m_items_group;
            QUndoStack * m_undo_stack;
    };
}

#endif // DOCUMENT_H
