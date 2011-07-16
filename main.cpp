#include "photoframeseditor.h"

#include <QDesktopWidget>
#include <QResource>
#include <qmath.h>

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>

using namespace KIPIPhotoFramesEditor;

int main(int argc, char *argv[])
{

    KAboutData aboutData("photoframeseditor",
                         0,
                         ki18n("Photo Framed Editor"),
                         QString("").toUtf8(),
                         ki18n(""),
                         KAboutData::License_GPL,
                         ki18n(""),
                         ki18n(""),
                         QString("").toUtf8());

    KCmdLineArgs::init(argc,argv,&aboutData);

    KApplication a;

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
