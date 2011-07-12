#ifndef ABSTRACTPHOTOEFFECTSGROUP_H
#define ABSTRACTPHOTOEFFECTSGROUP_H

#include <QObject>
#include <QPixmap>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhoto;
    class AbstractPhotoEffect;

    class AbstractPhotoEffectsGroup : public QObject
    {
            Q_OBJECT

            QList<AbstractPhotoEffect*> m_effects_list;

        public:

            explicit AbstractPhotoEffectsGroup(QObject * parent = 0);

        signals:

            void effectsChanged();

        public slots:

            void push_back(AbstractPhotoEffect * effect);
            void push_front(AbstractPhotoEffect * effect);
            QPixmap apply(const QPixmap & pixmap);

    };
}

#endif // ABSTRACTPHOTOEFFECTSGROUP_H
