#include "UndoCommandEvent.h"

using namespace KIPIPhotoFramesEditor;

UndoCommandEvent::UndoCommandEvent() :
    QEvent(registeredEventType()),
    m_command(0)
{
}
