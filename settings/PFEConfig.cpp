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

PFEConfig::PFEConfig(QObject *parent) :
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

PFEConfig * PFEConfig::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new PFEConfig(parent);
    return m_instance;
}

bool PFEConfig::embedImagesData()
{
    KConfigGroup saving = instance()->group(SAVING);
    return saving.readEntry<bool>(SAVING_EMBED_IMG, true);
}

void PFEConfig::setEmbedImagesData(bool embed)
{
    KConfigGroup saving = instance()->group(SAVING);
    saving.writeEntry<bool>(SAVING_EMBED_IMG, embed);
    instance()->sync();
}


bool PFEConfig::showGrid()
{
    KConfigGroup view = instance()->group(VIEW);
    return view.readEntry<bool>(VIEW_GRID_SHOW, false);
}

void PFEConfig::setShowGrid(bool isVisible)
{
    KConfigGroup view = instance()->group(VIEW);
    view.writeEntry<bool>(VIEW_GRID_SHOW, isVisible);
    instance()->sync();
}

qreal PFEConfig::gridX()
{
    KConfigGroup view = instance()->group(VIEW);
    return view.readEntry<qreal>(VIEW_GRID_X, 25.0);
}

void PFEConfig::setGridX(qreal x)
{
    if (x <= 0)
        return;
    KConfigGroup view = instance()->group(VIEW);
    view.writeEntry<qreal>(VIEW_GRID_X, x);
    instance()->sync();
}

qreal PFEConfig::gridY()
{
    KConfigGroup view = instance()->group(VIEW);
    return view.readEntry<qreal>(VIEW_GRID_Y, 25.0);
}

void PFEConfig::setGridY(qreal y)
{
    if (y <= 0)
        return;
    KConfigGroup view = instance()->group(VIEW);
    view.writeEntry<qreal>(VIEW_GRID_Y, y);
    instance()->sync();
}

KConfigSkeleton * PFEConfig::configSkeleton()
{
    return instance()->skeleton();
}

KConfigSkeleton * PFEConfig::skeleton()
{
    if (!m_skeleton)
        m_skeleton = PFEConfigSkeleton::self();
    return m_skeleton;
}
