#include "ToolsDockWidget.h"
#include "ColorizeTool.h"

#include <QDebug>
#include <QButtonGroup>
#include <QGridLayout>
#include <QPropertyAnimation>

#include <kicon.h>
#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

ToolsDockWidget::ToolsDockWidget(QWidget * parent) :
    QDockWidget("Tools",parent),
    m_has_selection(false)
{
    this->setFeatures(QDockWidget::DockWidgetMovable);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QWidget * widget = new QWidget(this);
    QVBoxLayout * layout = new QVBoxLayout(widget);
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    QGridLayout * formLayout = new QGridLayout();
    formLayout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->addLayout(formLayout);
    formLayout->addItem(new QSpacerItem(24,24,QSizePolicy::Expanding),0,0);
    formLayout->addItem(new QSpacerItem(24,24,QSizePolicy::Expanding),1,0);
    QButtonGroup * group = new QButtonGroup(widget);

    // Selection tool

    m_tool_pointer = new KPushButton(KGuiItem("",":tool_pointer.png",
                                              i18n("Allows to select and move images on canvas"),
                                              i18n("Tool which allows to select and move images on canvas. Any other operations are disabled.")), widget);
    m_tool_pointer->setIconSize(QSize(24,24));
    m_tool_pointer->setFixedSize(32,32);
    m_tool_pointer->setCheckable(true);
    group->addButton(m_tool_pointer);
    formLayout->addWidget(m_tool_pointer, 0,1, Qt::AlignCenter);
    connect(m_tool_pointer,SIGNAL(toggled(bool)),this,SLOT(emitPointerToolSelected(bool)));

    // View tool
    m_tool_hand = new KPushButton(KGuiItem("",":tool_hand.png",
                                           i18n("Viewing tool"),
                                           i18n("This tool allows to view whole canvas in read-only mode. Only scrolling and zooming are available.")), widget);
    m_tool_hand->setIconSize(QSize(24,24));
    m_tool_hand->setFixedSize(32,32);
    m_tool_hand->setCheckable(true);
    group->addButton(m_tool_hand);
    formLayout->addWidget(m_tool_hand, 0,2, Qt::AlignCenter);
    connect(m_tool_hand,SIGNAL(toggled(bool)),this,SLOT(emitHandToolSelected(bool)));

    // Colorize tool
    m_tool_effects = new KPushButton(KIcon(":tool_effects.png"), "", widget);
    m_tool_effects->setIconSize(QSize(24,24));
    m_tool_effects->setFixedSize(32,32);
    m_tool_effects->setCheckable(true);
    group->addButton(m_tool_effects);
    formLayout->addWidget(m_tool_effects, 1,1, Qt::AlignCenter);

    // Border edit tool
    m_tool_border = new KPushButton(KIcon(":tool_border.png"), "", widget);
    m_tool_border->setFixedSize(32,32);
    m_tool_border->setCheckable(true);
    group->addButton(m_tool_border);
    formLayout->addWidget(m_tool_border, 1,2, Qt::AlignCenter);
    connect(m_tool_border,SIGNAL(toggled(bool)),this,SLOT(emitBorderToolSelected(bool)));

    // Colorize tool
    m_tool_colorize_button = new KPushButton(KIcon(":tool_colorize.png"), "", widget);
    m_tool_colorize_button->setFixedSize(32,32);
    m_tool_colorize_button->setCheckable(true);
    group->addButton(m_tool_colorize_button);
    formLayout->addWidget(m_tool_colorize_button, 1,3, Qt::AlignCenter);
    connect(m_tool_colorize_button,SIGNAL(toggled(bool)),this,SLOT(emitColorizeToolSelected(bool)));

    // Spacer
    formLayout->addItem(new QSpacerItem(24,24,QSizePolicy::Expanding),0,5);
    formLayout->addItem(new QSpacerItem(24,24,QSizePolicy::Expanding),1,5);
    formLayout->setColumnStretch(5,1);
    formLayout->setColumnStretch(5,1);
    formLayout->setSpacing(0);
    formLayout->setMargin(0);

    layout->setSpacing(0);
    layout->setMargin(0);
    widget->setLayout(layout);
    widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    this->setWidget(widget);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored);

    setDefaultTool();
}

/// Default tool selection
void ToolsDockWidget::setDefaultTool()
{
    m_tool_hand->setChecked(true);
    this->emitHandToolSelected(true);
}
