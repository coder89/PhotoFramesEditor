// Local
#include "photoframeseditor.h"
#include "photoframeseditor_p.h"
#include "CanvasCreationDialog.h"
#include "Canvas.h"
#include "LayersSelectionModel.h"

// Qt
#include <QVBoxLayout>
#include <QDebug>
#include <QTreeView>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QDockWidget>
#include <QHeaderView>
#include <QLabel>
#include <QApplication>

// KDE
#include <kmenubar.h>
#include <kstandardaction.h>
#include <kactioncollection.h>
#include <kfiledialog.h>
#include <ktip.h>
#include <kaboutdata.h>
#include <kmessagebox.h>

using namespace KIPIPhotoFramesEditor;

PhotoFramesEditor * PhotoFramesEditor::m_instance = 0;

PhotoFramesEditor::PhotoFramesEditor(QWidget *parent) :
    KXmlGuiWindow(parent),
    m_canvas_widget(0),
    m_canvas(0),
    d(new PhotoFramesEditorPriv)
{
    setXMLFile(QApplication::applicationDirPath()+"/plugin/photoframeseditorui.rc");

    setObjectName("Photo Frames Editor");

    m_instance = this;

    setupActions();
    refreshActions();
    createWidgets();

    this->setAcceptDrops(true);
}

PhotoFramesEditor::~PhotoFramesEditor()
{
    if (m_canvas)
        delete m_canvas;
    if (d)
        delete d;
}

PhotoFramesEditor * PhotoFramesEditor::instancePhotoFramesEditor(QWidget * parent)
{
    if (m_instance)
        return m_instance;
    else
        return (m_instance = new PhotoFramesEditor(parent));
}

void PhotoFramesEditor::setupActions()
{
    d->openNewFileAction = KStandardAction::openNew(this, SLOT(open()), this);
    d->openNewFileAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_N));
    actionCollection()->addAction("open_new", d->openNewFileAction);
    //------------------------------------------------------------------------
    d->openFileAction = KStandardAction::open(this, SLOT(openDialog()), this);
    d->openFileAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_O));
    actionCollection()->addAction("open", d->openFileAction);
    //------------------------------------------------------------------------
    d->openRecentFilesMenu = KStandardAction::openRecent(this, SLOT(open(const KUrl &)), this);
    actionCollection()->addAction("open_recent", d->openRecentFilesMenu);
    //------------------------------------------------------------------------
    d->saveAction = KStandardAction::save(this, SLOT(save()), this);
    d->saveAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_S));
    actionCollection()->addAction("save", d->saveAction);
    //------------------------------------------------------------------------
    d->saveAsAction = KStandardAction::saveAs(this, SLOT(saveAs()), this);
    d->saveAsAction->setShortcut(KShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_S));
    actionCollection()->addAction("save_as", d->saveAsAction);
    //------------------------------------------------------------------------
    d->exportFileAction = new KAction(i18nc("Export current frame layout to image file", "Export"), this);
    d->exportFileAction->setShortcut(KShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_E));
    connect(d->exportFileAction, SIGNAL(triggered()), this, SLOT(exportFile()));
    actionCollection()->addAction("export", d->exportFileAction);
    //------------------------------------------------------------------------
    d->printPreviewAction = KStandardAction::printPreview(this, SLOT(printPreview()), this);
    d->printPreviewAction->setShortcut(KShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_P));
    actionCollection()->addAction("print_preview", d->printPreviewAction);
    //------------------------------------------------------------------------
    d->printAction = KStandardAction::print(this, SLOT(print()), this);
    d->printAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_P));
    actionCollection()->addAction("print", d->printAction);
    //------------------------------------------------------------------------
    d->closeAction = KStandardAction::close(this, SLOT(closeDocument()), this);
    d->closeAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_Q));
    actionCollection()->addAction("close", d->closeAction);
    //------------------------------------------------------------------------
    d->quitAction = KStandardAction::quit(this, SLOT(close()), this);
    d->quitAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_Q));
    actionCollection()->addAction("quit", d->quitAction);
    //------------------------------------------------------------------------
    d->gridActionMenu = new KActionMenu(i18nc("Grid lines", "Grid lines"), this);
    d->gridActionMenu->setDelayed(false);
    actionCollection()->addAction("grid_lines", d->gridActionMenu);

    d->showGridToggleAction = new KToggleAction(i18nc("View grid lines", "Show"), this);
    d->showGridToggleAction->setShortcut(KShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_G));
    connect(d->showGridToggleAction, SIGNAL(triggered(bool)), this, SLOT(setGridVisible(bool)));
    actionCollection()->addAction("grid_toggle", d->showGridToggleAction);
    d->gridActionMenu->addAction(d->showGridToggleAction);

    d->gridConfigAction = new KAction(i18nc("Configure grid lines visibility", "Setup grid"), this);
    connect(d->gridConfigAction, SIGNAL(triggered()), this, SLOT(setupGrid()));
    actionCollection()->addAction("grid_config", d->gridConfigAction);
    d->gridActionMenu->addAction(d->gridConfigAction);

    createGUI(xmlFile());
}

