#include "BorderEditTool.h"

#include <QDebug>
#include <QGridLayout>
#include <QFormLayout>
#include <QLabel>
#include <QFont>
#include <QColorDialog>

#include <kpushbutton.h>
#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

BorderEditTool::BorderEditTool(QWidget * parent) :
    QDockWidget(parent)
{
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);
    QWidget * widget = new QWidget(this);
    QFormLayout * layout = new QFormLayout(widget);

    m_width_widget = new QDoubleSpinBox(widget);
    layout->addRow(i18n("Width:"), m_width_widget);
    connect(m_width_widget,SIGNAL(valueChanged(double)),this,SLOT(widthChanged(double)));

    m_corners_style_widget = new KComboBox(this);
    m_corners_style_widget->addItem("Miter", Qt::MiterJoin);
    m_corners_style_widget->addItem("Round", Qt::RoundJoin);
    m_corners_style_widget->addItem("Bevel", Qt::BevelJoin);
    layout->addRow(i18n("Corners style:"), m_corners_style_widget);
    connect(m_corners_style_widget,SIGNAL(currentIndexChanged(int)),this,SLOT(cornerStyleChanged(int)));

    m_color_widget = new KColorButton(this);
    m_color_widget->setFlat(true);
    m_color_widget->setFocusPolicy(Qt::NoFocus);
    layout->addRow(i18n("Color:"), m_color_widget);
    connect(m_color_widget,SIGNAL(changed(QColor)),this,SLOT(colorChanged(QColor)));

    m_apply_button = new KPushButton(i18n("Apply"), widget);
    m_cancel_button = new KPushButton(i18n("Cancel"), widget);
    connect(m_apply_button,SIGNAL(clicked()),this,SLOT(emitBorderStyleChanged()));
    connect(m_cancel_button,SIGNAL(clicked()),this,SLOT(resetForm()));
    layout->addRow(m_apply_button,m_cancel_button);

    widget->setLayout(layout);
    this->setWidget(widget);

    QLabel * title = new QLabel(i18n("Borders"), this);
    QFont titleFont = title->font();
    titleFont.setBold(true);
    title->setFont(titleFont);
    this->setTitleBarWidget(title);
}

void BorderEditTool::setInitialValues(qreal width, Qt::PenJoinStyle cornersStyle, const QColor & color)
{
    m_apply_button->setEnabled(false);
    m_cancel_button->setEnabled(false);

    // Width selector
    m_width = width;
    m_width_widget->setValue(width);

    // Corner style selector
    m_corner_style = cornersStyle;
    for (int i = m_corners_style_widget->count()-1; i >= 0; --i)
        if (m_corners_style_widget->itemData(i) == cornersStyle)
            m_corners_style_widget->setCurrentIndex(i);

    m_color = color;
    m_color_widget->setColor(color);
}
