#ifndef ABSTRACTPROPERTIESMODEL_H
#define ABSTRACTPROPERTIESMODEL_H

#include <QWidget>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhoto;
    class EffectsListView;

    class AbstractTool : public QWidget
    {
            Q_OBJECT
            Q_PROPERTY(AbstractPhoto * m_photo READ currentItem WRITE setCurrentItem)

            AbstractPhoto * m_photo;

        public:

            AbstractTool(QWidget * parent = 0);

            /// Current photo for editing
            AbstractPhoto * currentItem();
            void setCurrentItem(AbstractPhoto * photo);
            virtual void currentItemChanged();
    };
}

#endif // ABSTRACTPROPERTIESMODEL_H
