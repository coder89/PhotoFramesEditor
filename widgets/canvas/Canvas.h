#ifndef DOCUMENT_H
#define DOCUMENT_H

// Qt
#include <QObject>
#include <QSizeF>
#include <QRectF>
#include <QGraphicsScene>
#include <QItemSelection>
#include <QUndoStack>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QDomDocument>
#include <QFile>

// KDE
#include <klocalizedstring.h>
#include <kurl.h>

namespace KIPIPhotoFramesEditor
{
    class Scene;
    class LayersModel;
    class LayersSelectionModel;
    class AbstractPhoto;

    class Canvas : public QGraphicsView
    {
            Q_OBJECT

        public:

            enum SelectionMode
            {
                Viewing = 1,
                MultiSelecting = 2,
                SingleSelcting = 4,
            };

            enum InteractionMode
            {
                NoInteraction = 0,
                SingleElementEditingMode = 1,
                BorderToolMode = 3,
                ColorizeToolMode = 5,
            };

            explicit Canvas(const QSizeF & dimension, QWidget * parent = 0);
            explicit Canvas(QDomDocument & dimension, QWidget * parent = 0);
            virtual void wheelEvent(QWheelEvent *event);

            QDomDocument toSvg() const;

            Q_PROPERTY(KUrl m_file READ file WRITE setFile)
            KUrl file() const;
            void setFile(const KUrl & file);

            /// Saves canvas state to SVG format file
            QString save(const KUrl & file, bool setAsDefault = true);

            /// Check if canvas is saved
            bool isSaved();

            /// Set selection mode
            void setSelectionMode(SelectionMode mode);
            void setInteractionMode(InteractionMode mode);

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

            /// Sets border style for selected item
            void borderChangeCommand(qreal width, Qt::PenJoinStyle cornerStyle, const QColor & color);

            /// Conrtols saved-state of the canvas
            void isSavedChanged(int currentCommandIndex);
            void isSavedChanged(bool isStackClean);

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

            /// Sets selecting mode
            void enableDefaultSelectionMode()
            {
                setInteractionMode(NoInteraction);
                setSelectionMode(MultiSelecting);
            }

            /// Sets viewing mode
            void enableViewingMode()
            {
                setInteractionMode(NoInteraction);
                setSelectionMode(Viewing);
            }

            /// Sets border editing mode
            void enableBordersToolMode()
            {
                setInteractionMode(BorderToolMode);
            }

            /// Sets border editing mode
            void enableColorizeToolMode()
            {
                setInteractionMode(ColorizeToolMode);
            }

            /// Refresh widgets connections to canvas signals
            void refreshWidgetConnections(bool isVisible);

            /// Appends new undo command
            void newUndoCommand(QUndoCommand * command);

        signals:

            void hasSelectionChanged(bool hasSelection);
            void selectedItem(AbstractPhoto * photo);
            void setInitialValues(qreal width, Qt::PenJoinStyle cornersStyle, const QColor & color);
            void savedStateChanged();

        protected slots:

            /// Used for synchronization scene with model (adds to model item added directly from scene)
            void addNewItemToModel(AbstractPhoto * item);

        private:

            void setupGUI();
            void prepareSignalsConnection();
            void fromSvg(QDomDocument & document);

            KUrl m_file;
            bool m_is_saved;
            int m_saved_on_index;

            Scene * m_scene;
            LayersModel * m_model;
            LayersSelectionModel * m_selmodel;
            QUndoStack * m_undo_stack;

            SelectionMode m_selection_mode;
            InteractionMode m_interaction_mode;
    };
}

#endif // DOCUMENT_H
