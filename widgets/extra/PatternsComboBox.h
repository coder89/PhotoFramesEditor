#ifndef PATTERNSCOMBOBOX_H
#define PATTERNSCOMBOBOX_H

#include <QComboBox>

namespace KIPIPhotoFramesEditor
{
    class PatternsComboBox : public QComboBox
    {
            Q_OBJECT

        public:

            explicit PatternsComboBox(QWidget * parent = 0);

        signals:

            void currentPatternChanged(Qt::BrushStyle pattern);

        protected:

            virtual void paintEvent(QPaintEvent *e);

        protected slots:

            void patternChanged(int index);

    };
}

#endif // PATTERNSCOMBOBOX_H
