#ifndef EFFECTSEDITORTOOL_H
#define EFFECTSEDITORTOOL_H

#include "AbstractTool.h"

#include <QListView>
#include <QModelIndex>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhotoEffect;
    class EffectsEditorToolPrivate;

    class EffectsEditorTool : public AbstractTool
    {
            Q_OBJECT

        public:
            explicit EffectsEditorTool(QWidget * parent = 0);
            virtual void currentItemChanged();
        protected slots:
            void viewCurrentEffectEditor(AbstractPhotoEffect * effect);
        private:
            void removeCurrentPropertyBrowser();
            EffectsEditorToolPrivate * d;
    };

    class EffectsListView : public QListView
    {
            Q_OBJECT
        public:
            EffectsListView(QWidget * parent = 0) : QListView(parent) {}
        signals:
            void selectionChanged(AbstractPhotoEffect * effect);
        protected:
            virtual void selectionChanged (const QItemSelection & selected, const QItemSelection & /*deselected*/)
            {
                QModelIndexList indexes = selected.indexes();
                if (indexes.count())
                {
                    QModelIndex index = indexes.at(0);
                    if (index.isValid())
                    {
                        emit selectionChanged(static_cast<AbstractPhotoEffect*>(index.internalPointer()));
                        return;
                    }
                }
                emit selectionChanged(0);
            }
    };
}

#endif // EFFECTSEDITORTOOL_H
