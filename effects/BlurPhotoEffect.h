#ifndef BLURPHOTOEFFECT_H
#define BLURPHOTOEFFECT_H

#include "BlurPhotoEffect_global.h"
#include "AbstractPhotoEffectFactory.h"

using namespace KIPIPhotoFramesEditor;

class BLURPHOTOEFFECTSHARED_EXPORT BlurPhotoEffectFactory : public AbstractPhotoEffectFactory
{
        Q_OBJECT
        Q_INTERFACES(KIPIPhotoFramesEditor::AbstractPhotoEffectFactory)

    public:

        BlurPhotoEffectFactory(QObject * parent = 0);
        virtual AbstractPhotoEffectInterface * getEffectInstance();
        virtual QString effectName() const;
};

#endif // BLURPHOTOEFFECT_H
