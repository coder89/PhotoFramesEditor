#ifndef GLOBAL_H
#define GLOBAL_H

#define Q_DELETE(ptr)   if(ptr) { delete ptr; ptr = 0; }

class QUndoCommand;

namespace KIPIPhotoFramesEditor
{
    extern void PFE_PostUndoCommand(QUndoCommand * command);
}

#endif // GLOBAL_H
