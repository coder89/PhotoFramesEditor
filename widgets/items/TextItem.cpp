#include "TextItem.h"

using namespace KIPIPhotoFramesEditor;

TextItem::TextItem(const QString & text, QGraphicsScene * scene) :
    AbstractPhoto(scene)
{
    this->setText(text);
    this->setFont(QFont());
    this->setColor(Qt::black);
}

QColor TextItem::color() const
{
    return m_color;
}

void TextItem::setColor(const QColor & color)
{
    m_color = color;
    this->refresh();
}

QFont TextItem::font() const
{
    return m_font;
}

void TextItem::setFont(const QFont & font)
{
    m_font = font;
    this->refresh();
}

QString TextItem::text() const
{
    return m_text;
}

void TextItem::setText(const QString & text)
{
    m_text = text;
    this->refresh();
}

QRectF TextItem::boundingRect() const
{
    return m_complete_path.boundingRect();
}

QPainterPath TextItem::shape() const
{
    return m_complete_path;
}

QPainterPath TextItem::opaqueArea() const
{
    return m_complete_path;
}

bool TextItem::contains(const QPointF & point) const
{
    return m_complete_path.contains(point);
}

void TextItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(m_text_path, m_color);
    painter->restore();
}

void TextItem::refresh()
{
    QPainterPath p;
    p.addText(0,0,m_font,m_text);
    p.translate(QPointF(0,p.boundingRect().height()));
    m_text_path = p;
    m_complete_path = QPainterPath();
    m_complete_path.addRect(m_text_path.boundingRect());
}

QDomNode TextItem::toSvg(QDomDocument & document) const
{
    QDomNode result = AbstractPhoto::toSvg(document);
    return result;
}

QDomElement TextItem::svgVisibleArea(QDomDocument & document) const
{
    QString d_attr = AbstractPhoto::pathToString(m_text_path);
    QDomElement element = document.createElement("path");
    element.setAttribute("d", d_attr);
    element.setAttribute("fill",QString("#")+QString::number(m_color.red(),16)+QString::number(m_color.green(),16)+QString::number(m_color.blue(),16));
    return element;
}

TextItem * TextItem::fromSvg(QDomElement & element)
{
    TextItem * result = new TextItem("");
    return result;
}
