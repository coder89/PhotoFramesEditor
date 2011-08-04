#include "SolidBorderDrawer_p.h"
#include "SolidBorderDrawer.h"

#include <klocalizedstring.h>

#include <QPainter>
#include <QPaintEngine>
#include <QMetaProperty>
#include <QDebug>

QMap<const char *,QString> SolidBorderDrawer::m_properties;
QMap<Qt::PenJoinStyle, QString> SolidBorderDrawer::m_corners_style_names;
int SolidBorderDrawer::m_default_width = 1;
QColor SolidBorderDrawer::m_default_color = Qt::red;
int SolidBorderDrawer::m_default_spacing = 0;
Qt::PenJoinStyle SolidBorderDrawer::m_default_corners_style = Qt::MiterJoin;

SolidBorderDrawer::SolidBorderDrawer(SolidBorderDrawerFactory * factory, QObject * parent) :
    BorderDrawerInterface(factory, parent),
    m_width(m_default_width),
    m_color(m_default_color),
    m_spacing(m_default_spacing),
    m_corners_style(m_default_corners_style)
{
    if (m_corners_style_names.isEmpty())
    {
        SolidBorderDrawer::m_corners_style_names.insert(Qt::MiterJoin, "Miter");
        SolidBorderDrawer::m_corners_style_names.insert(Qt::BevelJoin, "Bevel");
        SolidBorderDrawer::m_corners_style_names.insert(Qt::RoundJoin, "Round");
    }

    if (m_properties.isEmpty())
    {
        const QMetaObject * meta = this->metaObject();
        int count = meta->propertyCount();
        while (count--)
        {
            QMetaProperty property = meta->property(count);
            if (!QString("color").compare(property.name()))
                m_properties.insert(property.name(), i18n("Color"));
            else if (!QString("corners_style").compare(property.name()))
                m_properties.insert(property.name(), i18n("Corners style"));
            else if (!QString("width").compare(property.name()))
                m_properties.insert(property.name(), i18n("Width"));
            else if (!QString("spacing").compare(property.name()))
                m_properties.insert(property.name(), i18n("Spacing"));
        }
    }
}

QPainterPath SolidBorderDrawer::path(const QPainterPath & path)
{
    QPainterPathStroker spacing;
    spacing.setWidth(m_spacing);
    spacing.setJoinStyle(Qt::MiterJoin);
    QPainterPathStroker stroker;
    stroker.setJoinStyle(this->m_corners_style);
    stroker.setWidth(m_width);
    m_path = stroker.createStroke( spacing.createStroke(path).united(path) );
    return m_path;
}

void SolidBorderDrawer::paint(QPainter * painter, const QStyleOptionGraphicsItem * /*option*/)
{
    if (m_path.isEmpty())
        return;
    painter->save();
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(m_path, m_color);
    painter->restore();
}

QString SolidBorderDrawer::propertyName(const QMetaProperty & property) const
{
    return m_properties.value(property.name());
}

QVariant SolidBorderDrawer::propertyValue(const QString & propertyName) const
{
    const QMetaObject * meta = this->metaObject();
    int index = meta->indexOfProperty( m_properties.key(propertyName) );
    if (index >= meta->propertyCount())
        return QVariant();
    return meta->property( index ).read(this);
}

void SolidBorderDrawer::setPropertyValue(const QString & propertyName, const QVariant & value)
{
    const QMetaObject * meta = this->metaObject();
    int index = meta->indexOfProperty( m_properties.key(propertyName) );
    if (index >= meta->propertyCount())
        return;
    meta->property( index ).write(this, value);
}

QDomElement SolidBorderDrawer::toSvg(QDomDocument & document) const
{
    QDomElement result = document.createElement("border");
    return result;
}

QString SolidBorderDrawer::toString() const
{
    return factory()->drawerName().append(" [") + QString::number(m_width).append(" ") + m_color.name().append("]");
}

SolidBorderDrawer::operator QString() const
{
    return this->toString();
}

QVariant SolidBorderDrawer::stringNames(const QMetaProperty & property)
{
    const char * name = property.name();
    if (!QString("corners_style").compare(name))
        return QVariant(m_corners_style_names.values());
    return QVariant();
}

QVariant SolidBorderDrawer::minimumValue(const QMetaProperty & property)
{
    const char * name = property.name();
    if (!QString("width").compare(name))
        return 0;
    if (!QString("spacing").compare(name))
        return -100;
    return QVariant();
}

QVariant SolidBorderDrawer::maximumValue(const QMetaProperty & property)
{
    const char * name = property.name();
    if (!QString("width").compare(name))
        return 100;
    if (!QString("spacing").compare(name))
        return 100;
    return QVariant();
}

QVariant SolidBorderDrawer::stepValue(const QMetaProperty & /*property*/)
{
    return QVariant();
}
