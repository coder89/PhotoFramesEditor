#ifndef PHOTOFRAMESEDITOR_H
#define PHOTOFRAMESEDITOR_H

// Qt
#include <QUndoStack>
#include <QDebug>

// KDE
#include <kxmlguiwindow.h>
#include <kurl.h>

// LibKIPI
#include <libkipi/interface.h>

// Local
#include "CanvasWidget.h"
#include "UndoCommandEvent.h"
#include "CanvasSize.h"

namespace KIPIPhotoFramesEditor
{
    class Canvas;
    class CanvasSizeChangeCommand;

    class PhotoFramesEditor : public KXmlGuiWindow
    {
            Q_OBJECT

        public:

            ~PhotoFramesEditor();
            static PhotoFramesEditor * instance(QWidget * parent = 0);
            void addUndoCommand(QUndoCommand * command);
            void beginUndoCommandGroup(const QString & name);
            void endUndoCommandGroup();
            void setInterface(KIPI::Interface * interface);
            bool hasInterface() const;
            KIPI::Interface * interface() const;

        public Q_SLOTS:

            void open();
            void openDialog();
            void open(const KUrl & fileUrl);
            void save();
            void saveAs();
            void saveFile(const KUrl & fileUrl = KUrl(), bool setFileAsDefault = true);
            void exportFile();
            void printPreview();
            void print();
            bool closeDocument();
            void setGridVisible(bool isVisible);
            void createCanvas(const CanvasSize & size);
            void createCanvas(const KUrl & fileUrl);
            void settings();
            void setupGrid();
            void changeCanvasSize();

        protected Q_SLOTS:

            bool queryClose();
            void refreshActions();
            void addRecentFile(const KUrl & url);
            void clearRecentList();

        private:

            explicit PhotoFramesEditor(QWidget * parent = 0);
            static PhotoFramesEditor * m_instance;

            void setupActions();
            void createWidgets();
            void loadEffects();
            void loadBorders();
            void prepareSignalsConnections();

            Canvas * m_canvas;
            KIPI::Interface * m_interface;

            class PhotoFramesEditorPriv;
            PhotoFramesEditorPriv * const d;
    };

}

#endif // PHOTOFRAMESEDITOR_H
