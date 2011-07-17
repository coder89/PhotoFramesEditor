#ifndef COLORIZEPHOTOEFFECT_H
#define COLORIZEPHOTOEFFECT_H

#include "ColorizePhotoEffect_global.h"
#include "AbstractPhotoEffectFactory.h"

using namespace KIPIPhotoFramesEditor;

class COLORIZEPHOTOEFFECTSHARED_EXPORT ColorizePhotoEffectFactory : public AbstractPhotoEffectFactory
{
        Q_OBJECT
        Q_INTERFACES(KIPIPhotoFramesEditor::AbstractPhotoEffectFactory)

    public:

        ColorizePhotoEffectFactory(QObject * parent = 0);
        virtual AbstractPhotoEffectInterface * getEffectInstance();
        virtual QString effectName() const;
};

#endif // COLORIZEPHOTOEFFECT_H
