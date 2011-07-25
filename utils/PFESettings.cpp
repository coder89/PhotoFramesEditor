#include "PFESettings.h"
#include "global.h"

#include <QStringList>

using namespace KIPIPhotoFramesEditor;

PFESettings * PFESettings::m_instance = 0;
QString PFESettings::SLASH = "/";
QString PFESettings::SAVING = "saving";
QString PFESettings::SAVING_EMBED_IMG = "embed_images_data";

PFESettings::PFESettings(QObject *parent) :
    QSettings(QSettings::UserScope, "KDE", KIPIPhotoFramesEditor::name(), parent)
{
    QStringList groups = this->childGroups();

    if (!groups.contains(SAVING))
    {
        beginGroup(SAVING);
        setValue(SAVING_EMBED_IMG, false);
        endGroup();
    }
    this->sync();
}

PFESettings::~PFESettings()
{
    m_instance = 0;
}

PFESettings * PFESettings::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new PFESettings(parent);
    return m_instance;
}

bool PFESettings::embedImagesData()
{
    return m_instance->value(SAVING + SLASH + SAVING_EMBED_IMG, false).toBool();
}

void PFESettings::setEmbedImagesData(bool embed)
{
    m_instance->setValue(SAVING + SLASH + SAVING_EMBED_IMG, embed);
    m_instance->sync();
}
