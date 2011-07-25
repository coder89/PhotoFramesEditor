#ifndef PFESETTINGS_H
#define PFESETTINGS_H

#include <QSettings>

namespace KIPIPhotoFramesEditor
{
    class PFESettings : public QSettings
    {
            Q_OBJECT

            static PFESettings * m_instance;

            static QString SLASH;
            static QString SAVING;
            static QString SAVING_EMBED_IMG;

        public:

            ~PFESettings();

            static PFESettings * instance(QObject * parent = 0);

            static bool embedImagesData();
            static void setEmbedImagesData(bool embed);

        private:

            explicit PFESettings(QObject *parent = 0);
    };
}

#endif // PFESETTINGS_H
