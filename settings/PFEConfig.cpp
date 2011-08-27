#include "PFEConfig.h"
#include "global.h"
#include "PFEConfigSkeleton.h"

#include <QStringList>

#include <klocalizedstring.h>
#include <kconfiggroup.h>

using namespace KIPIPhotoFramesEditor;

PFEConfig * PFEConfig::m_instance = 0;
QString PFEConfig::SAVING = "saving";
QString PFEConfig::SAVING_EMBED_IMG = "embed_images_data";

QString PFEConfig::VIEW = "view";
QString PFEConfig::VIEW_GRID_SHOW = "view_grid_show";
QString PFEConfig::VIEW_GRID_X = "view_grid_x";
QString PFEConfig::VIEW_GRID_Y = "view_grid_y";

PFEConfig::PFEConfig() :
    KConfig(),
    m_skeleton(0)
{
    QStringList groups = this->groupList();

    if (!groups.contains(SAVING))
    {
        KConfigGroup saving = group(SAVING);
        saving.writeEntry<bool>(SAVING_EMBED_IMG, true);
    }
    this->sync();
}

PFEConfig::~PFEConfig()
{
    m_instance = 0;
    Q_DELETE(m_skeleton);
}

PFEConfig * PFEConfig::self()
{
    if (!m_instance)
        m_instance = new PFEConfig();
    return m_instance;
}

bool PFEConfig::embedImagesData()
{
    KConfigGroup saving = self()->group(SAVING);
    return saving.readEntry<bool>(SAVING_EMBED_IMG, true);
}

void PFEConfig::setEmbedImagesData(bool embed)
{
    KConfigGroup saving = self()->group(SAVING);
    saving.writeEntry<bool>(SAVING_EMBED_IMG, embed);
    self()->sync();
}


bool PFEConfig::showGrid()
{
    KConfigGroup view = self()->group(VIEW);
    return view.readEntry<bool>(VIEW_GRID_SHOW, false);
}

void PFEConfig::setShowGrid(bool isVisible)
{
    KConfigGroup view = self()->group(VIEW);
    view.writeEntry<bool>(VIEW_GRID_SHOW, isVisible);
    self()->sync();
}

qreal PFEConfig::gridX()
{
    KConfigGroup view = self()->group(VIEW);
    return view.readEntry<qreal>(VIEW_GRID_X, 25.0);
}

void PFEConfig::setGridX(qreal x)
{
    if (x <= 0)
        return;
    KConfigGroup view = self()->group(VIEW);
    view.writeEntry<qreal>(VIEW_GRID_X, x);
    self()->sync();
}

qreal PFEConfig::gridY()
{
    KConfigGroup view = self()->group(VIEW);
    return view.readEntry<qreal>(VIEW_GRID_Y, 25.0);
}

void PFEConfig::setGridY(qreal y)
{
    if (y <= 0)
        return;
    KConfigGroup view = self()->group(VIEW);
    view.writeEntry<qreal>(VIEW_GRID_Y, y);
    self()->sync();
}

KConfigSkeleton * PFEConfig::configSkeleton()
{
    return self()->skeleton();
}

KConfigSkeleton * PFEConfig::skeleton()
{
    if (!m_skeleton)
        m_skeleton = PFEConfigSkeleton::self();
    return m_skeleton;
}
