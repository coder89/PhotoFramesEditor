#ifndef EFFECTSEDITORTOOL_H
#define EFFECTSEDITORTOOL_H

#include "AbstractTool.h"

#include <QListView>
#include <QModelIndex>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhotoEffectInterface;
    class EffectsEditorToolPrivate;

    class EffectsEditorTool : public AbstractTool
    {
            Q_OBJECT

        public:
            explicit EffectsEditorTool(QWidget * parent = 0);
            virtual void currentItemChanged();
        protected slots:
            void viewCurrentEffectEditor(const QModelIndex & index);
            void removeSelected();
            void moveSelectedDown();
            void moveSelectedUp();
        private:
            void removeCurrentPropertyBrowser();
            EffectsEditorToolPrivate * d;
    };

    class EffectsListView : public QListView
    {
            Q_OBJECT
        public:
            EffectsListView(QWidget * parent = 0) : QListView(parent)
            {
                this->setSelectionMode(QAbstractItemView::SingleSelection);
            }
            QModelIndex selectedIndex() const
            {
                QModelIndexList indexes = selectedIndexes();
                if (indexes.count() == 1)
                    return indexes.at(0);
                return QModelIndex();
            }
        signals:
            void selectedIndex(const QModelIndex & index);
        protected:
            virtual void selectionChanged (const QItemSelection & selected, const QItemSelection & /*deselected*/)
            {
                QModelIndexList indexes = selected.indexes();
                if (indexes.count())
                {
                    QModelIndex index = indexes.at(0);
                    if (index.isValid())
                    {
                        emit selectedIndex(index);
                        return;
                    }
                }
                emit selectedIndex(QModelIndex());
            }
    };
}

#endif // EFFECTSEDITORTOOL_H
