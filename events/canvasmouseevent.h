#ifndef CANVASMOUSEEVENT_H
#define CANVASMOUSEEVENT_H

// Qt
#include <QGraphicsSceneMouseEvent>
#include <qcoreevent.h>
#include <qglobal.h>
#include <QScopedPointer>

namespace KIPIPhotoFramesEditor
{
    class CanvasMouseEventPrivate;

    class CanvasMouseEvent : public QGraphicsSceneMouseEvent
    {
        public:

            enum CanvasType
            {
                // Predefined values
                CanvasMouseMove = QEvent::User+QEvent::GraphicsSceneMouseMove,
                CanvasMousePress = QEvent::User+QEvent::GraphicsSceneMousePress,
                CanvasMouseRelease = QEvent::User+QEvent::GraphicsSceneMouseRelease,
                CanvasMouseDoubleClick = QEvent::User+QEvent::GraphicsSceneMouseDoubleClick,

                // Newly defined
                CanvasMouseDrawStart,
                CanvasMouseDrawMove,
                CanvasMouseDrawEnd
            };

            CanvasMouseEvent(Type type);
            CanvasMouseEvent(CanvasType type);
            ~CanvasMouseEvent();

        protected:

            CanvasMouseEvent(CanvasMouseEventPrivate &dd, Type type = None);
            CanvasMouseEvent(CanvasMouseEventPrivate &dd, CanvasType type);
            CanvasMouseEventPrivate * d_ptr;
            Q_DECLARE_PRIVATE(CanvasMouseEvent);

        private:
            Q_DISABLE_COPY(CanvasMouseEvent);
    };
}

#endif // CANVASMOUSEEVENT_H
