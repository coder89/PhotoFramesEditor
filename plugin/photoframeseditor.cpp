// Local
#include "photoframeseditor.h"
#include "photoframeseditor_p.h"
#include "CanvasCreationDialog.h"
#include "Canvas.h"
#include "Scene.h"
#include "LayersSelectionModel.h"
#include "UndoCommandEventFilter.h"
#include "PhotoEffectsLoader.h"
#include "AbstractPhotoEffectFactory.h"
#include "ImageFileDialog.h"
#include "GridSetupDialog.h"
#include "global.h"
#include "PFEConfigSkeleton.h"
#include "PFEConfigViewWidget.h"

#include "BorderDrawerInterface.h"
#include "BorderDrawersLoader.h"

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
#include <QFile>
#include <QPrintPreviewDialog>
#include <QImageWriter>
#include <QPrinter>
#include <QPrintDialog>

// KDE
#include <kmenubar.h>
#include <kstandardaction.h>
#include <kactioncollection.h>
#include <kfiledialog.h>
#include <ktip.h>
#include <kaboutdata.h>
#include <kmessagebox.h>
#include <kapplication.h>
#include <kprintpreview.h>
#include <kconfigdialog.h>

using namespace KIPIPhotoFramesEditor;

PhotoFramesEditor * PhotoFramesEditor::m_instance = 0;

