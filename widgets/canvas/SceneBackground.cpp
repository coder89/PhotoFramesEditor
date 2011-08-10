#include "SceneBackground.h"
#include "global.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QUndoCommand>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QPixmap>
#include <QBuffer>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::SceneBackground::BackgroundFirstColorChangedCommand : public QUndoCommand
{
    QColor m_first_color;
    SceneBackground * m_backgropund_item;
public:
    BackgroundFirstColorChangedCommand(const QColor & color, SceneBackground * backgroundItem, QUndoCommand * parent = 0) :
        QUndoCommand(i18n("Background changed"), parent),
        m_first_color(color),
        m_backgropund_item(backgroundItem)
    {}
    virtual void redo()
    {
        run();
    }
    virtual void undo()
    {
        run();
    }
    void run()
    {
        QColor temp = m_backgropund_item->m_first_brush.color();
        m_backgropund_item->m_first_brush.setColor(m_first_color);
        m_first_color = temp;

        m_backgropund_item->emitFirstColorChanged();
        m_backgropund_item->render();
        m_backgropund_item->update();
    }
};
class KIPIPhotoFramesEditor::SceneBackground::BackgroundSecondColorChangedCommand : public QUndoCommand
{
    QColor m_second_color;
    Qt::BrushStyle m_second_style;
    SceneBackground * m_backgropund_item;
public:
    BackgroundSecondColorChangedCommand(const QColor & color, SceneBackground * backgroundItem, QUndoCommand * parent = 0) :
        QUndoCommand(i18n("Background changed"), parent),
        m_second_color(color),
        m_second_style(Qt::SolidPattern),
        m_backgropund_item(backgroundItem)
    {}
    virtual void redo()
    {
        run();
    }
    virtual void undo()
    {
        run();
    }
    void run()
    {
        QColor temp = m_backgropund_item->m_second_brush.color();
        m_backgropund_item->m_second_brush.setColor(m_second_color);
        m_second_color = temp;

        Qt::BrushStyle bstemp = m_backgropund_item->m_second_brush.style();
        m_backgropund_item->m_second_brush.setStyle(m_second_style);
        m_second_style = bstemp;

        m_backgropund_item->emitSecondColorChanged();
        m_backgropund_item->render();
        m_backgropund_item->update();
    }
};
class KIPIPhotoFramesEditor::SceneBackground::BackgroundPatternChangedCommand : public QUndoCommand
{
    Qt::BrushStyle pattern;
    SceneBackground * m_backgropund_item;
public:
    BackgroundPatternChangedCommand(Qt::BrushStyle patternStyle, SceneBackground * backgroundItem, QUndoCommand * parent = 0) :
        QUndoCommand(i18n("Background changed"), parent),
        pattern(patternStyle),
        m_backgropund_item(backgroundItem)
    {}
    virtual void redo()
    {
        run();
    }
    virtual void undo()
    {
        run();
    }
    void run()
    {
        Qt::BrushStyle temp = m_backgropund_item->m_first_brush.style();
        m_backgropund_item->m_first_brush.setStyle(pattern);
        pattern = temp;

        m_backgropund_item->emitPatternChanged();
        m_backgropund_item->render();
        m_backgropund_item->update();
    }
};

