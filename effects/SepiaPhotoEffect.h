#ifndef SEPIAPHOTOEFFECT_H
#define SEPIAPHOTOEFFECT_H

#include "SepiaPhotoEffect_global.h"
#include "AbstractPhotoEffectFactory.h"

using namespace KIPIPhotoFramesEditor;

class SEPIAPHOTOEFFECTSHARED_EXPORT SepiaPhotoEffectFactory : public AbstractPhotoEffectFactory
{
        Q_OBJECT
        Q_INTERFACES(KIPIPhotoFramesEditor::AbstractPhotoEffectFactory)

    public:

        SepiaPhotoEffectFactory(QObject * parent = 0);
        virtual AbstractPhotoEffectInterface * getEffectInstance();
        virtual QString effectName() const;
};

#endif // SEPIAPHOTOEFFECT_H
