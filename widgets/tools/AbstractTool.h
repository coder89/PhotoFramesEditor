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

          /** Current photo property
            * This property holds an information which item is currently editing.
            */
            Q_PROPERTY(AbstractPhoto * m_photo READ currentItem WRITE setCurrentItem)
            AbstractPhoto * currentItem();
            void setCurrentItem(AbstractPhoto * photo);

        public slots:

          /** This slot is called before current item change
            * It gives a chanse to save changes of currently edited item.
            */
            virtual void currentItemAboutToBeChanged() = 0;

          /** This slot is called after current item changed.
            * This is a notification to open editors/tools and configure it for new item.
            */
            virtual void currentItemChanged() = 0;
    };
}

#endif // ABSTRACTPROPERTIESMODEL_H
