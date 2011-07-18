#include "global.h"
#include "UndoCommandEvent.h"
#include "photoframeseditor.h"

#include <kapplication.h>

void KIPIPhotoFramesEditor::PFE_PostUndoCommand(QUndoCommand * command)
{
    UndoCommandEvent * event = new UndoCommandEvent();
    event->setUndoCommand(command);
    KApplication::postEvent(PhotoFramesEditor::instancePhotoFramesEditor(), event);
}
