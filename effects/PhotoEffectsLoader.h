#ifndef ABSTRACTPHOTOEFFECTS_H
#define ABSTRACTPHOTOEFFECTS_H

#include <QImage>
#include <QObject>
#include <QSharedPointer>
#include <QUndoCommand>
#include <QSemaphore>
#include <QMap>

class QtProperty;
class QtAbstractPropertyBrowser;

namespace KIPIPhotoFramesEditor
{
    class AbstractPhoto;
    class PhotoEffectsGroup;
    class AbstractPhotoEffectFactory;
    class AbstractPhotoEffectInterface;


    class PhotoEffectsLoader : public QObject
    {
            Q_OBJECT

            class OpacityUndoCommand;

            static QString m_effect_name;
            QString m_name;

            // Semafore (for multi-thread safety)
            QSemaphore sem;

            static QMap<QString, AbstractPhotoEffectFactory*> registeredEffects;

        public:

            explicit PhotoEffectsLoader(QObject * parent = 0);
            PhotoEffectsGroup * group() const;
            AbstractPhoto * photo() const;

            virtual QImage apply(const QImage & image);
            virtual QtAbstractPropertyBrowser * propertyBrowser() const;
            virtual QString toString() const = 0;

          /**
            * Effect name propetry
            */
            Q_PROPERTY(QString m_effect_name READ effectName)
            virtual QString effectName() const = 0;

          /**
            * Name propetry
            */
            Q_PROPERTY(QString m_name READ name WRITE setName)
            virtual QString name() const
            {
                return m_name;
            }
            virtual void setName(const QString & name)
            {
                m_name = name;
            }

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

          /** Registers new effect using it's factory object.
            * \arg effectFactory - this object should be allocated on heap usong \fn operator new(),
            * this class takes over the parenthood of this factory and it will delete it if it'll no longer needed.
            */
            static bool registerEffect(AbstractPhotoEffectFactory * effectFactory);

          /** Returns registered effects names
            * This implementation returns \class QStringList object with effects names obtained by calling \fn effectName()
            * method of its factory object.
            */
            static QStringList registeredEffectsNames();

          /** Return an instance of effect using its name.
            */
            static AbstractPhotoEffectInterface * getEffectByName(const QString & name);

        protected:

            int m_opacity;
            QUndoCommand * m_undo_command;
            static const QString OPACITY_STRING;

          /// Use this function before/after modifying or creating new QUndoCommand for your effect
            void beginUndoCommandChange()
            {
                sem.acquire();
            }
            void endUndoCommandChange()
            {
                sem.release();
            }

        protected slots:

          /** Call this slot to post your QUndoCommand.
            * \note It is good to connect this slot to \fn editingFinished() signal of your QtEditorFacroty objects.
            */
            void postEffectChangedEvent();

          /** This function if called when any QtProperty has changed.
            * If you are reimplementing this function remember to call yours parent's version to process it's
            * properties if \arg property is not your own.
            * \note Remember to create/modify your version of QUndoCommand here.
            * \warning Also remember to connect this function to all of your QtPropertyManager's to
            * recieve all property change events.
            */
            virtual void propertyChanged(QtProperty * property);

        signals:

          /** Effect changed signal.
            * Emit this signal to notify all listeners that your effects state has changed
            * (i.e. opacity has changed and repaint is needed)
            */
            void effectChanged(PhotoEffectsLoader * effect);

        friend class AbstractPhotoEffectFactory;
    };
}

#endif // ABSTRACTPHOTOEFFECTS_H
