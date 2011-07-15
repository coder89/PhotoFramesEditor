#ifndef EFFECTSEDITORTOOL_H
#define EFFECTSEDITORTOOL_H

#include "AbstractTool.h"

#include <QModelIndex>

namespace KIPIPhotoFramesEditor
{
    class EffectsEditorToolPrivate;

    class EffectsEditorTool : public AbstractTool
    {
            Q_OBJECT

            EffectsEditorToolPrivate * d;

        public:

            explicit EffectsEditorTool(QWidget * parent = 0);
            virtual void currentItemChanged();

        signals:

        protected slots:

            void viewCurrentEffectEditor(const QModelIndex & index);
    };
}

#endif // EFFECTSEDITORTOOL_H
