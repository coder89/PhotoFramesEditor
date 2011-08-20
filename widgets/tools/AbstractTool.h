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
            Q_OBJECT

            Scene * m_scene;

        public:

            AbstractTool(Scene * scene, QWidget * parent = 0) :
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
                if (m_scene == scene)
                    return;
                this->sceneChange();
                this->m_scene = scene;
                if (scene)
                {
                    connect(m_scene, SIGNAL(destroyed()), this, SLOT(sceneDestroyed()));
                    this->setEnabled(true);
                }
                else
                    this->setEnabled(false);
                this->sceneChanged();
            }

            virtual void sceneChange()
            {}

            virtual void sceneChanged()
            {}

        protected slots:

            void sceneDestroyed()
            {
                if (sender() == m_scene)
                    this->setScene(0);
            }

        friend class ToolsDockWidget;
    };
}

#endif // ABSTRACTTOOL_H
