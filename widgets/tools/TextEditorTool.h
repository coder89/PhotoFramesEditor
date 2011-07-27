#ifndef TEXTEDITORTOOL_H
#define TEXTEDITORTOOL_H

#include "AbstractTool.h"

#include <QtAbstractPropertyBrowser>

namespace KIPIPhotoFramesEditor
{
    class TextItem;

    class TextEditorTool : public AbstractTool
    {
            Q_OBJECT

            TextItem * m_text_item;
            QtAbstractPropertyBrowser * m_browser;

        public:

            explicit TextEditorTool(ToolsDockWidget * parent = 0);

        signals:

        public slots:

            virtual void currentItemAboutToBeChanged();
            virtual void currentItemChanged();
            virtual void positionAboutToBeChanged();
            virtual void positionChanged();
    };
}

#endif // TEXTEDITORTOOL_H
