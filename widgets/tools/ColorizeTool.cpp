#include "ColorizeTool.h"

#include <QGridLayout>
#include <QLabel>
#include <QGraphicsEffect>
#include <QPainter>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

ColorizeTool::ColorizeTool(QWidget * parent) :
    AbstractTool(parent)
{
    QGridLayout * layout = new QGridLayout(this);

    // Title
    QLabel * title = new QLabel(i18n("Colorize"), this);
    QFont titleFont = title->font();
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title,0,0,1,2);

    m_form_layout = new QFormLayout();

    m_colorize_type_widget = new KComboBox(this);
    m_colorize_type_widget->addItem("Solid",Solid);
    m_colorize_type_widget->addItem("Linear gradient",LinearGradient);
    m_colorize_type_widget->addItem("Radial gradient",RadialGradient);
    m_form_layout->addRow(i18n("Mode"),m_colorize_type_widget);
    connect(m_colorize_type_widget,SIGNAL(currentIndexChanged(int)),this,SLOT(modeChanged(int)));

    m_strength_slider = new QSlider(Qt::Horizontal, this);
    m_strength_slider->setMinimum(0);
    m_strength_slider->setMaximum(255);
    m_strength_spinbox = new QSpinBox(this);
    m_strength_spinbox->setMinimum(0);
    m_strength_spinbox->setMaximum(255);
    connect(m_strength_spinbox,SIGNAL(valueChanged(int)),m_strength_slider,SLOT(setValue(int)));
    connect(m_strength_slider,SIGNAL(valueChanged(int)),m_strength_spinbox,SLOT(setValue(int)));
    m_strength_slider->setValue(50);
    QHBoxLayout * strengthLayout = new QHBoxLayout();
    strengthLayout->addWidget(m_strength_slider);
    strengthLayout->addWidget(m_strength_spinbox);
    m_form_layout->addRow(i18n("Strength"),strengthLayout);

    m_color_button = new KColorButton(this);
    m_form_layout->addRow(i18n("Color"),m_color_button);

    m_color_button1 = new KColorButton(this);
    m_form_layout->addRow(i18n("Color1"),m_color_button1);

    m_color_button2 = new KColorButton(this);
    m_form_layout->addRow(i18n("Color2"),m_color_button2);

    layout->addLayout(m_form_layout,1,0,1,2);
    layout->setRowStretch(2,1);
    this->setLayout(layout);

    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
    m_colorize_type_widget->setCurrentIndex(0);
    this->modeChanged(0);
}

void ColorizeTool::modeChanged(int currentIndex)
{
    if ((ColorizeType)m_colorize_type_widget->itemData(currentIndex).toInt() == Solid)
    {
        m_color_button->setVisible(true);
        m_form_layout->labelForField(m_color_button)->setVisible(true);
        m_color_button1->setVisible(false);
        m_form_layout->labelForField(m_color_button1)->setVisible(false);
        m_color_button2->setVisible(false);
        m_form_layout->labelForField(m_color_button2)->setVisible(false);
    }
    else
    {
        m_color_button->setVisible(false);
        m_form_layout->labelForField(m_color_button)->setVisible(false);
        m_color_button1->setVisible(true);
        m_form_layout->labelForField(m_color_button1)->setVisible(true);
        m_color_button2->setVisible(true);
        m_form_layout->labelForField(m_color_button2)->setVisible(true);
    }
}