void PhotoFramesEditor::refreshActions()
{
    bool isEnabledForCanvas = false;
    if (m_canvas)
        isEnabledForCanvas = true;

    d->saveAction->setEnabled(isEnabledForCanvas);
    d->saveAsAction->setEnabled(isEnabledForCanvas);
    d->exportFileAction->setEnabled(isEnabledForCanvas);
    d->printPreviewAction->setEnabled(isEnabledForCanvas);
    d->printAction->setEnabled(isEnabledForCanvas);
    d->closeAction->setEnabled(isEnabledForCanvas);
    d->gridActionMenu->setEnabled(isEnabledForCanvas);
}

void PhotoFramesEditor::createWidgets()
{
    QDockWidget * treeWidget = new QDockWidget("Layers", this);
    treeWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    treeWidget->setFloating(false);
    treeWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    d->tree = new LayersTree(this);
    d->tree->setAnimated(true);
    treeWidget->setWidget(d->tree);
    d->tree->setIconSize(QSize(50,50));
    m_canvas_widget = new CanvasWidget(this);
    this->setCentralWidget(m_canvas_widget);
    this->addDockWidget(Qt::RightDockWidgetArea, treeWidget);

    createCanvas(QSizeF(800,800));                               /// TODO : REMOVE WHEN FINISH
}

void PhotoFramesEditor::createCanvas(const QSizeF & dimension)
{
    if (m_canvas)
        m_canvas->deleteLater();
    m_canvas = new Canvas(dimension, this);
    m_canvas_widget->setCanvas(m_canvas);
    d->tree->setModel(m_canvas->model());
    d->tree->setSelectionModel(m_canvas->selectionModel());
    for (int i = d->tree->model()->columnCount()-1; i >= 0; --i)
        d->tree->resizeColumnToContents(i);
}

void PhotoFramesEditor::open()
{
    closeDocument();

    CanvasCreationDialog * newCanvas = new CanvasCreationDialog(this);
    int result = newCanvas->exec();

    if (result == KDialog::Accepted)
    {
        createCanvas(QSizeF(newCanvas->canvasSize()));
    }

    refreshActions();
}

void PhotoFramesEditor::openDialog()
{
    if (!d->fileDialog)
        d->fileDialog = new KFileDialog(KUrl(), "*.pfe|Photo Frames Editor files", this);
    d->fileDialog->setOperationMode(KFileDialog::Opening);
    d->fileDialog->setMode(KFile::File);
    d->fileDialog->setKeepLocation(true);
    int result = d->fileDialog->exec();
    if (result == KFileDialog::Ok)
    {
        open(d->fileDialog->selectedUrl());
    }
}

void PhotoFramesEditor::open(const KUrl & fileUrl)
{
    if (fileUrl.isEmpty())
    {
        open();
        return;
    }
    else
    {
        // TODO : open file from url
        //createCanvas(document);
    }
}

void PhotoFramesEditor::save()
{
    // TODO : saving and testing unsaved changes
}

void PhotoFramesEditor::saveAs()
{
    if (!d->fileDialog)
        d->fileDialog = new KFileDialog(KUrl(), "*.pfe|Photo Frames Editor files", this);
    d->fileDialog->setOperationMode(KFileDialog::Saving);
    d->fileDialog->setMode(KFile::File);
    d->fileDialog->setKeepLocation(true);
    int result = d->fileDialog->exec();
    if (result == KFileDialog::Ok)
    {
        saveFile(d->fileDialog->selectedUrl());
    }
}

void PhotoFramesEditor::exportFile()
{
    // TODO : exporting to images
}

void PhotoFramesEditor::saveFile(const KUrl & /*fileUrl*/)
{
    // TODO : saving file
}

void PhotoFramesEditor::printPreview()
{
    // TODO : print preview
}

void PhotoFramesEditor::print()
{
    // TODO : print dialog
}

bool PhotoFramesEditor::closeDocument()
{
    if (m_canvas)
    {
        // TODO : check unsaved?
        //switch ( KMessageBox::warningYesNoCancel( this, i18n("Save changes to current frame?")) )
        {
            //   case KMessageBox::Yes:
            save();
            //    case KMessageBox::No:
            delete m_canvas;
            m_canvas = 0;
            return true;
            //    default:
            return false;
        }
    }
    refreshActions();
    return true;
}

bool PhotoFramesEditor::queryClose()
{
    if (closeDocument())
        return true;
    else
        return false;
}

void PhotoFramesEditor::setGridVisible(bool isVisible)
{
    if (m_canvas_widget)
        m_canvas_widget->setGridVisible(isVisible);
}

void PhotoFramesEditor::setupGrid()
{

}
