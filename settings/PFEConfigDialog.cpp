#include "PFEConfigDialog.h"

#include "PFEConfig.h"
#include "PFEConfigSkeleton.h"
#include "PFEConfigViewWidget.h"

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

PFEConfigDialog::PFEConfigDialog(QWidget * parent) :
    KConfigDialog(parent, "settings", PFEConfig::configSkeleton())
{
    PFEConfigViewWidget * confWdg = new PFEConfigViewWidget( 0, i18n("View") );
    this->addPage( confWdg, i18n("View") )->setIcon(KIcon(QIcon(":view.png")));
}
