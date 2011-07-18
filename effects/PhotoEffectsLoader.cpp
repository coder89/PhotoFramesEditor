#include "PhotoEffectsLoader.h"
#include "PhotoEffectsGroup.h"
#include "AbstractPhoto.h"
#include "UndoCommandEvent.h"
#include "KEditFactory.h"
#include "global.h"
#include "AbstractPhotoEffectFactory.h"

#include <QtAbstractPropertyBrowser>
#include <QtTreePropertyBrowser>
#include <QtIntPropertyManager>
#include <QtVariantPropertyManager>

#include <kapplication.h>
#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

PhotoEffectsLoader * PhotoEffectsLoader::m_instance = 0;
QMap<QString, AbstractPhotoEffectFactory*> PhotoEffectsLoader::registeredEffects;
const QString PhotoEffectsLoader::OPACITY_STRING = i18n("Opacity");

class PhotoEffectsLoader::OpacityUndoCommand : public QUndoCommand
{
        PhotoEffectsLoader * m_effect;
        int m_opacity;
    public:
        OpacityUndoCommand(PhotoEffectsLoader * effect, int opacity);
        virtual void redo();
        virtual void undo();
        void setOpacity(int opacity);
    private:
        void runCommand()
        {
            int temp = m_effect->opacity();
            if (m_opacity != temp)
            {
                m_effect->setOpacity(m_opacity);
                m_opacity = temp;
            }
        }
};

PhotoEffectsLoader::OpacityUndoCommand::OpacityUndoCommand(PhotoEffectsLoader * effect, int opacity) :
    m_effect(effect),
    m_opacity(opacity)
{}

void PhotoEffectsLoader::OpacityUndoCommand::redo()
{
    runCommand();
}

void PhotoEffectsLoader::OpacityUndoCommand::undo()
{
    runCommand();
}

void PhotoEffectsLoader::OpacityUndoCommand::setOpacity(int opacity)
{
    m_opacity = opacity;
}

PhotoEffectsLoader::PhotoEffectsLoader(QObject * parent) :
    QObject(parent),
    sem(1),
    m_opacity(100),
    m_undo_command(0)
{
}


PhotoEffectsLoader * PhotoEffectsLoader::instance(QObject * parent)
{
    if (m_instance)
    {
        if (parent)
            m_instance->setParent(parent);
        return m_instance;
    }
    return (m_instance = new PhotoEffectsLoader(parent));
}

PhotoEffectsGroup * PhotoEffectsLoader::group() const
{
    return qobject_cast<PhotoEffectsGroup*>(this->parent());
}

AbstractPhoto * PhotoEffectsLoader::photo() const
{
    PhotoEffectsGroup * tempGroup = this->group();
    if (tempGroup)
        return tempGroup->photo();
    else
        return 0;
}

QImage PhotoEffectsLoader::apply(const QImage & image)
{
    if (opacity() != 100)
    {
        QImage result(image.size(),QImage::Format_ARGB32_Premultiplied);
        QPainter p(&result);
        p.drawImage(0,0,image);
        p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        p.fillRect(image.rect(), QColor(0, 0, 0, m_opacity*255/100));
        return result;
    }
    return image;
}

QtAbstractPropertyBrowser * PhotoEffectsLoader::propertyBrowser() const
{
    QtTreePropertyBrowser * browser = new QtTreePropertyBrowser();
    QtIntPropertyManager * intManager = new QtIntPropertyManager(browser);
    KSliderEditFactory * sliderFactory = new KSliderEditFactory(browser);
    browser->setFactoryForManager(intManager,sliderFactory);

    // Opacity property
    QtProperty * opacity = intManager->addProperty(OPACITY_STRING);
    intManager->setMaximum(opacity,100);
    intManager->setMinimum(opacity,0);
    browser->addProperty(opacity);

    intManager->setValue(opacity,m_opacity);

    connect(sliderFactory,SIGNAL(editingFinished()),this,SLOT(postEffectChangedEvent()));
    connect(intManager,SIGNAL(propertyChanged(QtProperty*)),this,SLOT(propertyChanged(QtProperty*)));

    return browser;
}

bool PhotoEffectsLoader::registerEffect(AbstractPhotoEffectFactory * effectFactory)
{
    QString effectName = effectFactory->effectName();
    registeredEffects.insert(effectName, effectFactory);
}

QStringList PhotoEffectsLoader::registeredEffectsNames()
{
    return registeredEffects.keys();
}

AbstractPhotoEffectInterface * PhotoEffectsLoader::getEffectByName(const QString & effectName)
{
    AbstractPhotoEffectFactory * factory = PhotoEffectsLoader::registeredEffects[effectName];
    if (factory)
        return factory->getEffectInstance();
    return 0;
}

