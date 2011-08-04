#ifndef ABSTRACTLISTTOOL_H
#define ABSTRACTLISTTOOL_H

#include "AbstractTool.h"
#include "AbstractMovableModel.h"

namespace KIPIPhotoFramesEditor
{
    class AbstractListToolPrivate;

    class AbstractListToolView;
    class AbstractListToolViewDelegate;

    class AbstractListTool : public AbstractTool
    {
            Q_OBJECT

        public:
            explicit AbstractListTool(const QString & toolsName, ToolsDockWidget * parent = 0);
            ~AbstractListTool();
            virtual void currentItemAboutToBeChanged();
            virtual void currentItemChanged();
            virtual void positionAboutToBeChanged(){} // Unused
            virtual void positionChanged(){} // Unused
            virtual QStringList options() const = 0;
            virtual QObject * createItem(const QString & name) = 0;

        protected slots:

            virtual AbstractMovableModel * model() = 0;
            void viewCurrentEditor(const QModelIndex & index);
            virtual QWidget * createEditor(QObject * item, bool createCommands = true) = 0;
            void createChooser();
            void itemSelected(const QString & selectedItem);
            virtual void addItemCommand(QObject * item, int row);
            void chooserAccepted();
            void chooserCancelled();
            void removeSelected();
            void moveSelectedDown();
            void moveSelectedUp();

        private:
            void closeEditor();
            AbstractListToolPrivate * d;
    };
}

#endif // ABSTRACTLISTTOOL_H
