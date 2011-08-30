#include "plugin_photoframeseditor.h"

#include <kdebug.h>
#include <kgenericfactory.h>
#include <kicon.h>
#include <kaction.h>
#include <kactioncollection.h>

#include <libkipi/interface.h>

#include "photoframeseditor.h"

using namespace KIPIPhotoFramesEditor;

K_PLUGIN_FACTORY ( PhotoFrmesEditorFactory, registerPlugin<Plugin_PhotoFramesEditor>(); )
K_EXPORT_PLUGIN  ( PhotoFrmesEditorFactory("kipiplugin_photoframeseditor") )

Plugin_PhotoFramesEditor::Plugin_PhotoFramesEditor(QObject * parent, const QVariantList & args) :
    KIPI::Plugin( PhotoFrmesEditorFactory::componentData(), parent, "Photo Frames Editor" )
{
    m_interface    = 0;
    m_action       = 0;
    m_parentWidget = 0;
    m_manager      = 0;

    kDebug() << "Plugin_PhotoFramesEditor plugin loaded";
}

Plugin_PhotoFramesEditor::~Plugin_PhotoFramesEditor()
{}

KIPI::Category Plugin_PhotoFramesEditor::category( KAction * action ) const
{
    if ( action == m_action )
       return KIPI::ToolsPlugin;

    kWarning() << "Unrecognized action for plugin category identification";
    return KIPI::ToolsPlugin; // no warning from compiler, please
}

void Plugin_PhotoFramesEditor::setup( QWidget * widget )
{
    m_parentWidget = widget;
    KIPI::Plugin::setup(m_parentWidget);

    m_action = actionCollection()->addAction("photolayoutseditor");
    m_action->setText(i18n("Create photo layouts..."));
    m_action->setIcon(KIcon("photolayoutseditor"));

    connect(m_action, SIGNAL(triggered(bool)),
            this, SLOT(slotActivate()));

    addAction(m_action);

    m_interface = dynamic_cast< KIPI::Interface* >(parent());
    if (!m_interface)
    {
       kError() << "Kipi interface is null!";
       return;
    }
}

void Plugin_PhotoFramesEditor::slotActivate()
{
    if (!m_interface)
    {
        kError() << "Kipi interface is null!";
        return;
    }

    m_manager = PhotoFramesEditor::instance(m_parentWidget);
    m_manager->setInterface(m_interface);
    m_manager->show();
}
