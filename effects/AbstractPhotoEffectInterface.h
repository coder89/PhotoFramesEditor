#ifndef ABSTRACTPROTOEFFECTINTERFACE_H
#define ABSTRACTPROTOEFFECTINTERFACE_H

#include <QtPlugin>
#include <QVariant>
#include <QList>
#include <QImage>
#include <QPainter>
#include <QUndoCommand>

#include "PhotoEffectsGroup.h"

#include <klocalizedstring.h>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhotoEffectInterface : public QObject
    {
            class AbstractPhotoEffectInterfaceCommand;

        protected:

            enum AttributeTypes
            {
                Maximum,
                Minimum,
                AllowedList,
            };

            struct Property
            {
                Property(const QString & _id) :
                    id(_id)
                {}
                const QString id;
                QString name;
                QVariant value;
                QList<QPair<AttributeTypes,QVariant> > data;
            };

            QList<Property*> m_properties;

        public:

            explicit AbstractPhotoEffectInterface(QObject * parent = 0) :
                QObject(parent)
            {
                Property * opacity = new Property("opacity");
                opacity->name = i18n("Opacity");
                opacity->value = QVariant(100);
                opacity->data << QPair<AttributeTypes,QVariant>(Minimum,0);
                opacity->data << QPair<AttributeTypes,QVariant>(Maximum,100);
                m_properties.push_back(opacity);
            }

            virtual ~AbstractPhotoEffectInterface()
            {
                while (m_properties.count())
                    delete m_properties.takeAt(0);
            }

            virtual QImage apply(const QImage & image) const
            {
                int _opacity = opacity();
                if (_opacity != 100)
                {
                    QImage result(image.size(),QImage::Format_ARGB32_Premultiplied);
                    QPainter p(&result);
                    p.drawImage(0,0,image);
                    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
                    p.fillRect(image.rect(), QColor(0, 0, 0, _opacity*255/100));
                    return result;
                }
                return image;
            }

            virtual QString effectName() const = 0;
            virtual QString toString() const = 0;
            virtual operator QString() const = 0;

            QUndoCommand * createChangeCommand(const QList<Property*> & properties)
            {
                if (properties.count())
                    return new AbstractPhotoEffectInterfaceCommand(this,properties);
                else
                    return 0;
            }

            QList<Property*> effectProperties()
            {
                return m_properties;
            }

            Property * getProperty(const QString & id)
            {
                foreach (Property * currentProperty, m_properties)
                {
                    if (currentProperty->id == id)
                        return currentProperty;
                }
                return 0;
            }

            PhotoEffectsGroup * group() const
            {
                return qobject_cast<PhotoEffectsGroup*>(this->parent());
            }

            int opacity() const
            {
                foreach (Property * property, m_properties)
                    if (property->id == "opacity")
                        return property->value.toInt();
                return 100;
            }

        private:

            void setEffectProperties(const QList<Property*> & properties)
            {
                foreach (Property * newProperty, properties)
                {
                    for (QList<Property*>::iterator it = m_properties.begin(); it != m_properties.end(); ++it)
                    {
                        if ((*it)->id == newProperty->id && newProperty->value.type() == (*it)->value.type())
                            *(it) = newProperty;
                    }
                }
            }

            class AbstractPhotoEffectInterfaceCommand : public QUndoCommand
            {
                    AbstractPhotoEffectInterface * m_effect;
                    QList<AbstractPhotoEffectInterface::Property*> tempProperties;
                public:
                    AbstractPhotoEffectInterfaceCommand(AbstractPhotoEffectInterface * effect, const QList<Property*> & properties, QUndoCommand * parent = 0) :
                        QUndoCommand(parent),
                        m_effect(effect),
                        tempProperties(properties)
                    {
                        this->setText(m_effect->effectName() + i18n(" change"));
                    }
                    virtual void redo()
                    {
                        runCommand();
                    }
                    virtual void undo()
                    {
                        runCommand();
                    }
                private:
                    void runCommand()
                    {
                        QList<Property*> temp = m_effect->effectProperties();
                        m_effect->setEffectProperties(tempProperties);
                        tempProperties = temp;
                        m_effect->group()->emitEffectsChanged(m_effect);
                    }
            };

        friend class AbstractPhotoEffectInterfaceCommand;
    };
}

Q_DECLARE_INTERFACE(KIPIPhotoFramesEditor::AbstractPhotoEffectInterface, "pl.coder89.pfe.AbstractPhotoEffectInterface")

#endif // ABSTRACTPROTOEFFECTINTERFACE_H
