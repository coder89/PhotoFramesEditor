#ifndef ABSTRACTPHOTOEFFECTS_H
#define ABSTRACTPHOTOEFFECTS_H

#include <QImage>
#include <QObject>
#include <QSharedPointer>
#include <QUndoCommand>
#include <QtAbstractPropertyBrowser>
#include <QSemaphore>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhoto;
    class PhotoEffectsGroup;

    class AbstractPhotoEffect : public QObject
    {
            Q_OBJECT

            QString m_name;
            QSemaphore sem;

            class OpacityUndoCommand;

        public:

            explicit AbstractPhotoEffect(const QString & name, QObject * parent = 0);
            PhotoEffectsGroup * group() const;
            AbstractPhoto * photo() const;

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

            virtual QImage apply(const QImage & image);
            virtual QtAbstractPropertyBrowser * propertyBrowser() const;
            virtual QString toString() const = 0;

          /**
            * Opacity property
            */
            Q_PROPERTY(int m_opacity READ opacity WRITE setOpacity)
            void setOpacity(int opacity)
            {
                m_opacity = opacity;
                emit effectChanged(this);
            }
            int opacity() const
            {
                return m_opacity;
            }

        protected:

            int m_opacity;
            QUndoCommand * m_undo_command;
            static const QString OPACITY_STRING;

            void beginUndoCommandChange()
            {
                sem.acquire();
            }

            void endUndoCommandChange()
            {
                sem.release();
            }

        protected slots:

            void emitEffectChange();
            virtual void propertyChanged(QtProperty * property);

        signals:

            void effectChanged(AbstractPhotoEffect * effect);
    };
}

#endif // ABSTRACTPHOTOEFFECTS_H
