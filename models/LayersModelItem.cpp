#include "LayersModelItem.h"
#include "abstract_photo.h"

// Qt
#include <QVariant>
#include <QDebug>
#include <QIcon>

using namespace KIPIPhotoFramesEditor;

LayersModelItem::LayersModelItem(AbstractPhoto * photo, LayersModelItem * parent) :
    parentItem(0),
    itemPhoto(photo)
{
    if (parent != this)
    {
        this->parentItem = parent;
        if (parent)
            parent->childItems.append(this);
    }

    qDebug() << this;
//    QList<QVariant> data;
//    data << "sfa" << QIcon(":eye.png") << QIcon(":padlock.png") << QString().append("Name=").append(QString::number(QTime::currentTime().msec()));
//    LayersModelItem::setData(data);
}

LayersModelItem::LayersModelItem() :
    parentItem(0),
    itemPhoto(0)
{
    qDebug() << this;
}

LayersModelItem::~LayersModelItem()
{
    //qDebug() << "DELETE ITEM!";
    foreach (LayersModelItem * child, childItems)
        delete child;
}

void LayersModelItem::removeChild(LayersModelItem * child)
{
    if (child && child->parent() == this)
    {
        //qDebug() << "removeChild" << "OK";
        child->setParent(0);
    }
}

LayersModelItem * LayersModelItem::parent() const
{
    //qDebug() << "parent" << parentItem;
    return parentItem;
}

int LayersModelItem::row() const
{
    if (parentItem)
    {
        //qDebug() << "row" << parentItem->childItems.indexOf(const_cast<LayersModelItem*>(this));
        return parentItem->childItems.indexOf(const_cast<LayersModelItem*>(this));
    }
    //qDebug() << "row" << 0;
    return 0;
}

int LayersModelItem::columnCount() const
{
    //qDebug() << "columnCoount" << COLUMN_COUNT;
    return COLUMN_COUNT;
}

int LayersModelItem::childCount() const
{
    //qDebug() << "childCount" << childItems.count();
    return childItems.count();
}

LayersModelItem * LayersModelItem::child(int row) const
{
    //qDebug() << "child" << childItems.value(row);
    return childItems.value(row);
}

void LayersModelItem::setParent(LayersModelItem * parent)
{
    if (parent == this->parentItem)
    {
        //qDebug() << "setParent" << "No parent";
        return;
    }
    if (parentItem)
    {
        //qDebug() << "setParent" << "clear" << parentItem;
        parentItem->removeChild(this);
    }
    this->parentItem = parent;
    //qDebug() << "setParent" << parentItem;
    if (parent && !parent->childItems.contains(this))
        parent->childItems.prepend(this);
    this->QObject::setParent(parent);
}

void LayersModelItem::setData(const QList<QVariant> & data)
{
    int i = 0;
    for (QList<QVariant>::const_iterator it = data.begin(); it != data.end(); ++it)
    {
        setData(*it, i);
        i++;
    }
}

bool LayersModelItem::setData(const QVariant & data, int type)
{
    if (type >= COLUMN_COUNT || !this->itemPhoto)
        return false;
    if (type == NameString)
        this->itemPhoto->setName(data.toString());
    /// HERE ADD SOME OTHER DATA TYPES!!!
    return false;
}

void LayersModelItem::setPhoto(AbstractPhoto * photo)
{
    this->itemPhoto = photo;
    if (photo)
    {
        int newZValue = this->parent()->childCount()-this->parent()->childItems.indexOf(this);
        #ifdef QT_DEBUG
                if (photo->zValue() != newZValue)
                    qDebug() << "ZValue changed!" << photo << "Current:" << photo->zValue() << "New:" << newZValue;
        #endif
        photo->setZValue(newZValue);
    }
}

AbstractPhoto * LayersModelItem::photo() const
{
    return this->itemPhoto;
}

QVariant LayersModelItem::data(int column) const
{
    //qDebug() << "data" << this << column;
    if (column == LayersModelItem::NameString)
        return itemPhoto ? itemPhoto->name() : "Name";
    else
        return QVariant();
}

QList<QVariant> LayersModelItem::data() const
{
    //qDebug() << "data" << this << "all";
    QList<QVariant> result;
    for(int i = 0; i < COLUMN_COUNT; ++i)
        result << data(i);
    return result;
}

bool LayersModelItem::insertChildren(int position, LayersModelItem * item)
{
    //qDebug() << "insertChildern" << position << item;
    if (position < 0 || position > childItems.size())
        return false;
    //qDebug() << "dodajemy";
    childItems.insert(position, item);
    if (item != 0)
        item->setParent(this);
    this->refreshZValues();
    return true;
}

bool LayersModelItem::removeChildren(int position, int count)
{
    //qDebug() << "removeChildren" << position << count;
    if (position < 0 || position + count > childItems.size())
        return false;

    //qDebug() << "usuwamy";
    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    this->refreshZValues();
    return true;
}

bool LayersModelItem::moveChildren(int sourcePosition, int count, LayersModelItem * destParent, int destPosition)
{
    if (!count || (sourcePosition < destPosition && sourcePosition+count-1 >= destPosition) || !destParent)
        return false;
    QList<LayersModelItem*> movingItems;
    if (this == destParent && destPosition > sourcePosition)
        destPosition -= count;
    for (;count;--count)
        movingItems.push_back(this->childItems.takeAt(sourcePosition));
    for ( ; movingItems.count() ; movingItems.pop_back())
        destParent->childItems.insert(destPosition, movingItems.last());
    this->refreshZValues();
    if (destParent != this)
        destParent->refreshZValues();
    return true;
}

void LayersModelItem::refreshZValues()
{
    int i = childItems.count();
    foreach (LayersModelItem * item, childItems)
    {
        AbstractPhoto * photo = item->photo();
        if (photo)
            photo->setZValue(i);
        --i;
    }
}
