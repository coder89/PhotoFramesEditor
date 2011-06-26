#ifndef SCENE_H
#define SCENE_H

// Qt
#include <QGraphicsView>
#include <QWheelEvent>
#include <QStandardItemModel>

namespace KIPIPhotoFramesEditor
{
    class Canvas;
    class AbstractPhoto;
    class WidgetsFactory;

    class CanvasWidget : public QGraphicsView
    {
            Q_OBJECT

        public:

            explicit CanvasWidget(QWidget *parent = 0);

            void setupGUI();
            void setCanvas(Canvas * document);
            void setGridVisible(bool visible);
            QAbstractItemModel * model();

        signals:

            void toggleGrid(bool);
            void selectionChanged(const QList<AbstractPhoto*>, const QList<AbstractPhoto*>);

        public slots:

        protected:

            virtual void dragEnterEvent(QDragEnterEvent * event);
            virtual void keyPressEvent(QKeyEvent * event);
            virtual void keyReleaseEvent(QKeyEvent * event);
            virtual void wheelEvent(QWheelEvent * event);

        protected Q_SLOTS:

            void documentClosed();
            void addItemToModel(AbstractPhoto * item);

        private:

            // Stack model
            QStandardItemModel * m_model;
            QSet<AbstractPhoto*> currentSelectedList;
            Canvas * m_canvas;

            // Widgets factory
            const WidgetsFactory & m_widgetsFactory;
    };

}

#endif // SCENE_H