QtAbstractPropertyBrowser * PhotoEffectsLoader::propertyBrowser(AbstractPhotoEffectInterface * effect)
{
    PhotoEffectsLoader * loader = instance();
    loader->postEffectChangedEvent();

    QtAbstractPropertyBrowser * browser = new QtTreePropertyBrowser();

    loader->m_effect = effect;
    if (effect)
    {
        // QVariant::Int
        QtIntPropertyManager * intManager = new QtIntPropertyManager(browser);
        KSliderEditFactory * sliderEditFactory = new KSliderEditFactory(browser);
        browser->setFactoryForManager(intManager, sliderEditFactory);
        // QVariant::Color
        QtColorPropertyManager * colorManager = 0;
        KColorEditorFactory * colorFactory = 0;
        // QVariant others....
        QtVariantPropertyManager * variantManager = 0;
        KVariantEditorFactory * variantFactory = 0;
        QList<AbstractPhotoEffectProperty*> effectProperties = effect->effectProperties();
        foreach (AbstractPhotoEffectProperty * property, effectProperties)
        {
            QtProperty * qtproperty;
            switch(property->value.type())
            {
                case QVariant::Int:
                    qtproperty = intManager->addProperty(property->id);
                    intManager->setValue(qtproperty,property->value.toInt());
                    intManager->setMaximum(qtproperty,property->data[AbstractPhotoEffectProperty::Maximum].toInt());
                    intManager->setMinimum(qtproperty,property->data[AbstractPhotoEffectProperty::Minimum].toInt());
                    browser->addProperty(qtproperty);
                    break;
                case QVariant::Color:
                    if (!colorManager || !colorFactory)
                    {
                        colorManager = new QtColorPropertyManager(browser);
                        colorFactory = new KColorEditorFactory(browser);
                        browser->setFactoryForManager(colorManager,colorFactory);
                        connect(colorManager,SIGNAL(propertyChanged(QtProperty*)),loader,SLOT(propertyChanged(QtProperty*)));
                        connect(colorFactory,SIGNAL(editingFinished()),loader,SLOT(postEffectChangedEvent()));
                    }
                    qtproperty = colorManager->addProperty(property->id);
                    colorManager->setValue(qtproperty,property->value.value<QColor>());
                    browser->addProperty(qtproperty);
                    break;
                default:
                    if (!variantManager || !variantFactory)
                    {
                        variantManager = new QtVariantPropertyManager(browser);
                        variantFactory = new KVariantEditorFactory(browser);
                        browser->setFactoryForManager(variantManager,variantFactory);
                        connect(variantManager,SIGNAL(propertyChanged(QtProperty*)),loader,SLOT(propertyChanged(QtProperty*)));
                        connect(variantFactory,SIGNAL(editingFinished()),loader,SLOT(postEffectChangedEvent()));
                    }
                    break;
            }
        }
        connect(intManager,SIGNAL(propertyChanged(QtProperty*)),loader,SLOT(propertyChanged(QtProperty*)));
        connect(sliderEditFactory,SIGNAL(editingFinished()),loader,SLOT(postEffectChangedEvent()));
    }
    else
        browser->setEnabled(false);
    return browser;
}

void PhotoEffectsLoader::propertyChanged(QtProperty * property)
{
    sem.acquire();
    AbstractPhotoEffectProperty * tempProperty;
    foreach (AbstractPhotoEffectProperty * currentProperty, m_effect_edited_properties)
    {
        if (currentProperty->id == property->propertyName())
        {
            setEffectPropertyValue(currentProperty, property);
            goto _return;
        }
    }
    foreach (AbstractPhotoEffectProperty * currentProperty, m_effect->effectProperties())
    {
        if (currentProperty->id == property->propertyName())
        {
            tempProperty = new AbstractPhotoEffectProperty(currentProperty->id);
            tempProperty->value = currentProperty->value;
            tempProperty->data = currentProperty->data;
            setEffectPropertyValue(tempProperty, property);
            m_effect_edited_properties.append(tempProperty);
            goto _return;
        }
    }
    _return:
    sem.release();
}

void PhotoEffectsLoader::postEffectChangedEvent()
{
    qDebug() << "Command posted!";
    sem.acquire();
    QUndoCommand * command = m_effect->createChangeCommand(m_effect_edited_properties);
    if (command)
    {
        UndoCommandEvent * event = new UndoCommandEvent();
        event->setUndoCommand(command);
        KApplication::postEvent(PhotoFramesEditor::instancePhotoFramesEditor(),event);
    }
    m_effect_edited_properties.clear();
    sem.release();
}

void PhotoEffectsLoader::setEffectPropertyValue(AbstractPhotoEffectProperty * effectProperty, QtProperty * property)
{
    switch (effectProperty->value.type())
    {
        case QVariant::Int:
            effectProperty->value = qobject_cast<QtIntPropertyManager*>(property->propertyManager())->value(property);
            return;
        case QVariant::Color:
            effectProperty->value = qobject_cast<QtColorPropertyManager*>(property->propertyManager())->value(property);
            return;
        default:
            qDebug() << "Unexpected property type";
            return;
    }
}
