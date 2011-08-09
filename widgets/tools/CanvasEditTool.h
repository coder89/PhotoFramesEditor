#ifndef CANVASEDITTOOL_H
#define CANVASEDITTOOL_H

#include <QWidget>

#include <kcombobox.h>

#include "AbstractTool.h"

namespace KIPIPhotoFramesEditor
{
    class CanvasEditToolPrivate;
    class CanvasEditTool : public AbstractTool
    {
            Q_OBJECT

            CanvasEditToolPrivate * d;

        public:

            explicit CanvasEditTool(Scene * scene, ToolsDockWidget * parent = 0);
            virtual ~CanvasEditTool();

        signals:

        public slots:

            void backgroundTypeChanged(const QString & typeName);

        protected slots:

            void colorBackgroundSelected();
            void gradientBackgroundSelected();
            void imageBackgroundSelected();
            void patternBackgroundSelected();
            void colorChanged(const QColor & color);
            void patternColorChanged(const QColor & color);
            void patternChanged(Qt::BrushStyle);

            void readMousePosition(const QPointF & scenePos);

        private:

            void setupGUI();
    };
}

#endif // CANVASEDITTOOL_H
