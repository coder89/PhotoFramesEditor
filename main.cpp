#include "photoframeseditor.h"

#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <QResource>

using namespace KIPIPhotoFramesEditor;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QResource::registerResource("resources/icons.qcc");

    PhotoFramesEditor * w = PhotoFramesEditor::instancePhotoFramesEditor(0);
    w->setAttribute(Qt::WA_DeleteOnClose, true);
    w->resize(QApplication::desktop()->width()-500,QApplication::desktop()->height()-200);
    QDesktopWidget * d = a.desktop();
    w->move(d->rect().center()-w->frameGeometry().center());
    w->show();

    return a.exec();
    QResource::unregisterResource("resources/icons.qcc");
}
