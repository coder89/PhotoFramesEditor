#ifndef ABSTRACTTOOL_H
#define ABSTRACTTOOL_H

#include <QWidget>

#include "Scene.h"
#include "ToolsDockWidget.h"

namespace KIPIPhotoFramesEditor
{
    class ToolsDockWidget;
    class AbstractTool : public QWidget
    {
            Scene * m_scene;

        public:

            AbstractTool(Scene * scene, ToolsDockWidget * parent = 0) :
                QWidget(parent),
                m_scene(scene)
            {}

            Scene * scene() const
            {
                return m_scene;
            }

        protected:

            void setScene(Scene * scene)
            {
                this->m_scene = scene;
                if (scene)
                    this->setEnabled(true);
                else
                    this->setEnabled(false);
            }

        friend class ToolsDockWidget;
    };
}

#endif // ABSTRACTTOOL_H
