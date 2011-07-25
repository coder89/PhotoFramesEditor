#ifndef PHOTOFRAMESEDITOR_P_H
#define PHOTOFRAMESEDITOR_P_H

// KDE
#include <kaction.h>
#include <kactionmenu.h>
#include <krecentfilesaction.h>
#include <kfiledialog.h>
#include <ktoggleaction.h>

// Qt
#include <QHBoxLayout>
#include <QTreeView>
#include <QDockWidget>

// Local
#include "PFESettings.h"
#include "LayersTree.h"
#include "LayersTreeTitleWidget.h"
#include "ToolsDockWidget.h"
#include "EffectsEditorTool.h"
#include "BorderEditTool.h"
#include "ColorizeTool.h"

#define Q_DELETE(ptr)   if (ptr) delete ptr;

namespace KIPIPhotoFramesEditor
{
    class PhotoFramesEditor::PhotoFramesEditorPriv
    {
        public:
            PhotoFramesEditorPriv() :
                settings(0),
                centralWidget(0),
                openNewFileAction(0),
                openFileAction(0),
                openRecentFilesMenu(0),
                saveAction(0),
                saveAsAction(0),
                exportFileAction(0),
                printPreviewAction(0),
                printAction(0),
                closeAction(0),
                quitAction(0),
                undoAction(0),
                redoAction(0),
                gridActionMenu(0),
                showGridToggleAction(0),
                gridConfigAction(0),
                fileDialog(0),
                tree(0),
                treeWidget(0),
                treeTitle(0),
                toolsWidget(0),
                toolEffects(0),
                toolBorders(0),
                toolColorize(0)
            {}

            ~PhotoFramesEditorPriv()
            {
                Q_DELETE(centralWidget)

                // File menu
                Q_DELETE(openNewFileAction)
                Q_DELETE(openFileAction)
                Q_DELETE(openRecentFilesMenu)
                Q_DELETE(saveAction)
                Q_DELETE(saveAsAction)
                Q_DELETE(exportFileAction)
                Q_DELETE(printPreviewAction)
                Q_DELETE(printAction)
                Q_DELETE(closeAction)
                Q_DELETE(quitAction)

                // Edit menu
                Q_DELETE(undoAction)
                Q_DELETE(redoAction)

                // View menu
                Q_DELETE(gridActionMenu)
                Q_DELETE(showGridToggleAction)
                Q_DELETE(gridConfigAction)

                // Other
                Q_DELETE(fileDialog)
                Q_DELETE(tree)
                //Q_DELETE(treeWidget)  // DELETED BY main window
                //Q_DELETE(treeTitle)   // DELETED BY treeWidget

                //Q_DELETE(toolsWidget)     // DELETED BY main window
                //Q_DELETE(toolEffects)     // DELETED BY main window
                //Q_DELETE(toolBorders)     // DELETED BY main window
                //Q_DELETE(toolColorize)    // DELETED BY main window
            }

            // Settings object
            PFESettings * settings;

            // Central widget of the window
            QWidget *   centralWidget;

            // File menu
            KAction *   openNewFileAction;
            KAction *   openFileAction;
            KRecentFilesAction *   openRecentFilesMenu;
            KAction *   saveAction;
            KAction *   saveAsAction;
            KAction *   exportFileAction;
            KAction *   printPreviewAction;
            KAction *   printAction;
            KAction *   closeAction;
            KAction *   quitAction;

            // Edit menu
            KAction *   undoAction;
            KAction *   redoAction;

            // View menu
            KActionMenu * gridActionMenu;
            KToggleAction * showGridToggleAction;
            KAction *   gridConfigAction;

            // File dialog
            KFileDialog * fileDialog;

            // Tree of layers
            LayersTree *  tree;
            QDockWidget * treeWidget;
            LayersTreeTitleWidget * treeTitle;

            // Tools
            ToolsDockWidget * toolsWidget;
            EffectsEditorTool * toolEffects;
            BorderEditTool * toolBorders;
            ColorizeTool * toolColorize;
    };
}

#undef Q_DELETE

#endif // PHOTOFRAMESEDITOR_P_H
