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

            explicit TextEditorTool(QWidget *parent = 0);

        signals:

        public slots:

            virtual void currentItemAboutToBeChanged();
            virtual void currentItemChanged();

        protected slots:

           // void propertyChanged(QtProperty * property);
    };
}

#endif // TEXTEDITORTOOL_H
