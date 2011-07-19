#ifndef GREYSCALEPHOTOEFFECT_H
#define GREYSCALEPHOTOEFFECT_H

#include "GreyscalePhotoEffect_global.h"
#include "AbstractPhotoEffectFactory.h"

using namespace KIPIPhotoFramesEditor;

class GREYSCALEPHOTOEFFECTSHARED_EXPORT GreyscalePhotoEffectFactory : public AbstractPhotoEffectFactory
{
        Q_OBJECT
        Q_INTERFACES(KIPIPhotoFramesEditor::AbstractPhotoEffectFactory)

    public:

        GreyscalePhotoEffectFactory(QObject * parent = 0);
        virtual AbstractPhotoEffectInterface * getEffectInstance();
        virtual QString effectName() const;
};

#endif // GREYSCALEPHOTOEFFECT_H
