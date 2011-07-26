#ifndef ABSTRACTPHOTOEFFECTFACTORY_H
#define ABSTRACTPHOTOEFFECTFACTORY_H

#include <QObject>
#include <QDomDocument>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhotoEffectInterface;
    class AbstractPhotoEffectFactory : public QObject
    {
        public:

            AbstractPhotoEffectFactory(QObject * parent = 0) :
                QObject(parent)
            {}
            virtual ~AbstractPhotoEffectFactory()
            {}

          /** Returns effects property browser.
            * \arg effect - pass effect object for configuration or pass NULL to get default \class QtAbstractPropertyBrowser
            * which can be passed to \fn getEffectInstance() method.
            */
          //  virtual QtAbstractPropertyBrowser * propertyBrowser(AbstractPhotoEffect * effect = 0) const = 0;

          /** Returns effects instance.
            * \arg browser - as this argument you can set \class QtAbstractPropertyBrowser recieved from virtual
            * \fn propertyBrowser() method of this object.
            */
            virtual AbstractPhotoEffectInterface * getEffectInstance() = 0;

          /** Returns effect name.
            * This name is used be the user to identify effect. This name should be unique becouse effects are identified by this name.
            * Moreover, this name is used in UI to name effects.
            */
            virtual QString effectName() const = 0;

          /** Returns DOM node which contains effects attributes
            */
            virtual QDomElement toSvg(AbstractPhotoEffectInterface * effect, QDomDocument & document) = 0;

          /** Reads node attributes from DOM node and returns ready effect object.
            */
            virtual AbstractPhotoEffectInterface * fromSvg(QDomElement & element) = 0;
    };
}

Q_DECLARE_INTERFACE(KIPIPhotoFramesEditor::AbstractPhotoEffectFactory,"pl.coder89.pfe.AbstractPhotoEffectFactory/1.0")

#endif // ABSTRACTPHOTOEFFECTFACTORY_H
