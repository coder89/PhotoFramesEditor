#ifndef QGRAPHICSEDITIONWIDGET_H
#define QGRAPHICSEDITIONWIDGET_H

#include <QGraphicsWidget>

namespace KIPIPhotoFramesEditor
{
    class QGraphicsRotationItem;
    class QGraphicsSelectionItem;

    class QGraphicsEditionWidget : public QGraphicsWidget
    {
            Q_OBJECT

            QGraphicsRotationItem * m_rot;
            QGraphicsSelectionItem * m_sel;

        public:

            explicit QGraphicsEditionWidget(QGraphicsItem * parent = 0);

        signals:

        public slots:

    };
}

#endif // QGRAPHICSEDITIONWIDGET_H
