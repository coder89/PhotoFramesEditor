/**
  *
  * In next version:
  *
  *     TODO : Gradient fill background:
  *               - create gradient editor widget
  *               - create MouseListener button
  *
  */

#include "CanvasEditTool.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QStackedLayout>
#include <QDebug>

#include <klocalizedstring.h>
#include <kcolorbutton.h>

#include "SceneBackground.h"
#include "MousePressListener.h"
#include "PatternsComboBox.h"

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::CanvasEditToolPrivate
{
    enum BackgroundType
    {
        ColorFill,
        PatternFill,
        GradientFill,
        ImageFill,
    };

    CanvasEditToolPrivate(CanvasEditTool * parent) :
        m_parent(parent),
        background_color_widget(0),
//        background_gradient_widget(0),
        background_image_widget(0),
        background_pattern_widget(0)
    {
        background_types.insert("Color",    ColorFill);
        background_types.insert("Image",    ImageFill);
        background_types.insert("Pattern",  PatternFill);
//        background_types.insert("Gradient", GradientFill);
    }

    CanvasEditTool * m_parent;

    QMap<QString, BackgroundType> background_types;
    KComboBox * background_type_widget;
    QStackedLayout * background_widgets;

    QWidget * background_color_widget;
    KColorButton * background_color;

//    QWidget * background_gradient_widget;

    QWidget * background_image_widget;

    QWidget * background_pattern_widget;
    KColorButton * background_pattern_color;
    PatternsComboBox * background_pattern_type;

    MousePressListener mouse_listener;

    friend class CanvasEditTool;
};

CanvasEditTool::CanvasEditTool(Scene * scene, ToolsDockWidget * parent) :
    AbstractTool(scene, parent),
    d(new CanvasEditToolPrivate(this))
{
    setupGUI();
}

CanvasEditTool::~CanvasEditTool()
{
    delete d;
}

void CanvasEditTool::backgroundTypeChanged(const QString & typeName)
{
    CanvasEditToolPrivate::BackgroundType bt = d->background_types.value(typeName);
    switch (bt)
    {
        case CanvasEditToolPrivate::ColorFill:
            colorBackgroundSelected();
            break;
        case CanvasEditToolPrivate::GradientFill:
            gradientBackgroundSelected();
            break;
        case CanvasEditToolPrivate::ImageFill:
            imageBackgroundSelected();
            break;
        case CanvasEditToolPrivate::PatternFill:
            patternBackgroundSelected();
            break;
    }
}

void CanvasEditTool::colorBackgroundSelected()
{
    d->background_widgets->setCurrentWidget(d->background_color_widget);
    SceneBackground * background = scene()->background();
    d->background_color->setColor(background->color());
}

void CanvasEditTool::gradientBackgroundSelected()
{}

void CanvasEditTool::imageBackgroundSelected()
{}

void CanvasEditTool::patternBackgroundSelected()
{
    d->background_widgets->setCurrentWidget(d->background_pattern_widget);
    SceneBackground * background = scene()->background();
    d->background_pattern_color->setColor(background->color());
}

void CanvasEditTool::colorChanged(const QColor & color)
{
    Scene * scene = this->scene();
    if (!scene)
        return;
    scene->background()->setBrush( QBrush(color) );
}

void CanvasEditTool::patternColorChanged(const QColor & color)
{
}

void CanvasEditTool::patternChanged(Qt::BrushStyle)
{

}

void CanvasEditTool::setupGUI()
{
    QVBoxLayout * layout = new QVBoxLayout();
    this->setLayout(layout);

    // Canvas background group
    QGroupBox * backgroundGroup = new QGroupBox(i18n("Background"), this);
    layout->addWidget(backgroundGroup);
    QFormLayout * backgroundLayout = new QFormLayout();
    backgroundGroup->setLayout(backgroundLayout);
    {
        // Background type widget
        d->background_type_widget = new KComboBox(backgroundGroup);
        d->background_type_widget->addItems( d->background_types.keys() );
        connect(d->background_type_widget, SIGNAL(currentIndexChanged(QString)), this, SLOT(backgroundTypeChanged(QString)));
        backgroundLayout->addRow(i18n("Type"), d->background_type_widget);

        d->background_widgets = new QStackedLayout();
        backgroundLayout->addRow(d->background_widgets);

        // Color type widget
        d->background_color_widget = new QWidget(backgroundGroup);
        QFormLayout * colorFormLayout = new QFormLayout();
        d->background_color_widget->setLayout(colorFormLayout);
        d->background_widgets->addWidget(d->background_color_widget);
        d->background_color = new KColorButton(Qt::white, d->background_color_widget);
        colorFormLayout->addRow(i18n("Color"), d->background_color);
        connect(d->background_color, SIGNAL(changed(QColor)), this, SLOT(colorChanged(QColor)));

//        // Gradient type widget
//        d->background_gradient_widget = new QWidget(backgroundGroup);
//        QFormLayout * gradientFromLayout = new QFormLayout();
//        d->background_gradient_widget->setLayout(gradientFromLayout);
//        d->background_widgets->addWidget(d->background_gradient_widget);

        // Image type widget
        // Pattern type widget
        d->background_pattern_widget = new QWidget(backgroundGroup);
        QFormLayout * patternFormLayout = new QFormLayout();
        d->background_pattern_widget->setLayout(patternFormLayout);
        d->background_widgets->addWidget(d->background_pattern_widget);
        d->background_pattern_color = new KColorButton(Qt::white, d->background_pattern_widget);
        patternFormLayout->addRow(i18n("Color"), d->background_pattern_color);
        connect(d->background_pattern_color, SIGNAL(changed(QColor)), this, SLOT(patternColorChanged(QColor)));
        d->background_pattern_type = new PatternsComboBox(d->background_pattern_widget);
        patternFormLayout->addRow(i18n("Pattern"), d->background_pattern_type);
        connect(d->background_pattern_type, SIGNAL(currentPatternChanged(Qt::BrushStyle)), this, SLOT(patternChanged(Qt::BrushStyle)));
    }

    if (scene())
    {
        Scene * scene = this->scene();
        SceneBackground * background = scene->background();
        if (background->isColor())
            this->colorBackgroundSelected();
        else if (background->isGradient())
            this->gradientBackgroundSelected();
        else if (background->isImage())
            this->imageBackgroundSelected();
        else if (background->isPattern())
            this->patternBackgroundSelected();
    }
    connect(&(d->mouse_listener), SIGNAL(mousePressed(QPointF)), this, SLOT(readMousePosition(QPointF)));
}

void CanvasEditTool::readMousePosition(const QPointF & scenePos)
{
    qDebug() << scenePos;
}
