#include "CanvasEditTool.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QStackedLayout>
#include <QDebug>

#include <klocalizedstring.h>
#include <kcolorbutton.h>

#include "SceneBackground.h"

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
        m_parent(parent)
    {
        background_types.insert("Color",    ColorFill);
        background_types.insert("Image",    ImageFill);
        background_types.insert("Pattern",  PatternFill);
        background_types.insert("Gradient", GradientFill);
    }

    CanvasEditTool * m_parent;

    QMap<QString, BackgroundType> background_types;
    KComboBox * background_type_widget;
    QStackedLayout * background_widgets;

    QWidget * background_color_widget;
    KColorButton * background_color;

    QWidget * background_gradient_widget;

    QWidget * background_image_widget;

    QWidget * background_pattern_widget;

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
}

void CanvasEditTool::gradientBackgroundSelected()
{}

void CanvasEditTool::imageBackgroundSelected()
{}

void CanvasEditTool::patternBackgroundSelected()
{}

void CanvasEditTool::colorChanged(const QColor & color)
{
    Scene * scene = this->scene();
    if (!scene)
        return;
    scene->background()->setBrush( QBrush(color) );
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
        connect(d->background_type_widget, SIGNAL(currentIndexChanged(QString)), this, SLOT(backgroundTypeChanged(QString)));
        d->background_type_widget->addItems( d->background_types.keys() );
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

        // Gradient type widget
        // Image type widget
        // Pattern type widget
    }
}
