#ifndef PFECONFIGDIALOG_H
#define PFECONFIGDIALOG_H

#include <kconfigdialog.h>

namespace KIPIPhotoFramesEditor
{
    class PFEConfigDialog : public KConfigDialog
    {
            Q_OBJECT

        public:

            explicit PFEConfigDialog(QWidget * parent = 0);
    };
}

#endif // PFECONFIGDIALOG_H
