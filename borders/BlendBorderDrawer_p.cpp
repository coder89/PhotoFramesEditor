#include "BlendBorderDrawer_p.h"
#include "BlendBorderDrawer.h"

#include <klocalizedstring.h>

#include <QPainter>
#include <QPaintEngine>
#include <QMetaProperty>
#include <QDebug>

void blurred(QImage & image, const QRect& rect, unsigned int radius)
{
    int tab[] = { 14, 10, 8, 6, 5, 5, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2 };
    int alpha = (radius < 1)  ? 16 : (radius > sizeof(tab)) ? 1 : tab[radius-1];

    int r1 = rect.top();
    int r2 = rect.bottom();
    int c1 = rect.left();
    int c2 = rect.right();

    int bpl = image.bytesPerLine();
    int rgba[4];
    unsigned char * p;

    for (int col = c1; col <= c2; col++)
    {
        p = image.scanLine(r1) + col * 4;
        for (int i = 0; i < 4; i++)
            rgba[i] = p[i] << 4;

        p += bpl;
        for (int j = r1; j < r2; j++, p += bpl)
            for (int i = 0; i < 4; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    for (int row = r1; row <= r2; row++)
    {
        p = image.scanLine(row) + c1 * 4;
        for (int i = 0; i < 4; i++)
            rgba[i] = p[i] << 4;

        p += 4;
        for (int j = c1; j < c2; j++, p += 4)
            for (int i = 0; i < 4; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    for (int col = c1; col <= c2; col++)
    {
        p = image.scanLine(r2) + col * 4;
        for (int i = 0; i < 4; i++)
            rgba[i] = p[i] << 4;

        p -= bpl;
        for (int j = r1; j < r2; j++, p -= bpl)
            for (int i = 0; i < 4; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    for (int row = r1; row <= r2; row++)
    {
        p = image.scanLine(row) + c2 * 4;
        for (int i = 0; i < 4; i++)
            rgba[i] = p[i] << 4;

        p -= 4;
        for (int j = c1; j < c2; j++, p -= 4)
            for (int i = 0; i < 4; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }
}

QMap<const char *,QString> BlendBorderDrawer::m_properties;
QMap<Qt::PenJoinStyle, QString> BlendBorderDrawer::m_corners_style_names;
int BlendBorderDrawer::m_default_width = 10;
Qt::PenJoinStyle BlendBorderDrawer::m_default_corners_style = Qt::MiterJoin;

BlendBorderDrawer::BlendBorderDrawer(BlendBorderDrawerFactory * factory, QObject * parent) :
    BorderDrawerInterface(factory, parent),
    m_width(m_default_width),
    m_corners_style(m_default_corners_style)
{
    if (m_corners_style_names.isEmpty())
    {
        BlendBorderDrawer::m_corners_style_names.insert(Qt::MiterJoin, "Miter");
        BlendBorderDrawer::m_corners_style_names.insert(Qt::BevelJoin, "Bevel");
        BlendBorderDrawer::m_corners_style_names.insert(Qt::RoundJoin, "Round");
    }

    if (m_properties.isEmpty())
    {
        const QMetaObject * meta = this->metaObject();
        int count = meta->propertyCount();
        while (count--)
        {
            QMetaProperty property = meta->property(count);
            if (!QString("corners_style").compare(property.name()))
                m_properties.insert(property.name(), i18n("Corners style"));
            else if (!QString("width").compare(property.name()))
                m_properties.insert(property.name(), i18n("Width"));
        }
    }
}

QPainterPath BlendBorderDrawer::path(const QPainterPath & path)
{
    QPainterPath temp = path;
    QPainterPathStroker stroker;
    stroker.setWidth(m_width/2.0);
    stroker.setJoinStyle(Qt::MiterJoin);
    temp -= stroker.createStroke(temp);
    stroker.setJoinStyle(this->m_corners_style);
    temp += stroker.createStroke(temp);
    m_pixmap = QImage(path.boundingRect().size().toSize(),QImage::Format_ARGB32_Premultiplied);
    m_pixmap.fill(QColor(255,255,255,0).rgba());
    QPainter p(&m_pixmap);
    p.setPen(QPen(Qt::black,1));
    p.drawPath(path);
    blurred(m_pixmap, m_pixmap.rect(), m_width);
    return (m_path = temp);
}
int com = -1;
void BlendBorderDrawer::paint(QPainter * painter, const QStyleOptionGraphicsItem * /*option*/)
{
    if (m_path.isEmpty())
        return;
    painter->save();
    ++com;
    if (com > 32)
        com = 0;
    painter->setCompositionMode((QPainter::CompositionMode)com);
    qDebug() << "composition: " << com;
    painter->drawImage(m_path.boundingRect().topLeft(), m_pixmap);
    painter->restore();
}

QString BlendBorderDrawer::propertyName(const QMetaProperty & property) const
{
    return m_properties.value(property.name());
}

QVariant BlendBorderDrawer::propertyValue(const QString & propertyName) const
{
    const QMetaObject * meta = this->metaObject();
    int index = meta->indexOfProperty( m_properties.key(propertyName) );
    if (index >= meta->propertyCount())
        return QVariant();
    return meta->property( index ).read(this);
}

void BlendBorderDrawer::setPropertyValue(const QString & propertyName, const QVariant & value)
{
    const QMetaObject * meta = this->metaObject();
    int index = meta->indexOfProperty( m_properties.key(propertyName) );
    if (index >= meta->propertyCount())
        return;
    meta->property( index ).write(this, value);
}

QDomElement BlendBorderDrawer::toSvg(QDomDocument & document) const
{
    QDomElement result = document.createElement("border");
    return result;
}

QString BlendBorderDrawer::toString() const
{
    return factory()->drawerName().append(" [Width: ") + QString::number(m_width).append("]");
}

BlendBorderDrawer::operator QString() const
{
    return this->toString();
}

QVariant BlendBorderDrawer::stringNames(const QMetaProperty & property)
{
    const char * name = property.name();
    if (!QString("corners_style").compare(name))
        return QVariant(m_corners_style_names.values());
    return QVariant();
}

QVariant BlendBorderDrawer::minimumValue(const QMetaProperty & property)
{
    const char * name = property.name();
    if (!QString("width").compare(name))
        return 0;
    return QVariant();
}

QVariant BlendBorderDrawer::maximumValue(const QMetaProperty & property)
{
    const char * name = property.name();
    if (!QString("width").compare(name))
        return 100;
    return QVariant();
}

QVariant BlendBorderDrawer::stepValue(const QMetaProperty & /*property*/)
{
    return QVariant();
}
