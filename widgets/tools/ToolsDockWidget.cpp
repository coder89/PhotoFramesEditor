#include "ToolsDockWidget.h"
#include "ColorizeTool.h"
#include "EffectsEditorTool.h"
#include "TextEditorTool.h"

#include <QDebug>
#include <QButtonGroup>
#include <QGridLayout>
#include <QPropertyAnimation>

#include <kicon.h>
#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

class MyStackedLayout : public QStackedLayout
{
    public:

        MyStackedLayout(QWidget * parent = 0) : QStackedLayout(parent) {}

        virtual QSize sizeHint() const
        {
            QSize s = QStackedLayout::sizeHint();
            s.setHeight(this->currentWidget()->sizeHint().height());
            return s;
        }

        virtual QSize minimumSize() const
        {
            return sizeHint();
        }
};

ToolsDockWidget::ToolsDockWidget(QWidget * parent) :
    QDockWidget("Tools",parent),
    m_has_selection(false),
    m_currentPhoto(0)
{
    this->setFeatures(QDockWidget::DockWidgetMovable);
    this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QWidget * widget = new QWidget(this);
    QVBoxLayout * layout = new QVBoxLayout(widget);
    layout->setSizeConstraint(QLayout::SetMinimumSize);

    // tools buttons layout
    QGridLayout * formLayout = new QGridLayout();
    formLayout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->addLayout(formLayout);

    // stacked widget (with tools widgets)
    m_tool_widget_layout = new MyStackedLayout();
    m_tool_widget_layout->addWidget(new QWidget(widget));
    layout->addLayout(m_tool_widget_layout,1);

    formLayout->addItem(new QSpacerItem(24,24,QSizePolicy::Expanding),0,0);
    formLayout->addItem(new QSpacerItem(24,24,QSizePolicy::Expanding),1,0);
    QButtonGroup * group = new QButtonGroup(widget);

    // Selection tool

    m_tool_pointer = new KPushButton(KGuiItem("",":pointer.png",
                                              i18n("Allows to select and move images on canvas"),
                                              i18n("Tool which allows to select and move images on canvas. Any other operations are disabled.")), widget);
    m_tool_pointer->setIconSize(QSize(24,24));
    m_tool_pointer->setFixedSize(32,32);
    m_tool_pointer->setCheckable(true);
    group->addButton(m_tool_pointer);
    formLayout->addWidget(m_tool_pointer, 0,1, Qt::AlignCenter);
    connect(m_tool_pointer,SIGNAL(toggled(bool)),this,SLOT(emitPointerToolSelected(bool)));

    // View tool
    m_tool_hand = new KPushButton(KGuiItem("",":hand.png",
                                           i18n("Viewing tool"),
                                           i18n("This tool allows to view whole canvas in read-only mode. Only scrolling and zooming are available.")), widget);
    m_tool_hand->setIconSize(QSize(24,24));
    m_tool_hand->setFixedSize(32,32);
    m_tool_hand->setCheckable(true);
    group->addButton(m_tool_hand);
    formLayout->addWidget(m_tool_hand, 0,2, Qt::AlignCenter);
    connect(m_tool_hand,SIGNAL(toggled(bool)),this,SLOT(emitHandToolSelected(bool)));

    // Effects tool
    m_effects_button = new KPushButton(KGuiItem("", ":tool_effects.png",
                                              i18n("Effects editor"),
                                              i18n("This tool allows you to edit existing effects of your layers and add some new one.")), widget);
    m_effects_button->setIconSize(QSize(24,24));
    m_effects_button->setFixedSize(32,32);
    m_effects_button->setCheckable(true);
    group->addButton(m_effects_button);
    formLayout->addWidget(m_effects_button, 0,3, Qt::AlignCenter);
    m_effects_widget = new EffectsEditorTool(this);
    m_tool_widget_layout->addWidget(m_effects_widget);
    connect(m_effects_button,SIGNAL(toggled(bool)),this,SLOT(setEffectsWidgetVisible(bool)));

    // Text tool
    m_text_button = new KPushButton(KGuiItem("", ":tool_text.png",
                                              i18n("Text editor"),
                                              i18n("This tool allows you to write text on the canvas. It's simple - just click on the canvas where you want to add some text and write it!")), widget);
    m_text_button->setIconSize(QSize(24,24));
    m_text_button->setFixedSize(32,32);
    m_text_button->setCheckable(true);
    group->addButton(m_text_button);
    formLayout->addWidget(m_text_button, 0,4, Qt::AlignCenter);
    m_text_widget = new TextEditorTool(this);
    m_tool_widget_layout->addWidget(m_text_widget);
    connect(m_text_button,SIGNAL(toggled(bool)),this,SLOT(setTextWidgetVisible(bool)));
    connect(m_text_widget, SIGNAL(itemCreated(AbstractPhoto*)), this, SLOT(emitNewItemCreated(AbstractPhoto*)));

    // Border edit tool
    m_tool_border = new KPushButton(KIcon(":tool_border.png"), "", widget);
    m_tool_border->setIconSize(QSize(24,24));
    m_tool_border->setFixedSize(32,32);
    m_tool_border->setCheckable(true);
    group->addButton(m_tool_border);
    formLayout->addWidget(m_tool_border, 1,1, Qt::AlignCenter);
    connect(m_tool_border,SIGNAL(toggled(bool)),this,SLOT(emitBorderToolSelected(bool)));

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
    widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setWidget(widget);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    setDefaultTool();
}

/// Default tool selection
void ToolsDockWidget::setDefaultTool()
{
    qDebug() << "setDefaultTool";
    m_tool_hand->setChecked(true);
    this->emitHandToolSelected(true);
}

void ToolsDockWidget::itemSelected(AbstractPhoto * photo)
{
    qDebug() << "itemSelected" << photo;
    m_currentPhoto = photo;
    AbstractTool * tool =qobject_cast<AbstractTool*>(m_tool_widget_layout->currentWidget());
    if (tool)
        tool->setCurrentItem(photo);
}

void ToolsDockWidget::mousePositionChoosen(const QPointF & position)
{
    AbstractTool * tool =qobject_cast<AbstractTool*>(m_tool_widget_layout->currentWidget());
    if (tool)
        tool->setMousePosition(position);
}

void ToolsDockWidget::emitNewItemCreated(AbstractPhoto * item)
{
    if (!item)
        return;
    emit newItemCreated(item);
}

void ToolsDockWidget::setEffectsWidgetVisible(bool isVisible)
{
    emit effectsToolSelectionChanged(isVisible);
    if (isVisible)
    {
        m_effects_widget->setCurrentItem(m_currentPhoto);
        m_tool_widget_layout->setCurrentWidget(m_effects_widget);
        emit requireSingleSelection();
        emit effectsToolSelected();
        this->adjustSize();
    }
}

void ToolsDockWidget::setTextWidgetVisible(bool isVisible)
{
    emit textToolSelectionChanged(isVisible);
    if (isVisible)
    {
        m_text_widget->setCurrentItem(m_currentPhoto);
        m_tool_widget_layout->setCurrentWidget(m_text_widget);
        emit requireSingleSelection();
        emit textToolSelected();
        this->adjustSize();
    }
}
