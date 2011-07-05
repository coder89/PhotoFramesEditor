#include "photoframeseditor.h"

#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <QResource>
#include <qmath.h>

using namespace KIPIPhotoFramesEditor;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QResource::registerResource("resources/icons.qcc");

    PhotoFramesEditor * w = PhotoFramesEditor::instancePhotoFramesEditor(0);
    w->setAttribute(Qt::WA_DeleteOnClose, true);
    int height = QApplication::desktop()->height()-500;
    w->resize(round(height*16.0/9.0),height);
    QDesktopWidget * d = a.desktop();
    w->move(d->rect().center()-w->frameGeometry().center());
    w->show();

    return a.exec();
    QResource::unregisterResource("resources/icons.qcc");
}
