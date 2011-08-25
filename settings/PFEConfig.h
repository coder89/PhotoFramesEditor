#ifndef PFESETTINGS_H
#define PFESETTINGS_H

#include <kconfig.h>
#include <kconfigskeleton.h>

namespace KIPIPhotoFramesEditor
{
    class PFEConfig : public KConfig
    {
            KConfigSkeleton * m_skeleton;

            static PFEConfig * m_instance;

            static QString SAVING;
            static QString SAVING_EMBED_IMG;
            static QString VIEW;
            static QString VIEW_GRID_X;
            static QString VIEW_GRID_Y;

        public:

            ~PFEConfig();

            static PFEConfig * instance(QObject * parent = 0);

            static bool embedImagesData();
            static void setEmbedImagesData(bool embed);

            static qreal gridX();
            static void setGridX(qreal x);
            static qreal gridY();
            static void setGridY(qreal y);

            static KConfigSkeleton * configSkeleton();

        private:

            explicit PFEConfig(QObject *parent = 0);
            KConfigSkeleton * skeleton();
    };
}

#endif // PFESETTINGS_H