PhotoFramesEditor::PhotoFramesEditor(QWidget * parent) :
    KXmlGuiWindow(parent),
    m_canvas(0),
    m_interface(0),
    d(new PhotoFramesEditorPriv)
{
    setXMLFile(QApplication::applicationDirPath()+"/resources/photoframeseditorui.rc");

    setObjectName("Photo Frames Editor");

    m_instance = this;
    d->settings = PFEConfig::instance(this);

    loadEffects();
    loadBorders();
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

PhotoFramesEditor * PhotoFramesEditor::instance(QWidget * parent)
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

void PhotoFramesEditor::addUndoCommand(QUndoCommand * command)
{
    if (command)
    {
        if (m_canvas)
            m_canvas->undoStack()->push(command);
        else
        {
            command->redo();
            delete command;
        }
    }
}

void PhotoFramesEditor::setInterface(KIPI::Interface * interface)
{
    if (interface)
        m_interface = interface;
}

bool PhotoFramesEditor::hasInterface() const
{
    return (bool) m_interface;
}

void PhotoFramesEditor::setupActions()
{
    d->openNewFileAction = KStandardAction::openNew(this, SLOT(open()), actionCollection());
    d->openNewFileAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_N));
    actionCollection()->addAction("open_new", d->openNewFileAction);
    //------------------------------------------------------------------------
    d->openFileAction = KStandardAction::open(this, SLOT(openDialog()), actionCollection());
    d->openFileAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_O));
    actionCollection()->addAction("open", d->openFileAction);
    //------------------------------------------------------------------------
    d->openRecentFilesMenu = KStandardAction::openRecent(this, SLOT(open(const KUrl &)), actionCollection());
    actionCollection()->addAction("open_recent", d->openRecentFilesMenu);
    //------------------------------------------------------------------------
    d->saveAction = KStandardAction::save(this, SLOT(save()), actionCollection());
    d->saveAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_S));
    actionCollection()->addAction("save", d->saveAction);
    //------------------------------------------------------------------------
    d->saveAsAction = KStandardAction::saveAs(this, SLOT(saveAs()), actionCollection());
    d->saveAsAction->setShortcut(KShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_S));
    actionCollection()->addAction("save_as", d->saveAsAction);
    //------------------------------------------------------------------------
    d->exportFileAction = new KAction(i18nc("Export current frame layout to image file", "Export"), actionCollection());
    d->exportFileAction->setShortcut(KShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_E));
    connect(d->exportFileAction, SIGNAL(triggered()), this, SLOT(exportFile()));
    actionCollection()->addAction("export", d->exportFileAction);
    //------------------------------------------------------------------------
    d->printPreviewAction = KStandardAction::printPreview(this, SLOT(printPreview()), actionCollection());
    d->printPreviewAction->setShortcut(KShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_P));
    actionCollection()->addAction("print_preview", d->printPreviewAction);
    //------------------------------------------------------------------------
    d->printAction = KStandardAction::print(this, SLOT(print()), actionCollection());
    d->printAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_P));
    actionCollection()->addAction("print", d->printAction);
    //------------------------------------------------------------------------
    d->closeAction = KStandardAction::close(this, SLOT(closeDocument()), actionCollection());
    d->closeAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_Q));
    actionCollection()->addAction("close", d->closeAction);
    //------------------------------------------------------------------------
    d->quitAction = KStandardAction::quit(this, SLOT(close()), actionCollection());
    d->quitAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_Q));
    actionCollection()->addAction("quit", d->quitAction);
    //------------------------------------------------------------------------
    d->undoAction = KStandardAction::undo(0, 0, actionCollection());
    d->undoAction->setShortcut(KShortcut(Qt::CTRL + Qt::Key_Z));
    actionCollection()->addAction("undo", d->undoAction);
    //------------------------------------------------------------------------
    d->redoAction = KStandardAction::redo(0, 0, actionCollection());
    d->redoAction->setShortcut(KShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Z));
    actionCollection()->addAction("redo", d->redoAction);
    //------------------------------------------------------------------------
    d->settingsAction = KStandardAction::preferences(this, SLOT(settings()), actionCollection());
    actionCollection()->addAction("settings", d->settingsAction);
    //------------------------------------------------------------------------
    d->showGridToggleAction = new KToggleAction(i18nc("View grid lines", "Show"), actionCollection());
    d->showGridToggleAction->setShortcut(KShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_G));
    connect(d->showGridToggleAction, SIGNAL(triggered(bool)), this, SLOT(setGridVisible(bool)));
    actionCollection()->addAction("grid_toggle", d->showGridToggleAction);
    //------------------------------------------------------------------------
    d->gridConfigAction = new KAction(i18nc("Configure grid lines visibility", "Setup grid"), actionCollection());
    connect(d->gridConfigAction, SIGNAL(triggered()), this, SLOT(setupGrid()));
    actionCollection()->addAction("grid_config", d->gridConfigAction);
    //------------------------------------------------------------------------
    d->changeCanvasSizeAction = new KAction(i18nc("Configure canvas size", "Change canvas size"), actionCollection());
    connect(d->changeCanvasSizeAction, SIGNAL(triggered()), this, SLOT(changeCanvasSize()));
    actionCollection()->addAction("canvas_size", d->changeCanvasSizeAction);

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
    d->saveAction->setEnabled(isEnabledForCanvas && !m_canvas->isSaved());
    d->saveAsAction->setEnabled(isEnabledForCanvas);
    d->exportFileAction->setEnabled(isEnabledForCanvas);
    d->printPreviewAction->setEnabled(isEnabledForCanvas);
    d->printAction->setEnabled(isEnabledForCanvas);
    d->closeAction->setEnabled(isEnabledForCanvas);
    d->showGridToggleAction->setEnabled(isEnabledForCanvas);
    d->gridConfigAction->setEnabled(isEnabledForCanvas);
    d->changeCanvasSizeAction->setEnabled(isEnabledForCanvas);
    d->treeWidget->setEnabled(isEnabledForCanvas);
    d->toolsWidget->setEnabled(isEnabledForCanvas);
}

void PhotoFramesEditor::createWidgets()
{
    // Tools
    d->toolsWidget = new ToolsDockWidget(this);
    this->addDockWidget(Qt::RightDockWidgetArea, d->toolsWidget);

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

    this->open(KUrl("/home/coder89/Desktop/first.pfe"));   /// TODO : REMOVE WHEN FINISH
}

void PhotoFramesEditor::createCanvas(const QSize & dimension, const QSizeF & paperSize, SizeUnits sizeUnits)
{
    if (m_canvas)
    {
        d->centralWidget->layout()->removeWidget(m_canvas);
        m_canvas->deleteLater();
    }
    m_canvas = new Canvas(dimension, paperSize, sizeUnits, d->centralWidget);
    this->prepareSignalsConnections();
}

