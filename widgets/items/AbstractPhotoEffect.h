#ifndef ABSTRACTPHOTOEFFECTS_H
#define ABSTRACTPHOTOEFFECTS_H

#include <QObject>
#include <QImage>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhotoEffectsGroup;

    class AbstractPhotoEffect : public QObject
    {
            Q_OBJECT

            QString m_name;

        public:

            explicit AbstractPhotoEffect(const QString & name, QObject *parent = 0);

          /**
            * Name propetry
            */
            Q_PROPERTY(QString m_name READ name WRITE setName)
            QString name() const
            {
                return m_name;
            }
            void setName(const QString & name)
            {
                m_name = name;
            }

            virtual QImage apply(const QImage & image) = 0;
    };
}

#endif // ABSTRACTPHOTOEFFECTS_H
