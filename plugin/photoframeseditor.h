#ifndef PHOTOFRAMESEDITOR_H
#define PHOTOFRAMESEDITOR_H

// Qt
#include <QUndoStack>

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
            virtual void undoCommandEvent(UndoCommandEvent * event);

        public Q_SLOTS:

            void open();
            void openDialog();
            void open(const KUrl & fileUrl);
            void save();
            void saveAs();
            void exportFile();
            void saveFile(const KUrl & fileUrl);
            void printPreview();
            void print();
            bool closeDocument();
            void setGridVisible(bool isVisible);
            void createCanvas(const QSizeF & dimension);
            void setupGrid();

        protected Q_SLOTS:

            bool queryClose();

        private:

            explicit PhotoFramesEditor(QWidget * parent = 0);
            static PhotoFramesEditor * m_instance;

            void setupActions();
            void refreshActions();
            void createWidgets();
            void loadEffects();

            Canvas * m_canvas;

            class PhotoFramesEditorPriv;
            PhotoFramesEditorPriv * const d;
    };

}

#endif // PHOTOFRAMESEDITOR_H
