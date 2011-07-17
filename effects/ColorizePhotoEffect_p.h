#ifndef COLORIZEPHOTOEFFECT_P_H
#define COLORIZEPHOTOEFFECT_P_H

#include "ColorizePhotoEffect_global.h"
#include "AbstractPhotoEffectInterface.h"

class ColorizePhotoEffect : public KIPIPhotoFramesEditor::AbstractPhotoEffectInterface
{
        Q_INTERFACES(KIPIPhotoFramesEditor::AbstractPhotoEffectInterface)

    public:

        explicit ColorizePhotoEffect(QObject * parent = 0);
        virtual QImage apply(const QImage & image) const;
        virtual QString effectName() const;
        virtual QString toString() const;
        virtual operator QString() const;
};

#endif // COLORIZEPHOTOEFFECT_P_H