void PhotoFramesEditor::createCanvas(const KUrl & fileUrl)
{
    if (m_canvas)
    {
        d->centralWidget->layout()->removeWidget(m_canvas);
        m_canvas->deleteLater();
    }
    QFile file(fileUrl.path());
    QDomDocument document;
    document.setContent(&file, true);
    m_canvas = Canvas::fromSvg(document);
    m_canvas->setFile(fileUrl);
    if (m_canvas)
    {
        m_canvas->setParent(d->centralWidget);
        this->prepareSignalsConnections();
    }
    else
    {
        KMessageBox::error(this,
                           i18n("Can't read image file."));
    }
    m_canvas->undoStack()->clear();
}

void PhotoFramesEditor::prepareSignalsConnections()
{
    d->centralWidget->layout()->addWidget(m_canvas);
    d->tree->setModel(m_canvas->model());
    d->tree->setSelectionModel(m_canvas->selectionModel());
    d->toolsWidget->setScene(m_canvas->scene());

    // undo stack signals
    connect(m_canvas,               SIGNAL(savedStateChanged()),    this,                   SLOT(refreshActions()));
    connect(m_canvas->undoStack(),  SIGNAL(canRedoChanged(bool)),   d->redoAction,          SLOT(setEnabled(bool)));
    connect(m_canvas->undoStack(),  SIGNAL(canUndoChanged(bool)),   d->undoAction,          SLOT(setEnabled(bool)));
    connect(d->undoAction,          SIGNAL(triggered()),            m_canvas->undoStack(),  SLOT(undo()));
    connect(d->redoAction,          SIGNAL(triggered()),            m_canvas->undoStack(),  SLOT(redo()));

    // model/tree/canvas synchronization signals
    connect(d->tree,    SIGNAL(selectedRowsAboutToBeRemoved()),     m_canvas,   SLOT(removeSelectedRows()));
    connect(d->tree,    SIGNAL(selectedRowsAboutToBeMovedUp()),     m_canvas,   SLOT(moveSelectedRowsUp()));
    connect(d->tree,    SIGNAL(selectedRowsAboutToBeMovedDown()),   m_canvas,   SLOT(moveSelectedRowsDown()));
    connect(d->treeTitle->moveUpButton(),   SIGNAL(clicked()),      m_canvas,   SLOT(moveSelectedRowsUp()));
    connect(d->treeTitle->moveDownButton(), SIGNAL(clicked()),      m_canvas,   SLOT(moveSelectedRowsDown()));
    // interaction modes (tools)
    connect(m_canvas,       SIGNAL(selectedItem(AbstractPhoto*)),       d->toolsWidget,SLOT(itemSelected(AbstractPhoto*)));
    connect(d->toolsWidget, SIGNAL(undoCommandCreated(QUndoCommand*)),  m_canvas,   SLOT(newUndoCommand(QUndoCommand*)));
    connect(d->toolsWidget, SIGNAL(pointerToolSelected()),              m_canvas,   SLOT(enableDefaultSelectionMode()));
    connect(d->toolsWidget, SIGNAL(handToolSelected()),                 m_canvas,   SLOT(enableViewingMode()));
    connect(d->toolsWidget, SIGNAL(canvasToolSelected()),               m_canvas,   SLOT(enableCanvasEditingMode()));
    connect(d->toolsWidget, SIGNAL(effectsToolSelected()),              m_canvas,   SLOT(enableEffectsEditingMode()));
    connect(d->toolsWidget, SIGNAL(textToolSelected()),                 m_canvas,   SLOT(enableTextEditingMode()));
    connect(d->toolsWidget, SIGNAL(rotateToolSelected()),               m_canvas,   SLOT(enableRotateEditingMode()));
    connect(d->toolsWidget, SIGNAL(scaleToolSelected()),                m_canvas,   SLOT(enableScaleEditingMode()));
    connect(d->toolsWidget, SIGNAL(cropToolSelected()),                 m_canvas,   SLOT(enableCropEditingMode()));
    connect(d->toolsWidget, SIGNAL(borderToolSelected()),               m_canvas,   SLOT(enableBordersEditingMode()));
    connect(d->toolsWidget, SIGNAL(newItemCreated(AbstractPhoto*)),     m_canvas,   SLOT(addNewItem(AbstractPhoto*)));
    connect(m_canvas->scene()->toGraphicsScene(), SIGNAL(mousePressedPoint(QPointF)), d->toolsWidget, SLOT(mousePositionChoosen(QPointF)));

    d->toolsWidget->setDefaultTool();
}

