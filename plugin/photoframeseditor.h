#ifndef PHOTOFRAMESEDITOR_H
#define PHOTOFRAMESEDITOR_H

// Qt
#include <QUndoStack>
#include <QDebug>

// KDE
#include <kxmlguiwindow.h>
#include <kurl.h>

// Local
#include "CanvasWidget.h"
#include "UndoCommandEvent.h"

namespace KIPIPhotoFramesEditor
{
    class Canvas;

    class PhotoFramesEditor : public KXmlGuiWindow
    {
            Q_OBJECT

        public:

            ~PhotoFramesEditor();
            static PhotoFramesEditor * instancePhotoFramesEditor(QWidget * parent = 0);
            void addUndoCommand(QUndoCommand * command);

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
            void createCanvas(const QSizeF & dimension);
            void createCanvas(const KUrl & fileUrl);
            void setupGrid();

        protected Q_SLOTS:

            bool queryClose();
            void refreshActions();

        private:

            explicit PhotoFramesEditor(QWidget * parent = 0);
            static PhotoFramesEditor * m_instance;

            void setupActions();
            void createWidgets();
            void loadEffects();
            void loadBorders();
            void prepareSignalsConnections();

            Canvas * m_canvas;

            class PhotoFramesEditorPriv;
            PhotoFramesEditorPriv * const d;
    };

}

#endif // PHOTOFRAMESEDITOR_H
