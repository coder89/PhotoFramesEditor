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

            static QString MAIN;
            static QString MAIN_RECENT_FILES;
            static QString MAIN_RECENT_FILES_COUNT;
            static QString SAVING;
            static QString SAVING_EMBED_IMG;
            static QString VIEW;
            static QString VIEW_GRID_SHOW;
            static QString VIEW_GRID_X;
            static QString VIEW_GRID_Y;

        public:

            ~PFEConfig();

            static PFEConfig * self();

            static void addRecentFile(const KUrl & file);
            static void addRecentFile(const QString & file);
            static void setRecentFiles(const KUrl::List & files);
            static void setRecentFiles(const QStringList & files);
            static KUrl::List recentFiles();
            static void setMaxRecentFilesCount(unsigned count);
            static unsigned maxRecentFilesCount();

            static bool embedImagesData();
            static void setEmbedImagesData(bool embed);

            static bool showGrid();
            static void setShowGrid(bool isVisible);
            static qreal gridX();
            static void setGridX(qreal x);
            static qreal gridY();
            static void setGridY(qreal y);

            static KConfigSkeleton * configSkeleton();

        private:

            explicit PFEConfig();
            KConfigSkeleton * skeleton();
    };
}

#endif // PFESETTINGS_H
