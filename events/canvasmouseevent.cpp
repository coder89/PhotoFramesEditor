#include "canvasmouseevent.h"

namespace KIPIPhotoFramesEditor
{
    class CanvasMouseEventPrivate
    {

        public:
            inline CanvasMouseEventPrivate() :
                q_ptr(0)
            {}

            Q_DECLARE_PUBLIC(CanvasMouseEvent);

            QGraphicsSceneMouseEvent * q_ptr;
    };

    CanvasMouseEvent::CanvasMouseEvent(Type type) :
        QGraphicsSceneMouseEvent(type),
        d_ptr(new CanvasMouseEventPrivate)
    {
        d_ptr->q_ptr = this;
    }

    CanvasMouseEvent::CanvasMouseEvent(CanvasType type) :
        QGraphicsSceneMouseEvent((Type)type),
        d_ptr(new CanvasMouseEventPrivate)
    {
        d_ptr->q_ptr = this;
    }

    CanvasMouseEvent::CanvasMouseEvent(CanvasMouseEventPrivate &dd, Type type) :
        QGraphicsSceneMouseEvent(type),
        d_ptr(&dd)
    {
        d_ptr->q_ptr = this;
        Q_D(CanvasMouseEvent);
        CanvasMouseEvent * q = d_ptr->q_func();
        q->setAccepted(dd.q_ptr->isAccepted());
        q->setButton(dd.q_ptr->button());
        q->setButtonDownPos(q->button(), dd.q_ptr->buttonDownPos(q->button()));
        q->setButtonDownScenePos(q->button(), dd.q_ptr->buttonDownScenePos(q->button()));
        q->setButtonDownScreenPos(q->button(), dd.q_ptr->buttonDownScreenPos(q->button()));
        q->setButtons(dd.q_ptr->buttons());
        q->setLastPos(dd.q_ptr->lastPos());
        q->setLastScenePos(dd.q_ptr->lastScenePos());
        q->setLastScreenPos(dd.q_ptr->lastScreenPos());
        q->setModifiers(dd.q_ptr->modifiers());
        q->setPos(dd.q_ptr->pos());
        q->setScenePos(dd.q_ptr->scenePos());
        q->setScreenPos(dd.q_ptr->screenPos());
        q->setWidget(dd.q_ptr->widget());
    }

    CanvasMouseEvent::CanvasMouseEvent(CanvasMouseEventPrivate &dd, CanvasType type)
    {
        CanvasMouseEvent(dd, (Type)type);
    }

    CanvasMouseEvent::~CanvasMouseEvent()
    {
        delete d_ptr;
    }
}