void PhotoFramesEditor::open()
{
    closeDocument();

    CanvasCreationDialog * newCanvas = new CanvasCreationDialog(this);
    newCanvas->setModal(true);
    int result = newCanvas->exec();

    if (result == KDialog::Accepted)
    {
        createCanvas(newCanvas->canvasSize(),
                     newCanvas->paperSize(),
                     newCanvas->sizeUnits());
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
    if (result == KFileDialog::Accepted)
        open(d->fileDialog->selectedUrl());
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
        closeDocument();
        createCanvas(fileUrl);
        refreshActions();
    }
}

void PhotoFramesEditor::save()
{
    if (!m_canvas)
        return;
    if (m_canvas->file().fileName().isEmpty())
        saveAs();
    else
        saveFile();
}

void PhotoFramesEditor::saveAs()
{
    if (!d->fileDialog)
        d->fileDialog = new KFileDialog(KUrl(), "*.pfe|Photo Frames Editor files", this);
    d->fileDialog->setOperationMode(KFileDialog::Saving);
    d->fileDialog->setMode(KFile::File);
    d->fileDialog->setKeepLocation(true);
    int result = d->fileDialog->exec();
    if (result == KFileDialog::Accepted)
    {
        KUrl url = d->fileDialog->selectedUrl();
        saveFile(url);
    }
}

void PhotoFramesEditor::saveFile(const KUrl & fileUrl, bool setFileAsDefault)
{
    if (m_canvas)
    {
        QString error = m_canvas->save(fileUrl, setFileAsDefault);
        if (!error.isEmpty())
        {
            KMessageBox::detailedError(this,
                                       i18n("Can't save canvas to file!"),
                                       error);
        }
    }
    else
        KMessageBox::error(this,
                           i18n("There is nothing to save!"));
}

void PhotoFramesEditor::exportFile()
{
    if (!m_canvas)
        return;
    ImageFileDialog imageDialog(KUrl(), this);
    imageDialog.setOperationMode(KFileDialog::Saving);
    int result = imageDialog.exec();
    if (result == KFileDialog::Accepted)
    {
        const char * format = imageDialog.format();
        if (format)
        {
            QPixmap image(m_canvas->sceneRect().size().toSize());
            image.fill(Qt::transparent);
            m_canvas->renderCanvas(&image);
            QImageWriter writer(imageDialog.selectedFile());
            writer.setFormat(format);
            if (!writer.canWrite())
            {
                KMessageBox::error(this,
                                   i18n("Image can't be saved in selected file."));
            }
            if (!writer.write(image.toImage()))
            {
                KMessageBox::detailedError(this,
                                   i18n("Unexpected error while saving an image!"),
                                   writer.errorString());
            }
        }
    }
}

void PhotoFramesEditor::printPreview()
{
     if (m_canvas && m_canvas->scene())
     {
         QPrinter printer;
         printer.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
         SizeUnits su = m_canvas->sizeUnits();
         QSizeF cs = m_canvas->paperSize();
         switch (su)
         {
         case Pixels:
             printer.setPaperSize(cs, QPrinter::DevicePixel);
             break;
         case Meters:
             cs *= 100;
         case Centimeters:
             cs *= 10;
         case Milimeters:
             printer.setPaperSize(cs, QPrinter::Millimeter);
             break;
         case Yards:
             cs *= 3;
         case Feet:
             cs *= 12;
         case Inches:
             printer.setPaperSize(cs, QPrinter::Inch);
             break;
         case Points:
             printer.setPaperSize(cs, QPrinter::Point);
             break;
         case Picas:
             printer.setPaperSize(cs, QPrinter::Pica);
             break;
         default:
#ifdef QT_DEBUG
             qDebug() << "Unhandled size unit at:" << __FILE__ << ":" << __LINE__;
#endif
         }
         QPrintPreviewDialog dialog(&printer, this);
         connect(&dialog, SIGNAL(paintRequested(QPrinter*)), m_canvas, SLOT(renderCanvas(QPrinter*)));
         dialog.exec();
     }
}

