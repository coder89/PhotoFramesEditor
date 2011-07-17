// Local
#include "photoframeseditor.h"
#include "photoframeseditor_p.h"
#include "CanvasCreationDialog.h"
#include "Canvas.h"
#include "LayersSelectionModel.h"
#include "UndoCommandEventFilter.h"
#include "PhotoEffectsLoader.h"
#include "AbstractPhotoEffectFactory.h"

// Qt
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QTreeView>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QDockWidget>
#include <QHeaderView>
#include <QLabel>
#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include <QPluginLoader>

// KDE
#include <kmenubar.h>
#include <kstandardaction.h>
#include <kactioncollection.h>
#include <kfiledialog.h>
#include <ktip.h>
#include <kaboutdata.h>
#include <kmessagebox.h>
#include <kapplication.h>

using namespace KIPIPhotoFramesEditor;

PhotoFramesEditor * PhotoFramesEditor::m_instance = 0;

PhotoFramesEditor::PhotoFramesEditor(QWidget *parent) :
    KXmlGuiWindow(parent),
    m_canvas(0),
    d(new PhotoFramesEditorPriv)
{
    setXMLFile(QApplication::applicationDirPath()+"/resources/photoframeseditorui.rc");

    setObjectName("Photo Frames Editor");

    m_instance = this;

    loadEffects();
    setupActions();
    createWidgets();
    refreshActions();

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
    {
        KApplication * app = KApplication::kApplication();
        app->installEventFilter(new UndoCommandEventFilter(app));
        return (m_instance = new PhotoFramesEditor(parent));
    }
}

void PhotoFramesEditor::undoCommandEvent(UndoCommandEvent * event)
{
    QUndoCommand * command = event->undoCommand();
    if (command && m_canvas)
        m_canvas->undoStack()->push(command);
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
    d->undoAction = KStandardAction::undo(0, 0, this);
    d->undoAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_Z));
    actionCollection()->addAction("undo", d->undoAction);
    //------------------------------------------------------------------------
    d->redoAction = KStandardAction::redo(0, 0, this);
    d->redoAction->setShortcut(KShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Z));
    actionCollection()->addAction("redo", d->redoAction);
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
    {
        isEnabledForCanvas = true;
        d->undoAction->setEnabled(m_canvas->undoStack()->canUndo());
        d->redoAction->setEnabled(m_canvas->undoStack()->canRedo());
    }
    d->saveAction->setEnabled(isEnabledForCanvas);
    d->saveAsAction->setEnabled(isEnabledForCanvas);
    d->exportFileAction->setEnabled(isEnabledForCanvas);
    d->printPreviewAction->setEnabled(isEnabledForCanvas);
    d->printAction->setEnabled(isEnabledForCanvas);
    d->closeAction->setEnabled(isEnabledForCanvas);
    d->gridActionMenu->setEnabled(isEnabledForCanvas);
    d->treeWidget->setEnabled(isEnabledForCanvas);
    d->toolsWidget->setDefaultTool();
    d->toolsWidget->setEnabled(isEnabledForCanvas);
}

void PhotoFramesEditor::createWidgets()
{
    // Tools
    d->toolsWidget = new ToolsDockWidget(this);
    this->addDockWidget(Qt::RightDockWidgetArea, d->toolsWidget);

//    // Borders tool
//    d->toolBorders = new BorderEditTool(this);
//    this->addDockWidget(Qt::RightDockWidgetArea, d->toolBorders);
//    d->toolBorders->setVisible(false);
//    connect(d->toolsWidget,SIGNAL(borderToolSelectionChanged(bool)),d->toolBorders,SLOT(setShown(bool)));

//    // Colorize tool
//    d->toolColorize = new ColorizeTool(this);
//    this->addDockWidget(Qt::RightDockWidgetArea, d->toolColorize);
//    d->toolColorize->setVisible(false);
//    connect(d->toolsWidget,SIGNAL(colorizeToolSelectionChanged(bool)),d->toolColorize,SLOT(setShown(bool)));

    // Layers dockwidget
    d->treeWidget = new QDockWidget("Layers", this);
    d->treeWidget->setFeatures(QDockWidget::DockWidgetMovable);
    d->treeWidget->setFloating(false);
    d->treeWidget->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    d->tree = new LayersTree(d->treeWidget);
    d->tree->setAnimated(true);
    d->treeWidget->setWidget(d->tree);
    d->treeTitle = new LayersTreeTitleWidget(d->treeTitle);
    d->treeWidget->setTitleBarWidget(d->treeTitle);
    this->addDockWidget(Qt::LeftDockWidgetArea, d->treeWidget);
    d->treeWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(d->toolsWidget,SIGNAL(requireMultiSelection()),d->tree,SLOT(setMultiSelection()));
    connect(d->toolsWidget,SIGNAL(requireSingleSelection()),d->tree,SLOT(setSingleSelection()));

    // Central widget (widget with canvas)
    d->centralWidget = new QWidget(this);
    d->centralWidget->setLayout(new QHBoxLayout(d->centralWidget));
    d->centralWidget->layout()->setSpacing(0);
    d->centralWidget->layout()->setMargin(0);
    this->setCentralWidget(d->centralWidget);

    createCanvas(QSizeF(800,800));                               /// TODO : REMOVE WHEN FINISH
}

