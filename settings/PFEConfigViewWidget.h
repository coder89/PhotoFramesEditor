#ifndef PFECONFIGVIEWWIDGET_H
#define PFECONFIGVIEWWIDGET_H

#include <QWidget>

namespace KIPIPhotoFramesEditor
{
    class PFEConfigViewWidgetPrivate;

    class PFEConfigViewWidget : public QWidget
    {
            Q_OBJECT

            PFEConfigViewWidgetPrivate * d;

        public:

            explicit PFEConfigViewWidget(QWidget * parent = 0, const QString & caption = QString());
            ~PFEConfigViewWidget();

        private:

            void setupGUI();


        friend class PFEConfigViewWidgetPrivate;
    };
}

#endif // PFECONFIGVIEWWIDGET_H
