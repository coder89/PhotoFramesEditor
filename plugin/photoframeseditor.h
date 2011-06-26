#ifndef PHOTOFRAMESEDITOR_H
#define PHOTOFRAMESEDITOR_H

// KDE
#include <kxmlguiwindow.h>
#include <kurl.h>

// Local
#include "CanvasWidget.h"

namespace KIPIPhotoFramesEditor
{
    class Canvas;

    class PhotoFramesEditor : public KXmlGuiWindow
    {
            Q_OBJECT

        public:

            ~PhotoFramesEditor();
            static PhotoFramesEditor * instancePhotoFramesEditor(QWidget * parent = 0);

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

            CanvasWidget * m_canvas_widget;
            Canvas * m_canvas;

            class PhotoFramesEditorPriv;
            PhotoFramesEditorPriv * const d;
    };

}

#endif // PHOTOFRAMESEDITOR_H
