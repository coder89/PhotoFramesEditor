#include "ToolsDockWidget.h"

#include <QDebug>
#include <QWidgetItem>
#include <QLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QGridLayout>
#include <QGroupBox>

using namespace KIPIPhotoFramesEditor;

ToolsDockWidget::ToolsDockWidget(QWidget * parent) :
    QDockWidget("Tools",parent)
{
    this->setFeatures(QDockWidget::DockWidgetMovable);

    QWidget * widget = new QWidget(this);
    QGridLayout * layout = new QGridLayout(widget);
    layout->addItem(new QSpacerItem(24,24,QSizePolicy::Expanding),0,0);
    QButtonGroup * group = new QButtonGroup(widget);

    // Selection tool
    m_tool_pointer = new QPushButton(QIcon(":tool_pointer.png"), "", widget);
    m_tool_pointer->setFixedSize(32,32);
    m_tool_pointer->setCheckable(true);
    group->addButton(m_tool_pointer);
    layout->addWidget(m_tool_pointer, 0,1, Qt::AlignCenter);
    connect(m_tool_pointer,SIGNAL(toggled(bool)),this,SLOT(emitPointerToolSelected(bool)));

    // View tool
    m_tool_hand = new QPushButton(QIcon(":tool_hand.gif"), "", widget);
    m_tool_hand->setFixedSize(32,32);
    m_tool_hand->setCheckable(true);
    m_tool_hand->setChecked(true);
    group->addButton(m_tool_hand);
    layout->addWidget(m_tool_hand, 0,2, Qt::AlignCenter);
    connect(m_tool_hand,SIGNAL(toggled(bool)),this,SLOT(emitHandToolSelected(bool)));

    // Spacer
    layout->addItem(new QSpacerItem(24,24,QSizePolicy::Expanding),0,3);
    layout->setColumnStretch(5,1);
    layout->setSpacing(0);

    widget->setLayout(layout);
    widget->setMaximumHeight(layout->minimumSize().height());
    this->setWidget(widget);
}
