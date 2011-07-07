#ifndef LAYERSTREETITLEWIDGET_H
#define LAYERSTREETITLEWIDGET_H

#include <QWidget>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

namespace KIPIPhotoFramesEditor
{
    class LayersTreeTitleWidget : public QWidget
    {
            QHBoxLayout * m_layout;
            QLabel * m_label;
            QPushButton * m_up_btn;
            QPushButton * m_dwn_btn;

        public:

            LayersTreeTitleWidget (QWidget * parent = 0) :
                QWidget(parent),
                m_layout(new QHBoxLayout(this)),
                m_label(new QLabel("Layers",this)),
                m_up_btn(new QPushButton(QIcon(":arrow_top.png"), "", this)),
                m_dwn_btn(new QPushButton(QIcon(":arrow_down.png"), "", this))
            {
                m_up_btn->setGeometry(QRect(0,0,20,20));

                m_layout->addWidget(m_label,1);
                m_layout->addWidget(m_up_btn);
                m_layout->addWidget(m_dwn_btn);
                this->setLayout(m_layout);
                m_layout->setContentsMargins(0,0,0,0);
                m_layout->setMargin(0);
                m_layout->setSpacing(0);
                m_layout->update();
            }

            QAbstractButton * moveUpButton() const
            {
                return m_up_btn;
            }

            QAbstractButton * moveDownButton() const
            {
                return m_dwn_btn;
            }
    };
}

#endif // LAYERSTREETITLEWIDGET_H