void PhotoFramesEditor::createCanvas(const QSizeF & dimension)
{
    if (m_canvas)
    {
        d->centralWidget->layout()->removeWidget(m_canvas);
        m_canvas->deleteLater();
    }
    m_canvas = new Canvas(dimension, d->centralWidget);
    d->centralWidget->layout()->addWidget(m_canvas);
    d->tree->setModel(m_canvas->model());
    d->tree->setSelectionModel(m_canvas->selectionModel());
    for (int i = d->tree->model()->columnCount()-1; i >= 0; --i)
        d->tree->resizeColumnToContents(i);
    // model/tree/canvas synchronization signals
    connect(m_canvas->undoStack(),SIGNAL(canRedoChanged(bool)),d->redoAction,SLOT(setEnabled(bool)));
    connect(m_canvas->undoStack(),SIGNAL(canUndoChanged(bool)),d->undoAction,SLOT(setEnabled(bool)));
    connect(d->undoAction,SIGNAL(triggered()),m_canvas->undoStack(),SLOT(undo()));
    connect(d->redoAction,SIGNAL(triggered()),m_canvas->undoStack(),SLOT(redo()));
    connect(d->tree,SIGNAL(selectedRowsAboutToBeRemoved()),m_canvas,SLOT(removeSelectedRows()));
    connect(d->tree,SIGNAL(selectedRowsAboutToBeMovedUp()),m_canvas,SLOT(moveSelectedRowsUp()));
    connect(d->tree,SIGNAL(selectedRowsAboutToBeMovedDown()),m_canvas,SLOT(moveSelectedRowsDown()));
    connect(d->treeTitle->moveUpButton(),SIGNAL(clicked()),m_canvas,SLOT(moveSelectedRowsUp()));
    connect(d->treeTitle->moveDownButton(),SIGNAL(clicked()),m_canvas,SLOT(moveSelectedRowsDown()));
    // interaction modes (tools)
    connect(d->toolsWidget,SIGNAL(undoCommandCreated(QUndoCommand*)),m_canvas,SLOT(newUndoCommand(QUndoCommand*)));
    connect(m_canvas,SIGNAL(selectedItem(AbstractPhoto*)),d->toolsWidget,SLOT(itemSelected(AbstractPhoto*)));
    connect(d->toolsWidget,SIGNAL(pointerToolSelected()),m_canvas,SLOT(enableDefaultSelectionMode()));
    connect(d->toolsWidget,SIGNAL(handToolSelected()),m_canvas,SLOT(enableViewingMode()));
    connect(d->toolsWidget,SIGNAL(effectsToolSelected()),m_canvas,SLOT(enableBordersToolMode()));
    connect(d->toolsWidget,SIGNAL(borderToolSelected()),m_canvas,SLOT(enableBordersToolMode()));
    connect(d->toolsWidget,SIGNAL(colorizeToolSelected()),m_canvas,SLOT(enableColorizeToolMode()));
    // tools specific signals
//    connect(m_canvas,SIGNAL(setInitialValues(qreal,Qt::PenJoinStyle,QColor)),d->toolBorders,SLOT(setInitialValues(qreal,Qt::PenJoinStyle,QColor)));
//    connect(d->toolBorders,SIGNAL(borderStyleChanged(qreal,Qt::PenJoinStyle,QColor)),m_canvas,SLOT(borderChangeCommand(qreal,Qt::PenJoinStyle,QColor)));
//    connect(d->toolBorders,SIGNAL(visibilityChanged(bool)),m_canvas,SLOT(refreshWidgetConnections(bool)));
//    connect(d->toolColorize,SIGNAL(visibilityChanged(bool)),m_canvas,SLOT(refreshWidgetConnections(bool)));
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
            refreshActions();
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

void PhotoFramesEditor::setGridVisible(bool /*isVisible*/)
{
}

void PhotoFramesEditor::setupGrid()
{
}

void PhotoFramesEditor::loadEffects()
{
    QDir effectsDir("./effects");
    qDebug() << effectsDir.absolutePath();
    QStringList filters;
    filters << "*.so" << "*.dll";
    QFileInfoList filesList = effectsDir.entryInfoList(filters, QDir::Files);
    foreach (QFileInfo fileInfo, filesList)
    {
        QPluginLoader loader(fileInfo.absoluteFilePath());
        QObject * plugin = loader.instance();
        if (plugin)
        {
            AbstractPhotoEffectFactory * newEffectFactory = qobject_cast<AbstractPhotoEffectFactory*>(plugin);
            if (newEffectFactory)
            {
                PhotoEffectsLoader::registerEffect(newEffectFactory);
#ifdef QT_DEBUG
                qDebug() << "LOADED!";
#endif
            }
#ifdef QT_DEBUG
            else
                qDebug() << "Invalid class interface!";
#endif
        }
#ifdef QT_DEBUG
        else
            qDebug() << loader.errorString();
#endif
    }
    qDebug() << PhotoEffectsLoader::registeredEffectsNames();
}
