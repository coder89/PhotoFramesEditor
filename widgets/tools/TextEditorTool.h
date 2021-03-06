#ifndef TEXTEDITORTOOL_H
#define TEXTEDITORTOOL_H

#include "AbstractItemsTool.h"

#include <QtAbstractPropertyBrowser>

namespace KIPIPhotoFramesEditor
{
    class TextItem;

    class TextEditorTool : public AbstractItemsTool
    {
            Q_OBJECT

            TextItem * m_text_item;
            TextItem * m_created_text_item;
            QtAbstractPropertyBrowser * m_browser;
            bool m_create_new_item;

        public:

            explicit TextEditorTool(Scene * scene, QWidget * parent = 0);

        signals:

        public slots:

            virtual void currentItemAboutToBeChanged();
            virtual void currentItemChanged();
            virtual void positionAboutToBeChanged();
            virtual void positionChanged();
    };
}

#endif // TEXTEDITORTOOL_H