class KIPIPhotoFramesEditor::SceneBackground::BackgroundImageChangedCommand : public QUndoCommand
{
    QImage m_image;
    Qt::Alignment m_align;
    Qt::AspectRatioMode m_aspect_ratio;
    SceneBackground * m_backgropund_item;
    QSize m_size;
    bool m_repeat;
    QBrush m_first_brush;
public:
    BackgroundImageChangedCommand(const QImage & image, Qt::Alignment align, Qt::AspectRatioMode aspectRatio, bool repeat, SceneBackground * backgroundItem, QUndoCommand * parent = 0) :
        QUndoCommand(i18n("Background changed"), parent),
        m_image(image),
        m_align(align),
        m_aspect_ratio(aspectRatio),
        m_repeat(repeat),
        m_backgropund_item(backgroundItem)
    {
        QSize bgSize = backgroundItem->scene()->sceneRect().size().toSize();
        QImage temp = image.scaled(bgSize, aspectRatio, Qt::SmoothTransformation);
        m_size = temp.size();
        m_first_brush = QBrush(temp);
        this->align();
    }
    BackgroundImageChangedCommand(const QImage & image, Qt::Alignment align, const QSize & fixedSize, bool repeat, SceneBackground * backgroundItem, QUndoCommand * parent = 0) :
        QUndoCommand(i18n("Background changed"), parent),
        m_image(image),
        m_align(align),
        m_aspect_ratio(Qt::IgnoreAspectRatio),
        m_size(fixedSize),
        m_repeat(repeat),
        m_backgropund_item(backgroundItem)
    {
        m_first_brush = QBrush(image.scaled(fixedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        this->align();
    }
    virtual void redo()
    {
        run();
    }
    virtual void undo()
    {
        run();
    }
    void run()
    {
        QImage temp1 = m_backgropund_item->m_image;
        m_backgropund_item->m_image = m_image;
        m_image = temp1;

        Qt::Alignment temp2 = m_backgropund_item->m_image_align;
        m_backgropund_item->m_image_align = m_align;
        m_align = temp2;

        Qt::AspectRatioMode temp3 = m_backgropund_item->m_image_aspect_ratio;
        m_backgropund_item->m_image_aspect_ratio = m_aspect_ratio;
        m_aspect_ratio = temp3;

        QSize temp4 = m_backgropund_item->m_image_size;
        m_backgropund_item->m_image_size = m_size;
        m_size = temp4;

        bool temp5 = m_backgropund_item->m_image_repeat;
        m_backgropund_item->m_image_repeat = m_repeat;
        m_repeat = temp5;

        QBrush temp6 = m_backgropund_item->m_first_brush;
        m_backgropund_item->m_first_brush = m_first_brush;
        m_first_brush = temp6;

        m_backgropund_item->render();
        m_backgropund_item->update();
    }
    void align()
    {
        QSize bgSize = m_backgropund_item->scene()->sceneRect().size().toSize();
        QTransform t;
        qreal x = 0;
        if (m_align & Qt::AlignHCenter)
            x = (bgSize.width() - m_size.width()) / 2.0;
        else if (m_align & Qt::AlignRight)
            x = bgSize.width() - m_size.width();
        qreal y = 0;
        if (m_align & Qt::AlignVCenter)
            y = (bgSize.height() - m_size.height()) / 2.0;
        else if (m_align & Qt::AlignBottom)
            y = bgSize.height() - m_size.height();
        t.translate(x,y);
        m_first_brush.setTransform(t);
    }
};

SceneBackground::SceneBackground(QGraphicsScene * scene) :
    QGraphicsItem(0, scene),
    m_first_brush(Qt::transparent),
    m_second_brush(Qt::transparent)
{
    this->setZValue(-1.0/0.0);
    this->setFlags(0);
    this->sceneChanged();
}

QRectF SceneBackground::boundingRect() const
{
    return m_rect;
}

void SceneBackground::setFirstColor(const QColor & color)
{
    bool colorChanged = (m_first_brush.color() != color);
    bool patternChanged = (m_first_brush.style() != Qt::SolidPattern);

    QUndoCommand * parent = 0;
    QUndoCommand * command = 0;

    if (colorChanged && patternChanged)
        parent = new QUndoCommand(i18n("Background changed"));
    if (colorChanged)
        command = new BackgroundFirstColorChangedCommand(color, this, parent);
    if (patternChanged)
        command = new BackgroundPatternChangedCommand(Qt::SolidPattern, this, parent);

    if (parent)
        PFE_PostUndoCommand(parent);
    else if (command)
        PFE_PostUndoCommand(command);
}

void SceneBackground::setSecondColor(const QColor & color)
{
    bool colorChanged = (m_second_brush.color() != color);
    bool patternChanged = (m_second_brush.style() != Qt::SolidPattern);
    if (colorChanged || patternChanged)
    {
        QUndoCommand * command = new BackgroundSecondColorChangedCommand(color, this);
        PFE_PostUndoCommand(command);
    }
}

void SceneBackground::setPattern(const QColor & firstColor, const QColor & secondColor, Qt::BrushStyle patternStyle)
{
    bool color1Changed = (firstColor != m_first_brush.color());
    bool color2Changed = (secondColor != m_second_brush.color() || m_second_brush.style() != Qt::SolidPattern);
    bool patternChanged = (patternStyle != m_first_brush.style());
    int i = 0;
    QUndoCommand * parent = new QUndoCommand("Background changed");
    BackgroundFirstColorChangedCommand * command1 = 0;
    BackgroundSecondColorChangedCommand * command2 = 0;
    BackgroundPatternChangedCommand * command3 = 0;
    if (color1Changed)
        ++i;
    if (color2Changed)
        ++i;
    if (patternChanged)
        ++i;

    if (color1Changed)
        command1 = new BackgroundFirstColorChangedCommand(firstColor, this, i>1?parent:0);
    if (color2Changed)
        command2 = new BackgroundSecondColorChangedCommand(secondColor, this, i>1?parent:0);
    if (patternChanged)
        command3 = new BackgroundPatternChangedCommand(patternStyle, this, i>1?parent:0);

    if (i > 1)
        PFE_PostUndoCommand(parent);
    else
    {
        if (command1)
            PFE_PostUndoCommand(command1);
        if (command2)
            PFE_PostUndoCommand(command2);
        if (command3)
            PFE_PostUndoCommand(command3);
    }
}

void SceneBackground::setImage(const QImage & image, Qt::Alignment align, Qt::AspectRatioMode aspectRatio, bool repeat)
{
    BackgroundImageChangedCommand * command = new BackgroundImageChangedCommand(image, align, aspectRatio, repeat, this);
    PFE_PostUndoCommand(command);
}

void SceneBackground::setImage(const QImage & image, Qt::Alignment align, const QSize & fixedSize, bool repeat)
{
    BackgroundImageChangedCommand * command = new BackgroundImageChangedCommand(image, align, fixedSize, repeat, this);
    PFE_PostUndoCommand(command);
}

bool SceneBackground::isColor() const
{
    return m_first_brush.style() == Qt::SolidPattern;
}

bool SceneBackground::isGradient() const
{
    return m_first_brush.style() == Qt::LinearGradientPattern ||
           m_first_brush.style() == Qt::RadialGradientPattern ||
           m_first_brush.style() == Qt::ConicalGradientPattern;
}

bool SceneBackground::isImage() const
{
    return m_first_brush.style() == Qt::TexturePattern;
}

bool SceneBackground::isPattern() const
{
    return !(isColor() || isGradient() || isImage());
}

QDomElement SceneBackground::toSvg(QDomDocument & document) const
{
    QDomElement result = document.createElement("g");
    result.setAttribute("id", "background");
    if (this->isColor())
    {
        result.setAttribute("type", "color");
        QDomElement rect = document.createElement("rect");
        rect.setAttribute("width",  m_rect.width());
        rect.setAttribute("height", m_rect.height());
        rect.setAttribute("x", 0);
        rect.setAttribute("y", 0);
        rect.setAttribute("fill", m_first_brush.color().name());
        result.appendChild(rect);
    }
    else
    {
        QDomElement image = document.createElement("image");
        image.setAttribute("width",  m_pixmap.rect().width());
        image.setAttribute("height", m_pixmap.rect().height());
        image.setAttribute("x", 0);
        image.setAttribute("y", 0);
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        m_pixmap.save(&buffer, "PNG");
        image.setAttribute("xlink:href", QString("data:image/png;base64,") + byteArray.toBase64());
        result.appendChild(image);

        if (this->isGradient())
            result.setAttribute("type", "gradient");
        else if (this->isImage())
            result.setAttribute("type", "image");
        else
        {
            result.setAttribute("type", "pattern");
            result.setAttribute("brush_style", m_first_brush.style());
            result.setAttribute("color1", m_first_brush.color().name());
            result.setAttribute("color2", m_second_brush.color().name());
        }
    }

    return result;
}

bool SceneBackground::fromSvg(QDomElement & element)
{
    QDomNodeList list = element.childNodes();
    QDomElement background;
    for (int i = list.count()-1; i >= 0; --i)
    {
        if (!list.at(i).isElement())
            continue;
        background = list.at(i).toElement();
        if (background.attribute("id") != "background")
        {
            background = QDomElement();
            continue;
        }
    }
    if (background.isNull())
        return false;

    QString type = background.attribute("type");
    if (type == "color")
    {
        QColor color(background.attribute("fill"));
        if (!color.isValid())
            return false;
        m_first_brush.setColor(color);
    }
    else if (type == "pattern")
    {
        bool ok = true;
        Qt::BrushStyle bs = (Qt::BrushStyle) background.attribute("brush_style").toInt(&ok);
        QColor color1 = background.attribute("color1");
        QColor color2 = background.attribute("color2");
        if (!color1.isValid() || !color2.isValid() || !ok)
            return false;
        m_first_brush.setStyle(bs);
        m_first_brush.setColor(color1);
        m_second_brush.setColor(color2);
    }
    else if (type == "image")
    {
    }
    else if (type == "gradient")
    {}
    else
        return false;

    QPainter p(&m_pixmap);
    render(&p, m_pixmap.rect());

    return true;
}

QColor SceneBackground::firstColor() const
{
    return m_first_brush.color();
}

QColor SceneBackground::secondColor() const
{
    return m_second_brush.color();
}

Qt::BrushStyle SceneBackground::pattern() const
{
    return m_first_brush.style();
}

QImage SceneBackground::image() const
{
    return m_image;
}

Qt::Alignment SceneBackground::imageAlignment() const
{
    return m_image_align;
}

Qt::AspectRatioMode SceneBackground::imageAspectRatio() const
{
    return m_image_aspect_ratio;
}

QSize SceneBackground::imageSize() const
{
    return m_image_size;
}

bool SceneBackground::imageRepeated() const
{
    return m_image_repeat;
}

QVariant SceneBackground::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch(change)
    {
        case QGraphicsItem::ItemParentChange:
            return QVariant(0);
        case QGraphicsItem::ItemSceneHasChanged:
            sceneChanged();
        default:
            return QGraphicsItem::itemChange(change, value);
    }
}

void SceneBackground::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
    if (!m_rect.isValid())
        return;
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(QPoint(0,0), m_pixmap, option->exposedRect);
}

void SceneBackground::render()
{
    this->m_pixmap.fill(Qt::transparent);
    QPainter p(&this->m_pixmap);
    this->render(&p, this->m_pixmap.rect());
}

void SceneBackground::render(QPainter * painter, const QRect & rect)
{
    if (!rect.isValid())
        return;
    QRect r = rect;
    painter->save();
    if (this->isPattern())
        painter->fillRect(r, m_second_brush);
    else if (this->isImage() && !this->m_image_repeat)
        r = m_first_brush.transform().mapRect(QRect(0, 0, m_image_size.width(), m_image_size.height()));
    painter->fillRect(r, m_first_brush);
    painter->restore();
}

void SceneBackground::sceneChanged()
{
    if (scene())
    {
        m_rect = scene()->sceneRect();
        m_pixmap = QPixmap(m_rect.size().toSize());
        m_pixmap.fill(Qt::transparent);
        QPainter p(&m_pixmap);
        render(&p, m_rect.toRect());
    }
    else
        m_rect = QRectF();
}
