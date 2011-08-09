#ifndef PLUGIN_PHOTOFRAMESEDITOR_H
#define PLUGIN_PHOTOFRAMESEDITOR_H

#include <QWidget>
#include <QVariant>

#include <libkipi/plugin.h>

namespace KIPI
{
    class Interface;
}

namespace KIPIPhotoFramesEditor
{
    class PhotoFramesEditor;
}

class Plugin_PhotoFramesEditor : public KIPI::Plugin
{
        Q_OBJECT

    public:

        Plugin_PhotoFramesEditor(QObject* parent, const QVariantList& args);
        virtual ~Plugin_PhotoFramesEditor();

        KIPI::Category category( KAction* action ) const;
        void setup( QWidget* );

    public Q_SLOTS:

        void slotActivate();

    private:

        QWidget*                                    m_parentWidget;

        KAction*                                    m_action;

        KIPIPhotoFramesEditor::PhotoFramesEditor*   m_manager;

        KIPI::Interface*                            m_interface;
};

#endif // PLUGIN_PHOTOFRAMESEDITOR_H