void PhotoFramesEditor::print()
{
    QPrintDialog dialog(this);
    connect(&dialog, SIGNAL(accepted(QPrinter*)), m_canvas, SLOT(renderCanvas(QPrinter*)));
    dialog.exec();
}

bool PhotoFramesEditor::closeDocument()
{
    if (m_canvas)
    {
        int saving = KMessageBox::No;
        if (!m_canvas->isSaved())
            saving = KMessageBox::warningYesNoCancel( this, i18n("Save changes to current frame?"));
        switch (saving)
        {
            case KMessageBox::Yes:
                save();
            case KMessageBox::No:
                d->tree->setModel(0);
                m_canvas->deleteLater();
                m_canvas = 0;
                refreshActions();
                return true;
            default:
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

void PhotoFramesEditor::settings()
{
    if ( KConfigDialog::showDialog( "settings" ) )
      return;

    KConfigDialog * dialog = new KConfigDialog(this, "settings", PFEConfig::configSkeleton());
    PFEConfigViewWidget * confWdg = new PFEConfigViewWidget( 0, i18n("View") );
    dialog->addPage( confWdg, i18n("View"), "view" );
    connect( dialog, SIGNAL(settingsChanged()), this, SLOT(updateConfiguration()) );
    dialog->show();
}

void PhotoFramesEditor::setGridVisible(bool isVisible)
{
    if (m_canvas && m_canvas->scene())
        m_canvas->scene()->setGridVisible(isVisible);
}

void PhotoFramesEditor::setupGrid()
{
    if (m_canvas && m_canvas->scene())
    {
        GridSetupDialog dialog(this);
        dialog.setHorizontalDistance( m_canvas->scene()->gridHorizontalDistance() );
        dialog.setVerticalDistance( m_canvas->scene()->gridVerticalDistance() );
        dialog.exec();
        m_canvas->scene()->setGrid(dialog.horizontalDistance(),
                                   dialog.verticalDistance());
    }
}

void PhotoFramesEditor::changeCanvasSize()
{
    if (!m_canvas)
        return;
    CanvasCreationDialog ccd(m_canvas->paperSize(), m_canvas->sizeUnits(), m_canvas->resolution(), m_canvas->resolutionUnits(), this);
    int result = ccd.exec();
    if (result == KDialog::Accepted)
    {
        m_canvas->setCanvasSize(ccd.canvasSize());
        m_canvas->setPageSize(ccd.paperSize(), ccd.sizeUnits());
        m_canvas->setCanvasResolution(ccd.paperResolution(), ccd.resolutionUnits());
    }
}

void PhotoFramesEditor::loadEffects()
{
    QDir effectsDir("./effects");
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
}

void PhotoFramesEditor::loadBorders()
{
    QDir bordersDir("./borders");
    QStringList filters;
    filters << "*.so" << "*.dll";
    QFileInfoList filesList = bordersDir.entryInfoList(filters, QDir::Files);
    foreach (QFileInfo fileInfo, filesList)
    {
        QPluginLoader loader(fileInfo.absoluteFilePath());
        QObject * plugin = loader.instance();
        if (plugin)
        {
            BorderDrawerFactoryInterface * newBorderFactory = qobject_cast<BorderDrawerFactoryInterface*>(plugin);
            newBorderFactory->getDrawerInstance();
            if (newBorderFactory)
            {
                BorderDrawersLoader::registerDrawer(newBorderFactory);
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
}
