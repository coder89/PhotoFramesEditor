#include "ToolsDockWidget.h"

#include <QDebug>
#include <QButtonGroup>
#include <QGridLayout>

#include <kicon.h>

using namespace KIPIPhotoFramesEditor;

ToolsDockWidget::ToolsDockWidget(QWidget * parent) :
    QDockWidget("Tools",parent)
{
    this->setFeatures(QDockWidget::DockWidgetMovable);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QWidget * widget = new QWidget(this);
    QGridLayout * layout = new QGridLayout(widget);
    layout->addItem(new QSpacerItem(24,24,QSizePolicy::Expanding),0,0);
    layout->addItem(new QSpacerItem(24,24,QSizePolicy::Expanding),1,0);
    QButtonGroup * group = new QButtonGroup(widget);

    // Selection tool
    m_tool_pointer = new KPushButton(KIcon(":tool_pointer.png"), "", widget);
    m_tool_pointer->setFixedSize(32,32);
    m_tool_pointer->setCheckable(true);
    group->addButton(m_tool_pointer);
    layout->addWidget(m_tool_pointer, 0,1, Qt::AlignCenter);
    connect(m_tool_pointer,SIGNAL(toggled(bool)),this,SLOT(emitPointerToolSelected(bool)));

    // View tool
    m_tool_hand = new KPushButton(KIcon(":tool_hand.png"), "", widget);
    m_tool_hand->setFixedSize(32,32);
    m_tool_hand->setCheckable(true);
    group->addButton(m_tool_hand);
    layout->addWidget(m_tool_hand, 0,2, Qt::AlignCenter);
    connect(m_tool_hand,SIGNAL(toggled(bool)),this,SLOT(emitHandToolSelected(bool)));

    // Border edit tool
    m_tool_border = new KPushButton(KIcon(":tool_border.png"), "", widget);
    m_tool_border->setFixedSize(32,32);
    m_tool_border->setCheckable(true);
    group->addButton(m_tool_border);
    layout->addWidget(m_tool_border, 1,1, Qt::AlignCenter);
    connect(m_tool_border,SIGNAL(toggled(bool)),this,SLOT(emitBorderToolSelected(bool)));

    // Spacer
    layout->addItem(new QSpacerItem(24,24,QSizePolicy::Expanding),0,5);
    layout->addItem(new QSpacerItem(24,24,QSizePolicy::Expanding),1,5);
    layout->setColumnStretch(5,1);
    layout->setColumnStretch(5,1);
    layout->setSpacing(0);

    widget->setLayout(layout);
    widget->setMaximumHeight(layout->minimumSize().height());
    this->setWidget(widget);

    setDefaultTool();
}

/// Default tool selection
void ToolsDockWidget::setDefaultTool()
{
    m_tool_hand->setChecked(true);
    this->emitHandToolSelected(true);
}
