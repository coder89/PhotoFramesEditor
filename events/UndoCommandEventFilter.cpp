#include "UndoCommandEventFilter.h"
#include "UndoCommandEvent.h"
#include "global.h"
#include <QtGlobal>

using namespace KIPIPhotoFramesEditor;

bool UndoCommandEventFilter::eventFilter(QObject * watched, QEvent * event)
{
    if (event->type() == UndoCommandEvent::registeredEventType())
    {
        PhotoFramesEditor * editor = qobject_cast<PhotoFramesEditor*>(watched);
        if (editor)
        {
            editor->undoCommandEvent(dynamic_cast<UndoCommandEvent*>(event));
            return true;
        }
        else
            return false;
    }
    return QObject::eventFilter(watched, event);
}
