#include "LayersModelItem.h"
#include "abstract_photo.h"

// Qt
#include <QVariant>
#include <QDebug>

using namespace KIPIPhotoFramesEditor;

LayersModelItem::LayersModelItem(const QList<QVariant> & data, LayersModelItem * parent) :
    parentItem(0)
{
    if (parent != this)
    {
        this->parentItem = parent;
        if (parent)
            parent->childItems.append(this);
    }
    this->itemData = data;
    qDebug() << "LayersModelItem" << (long)this;
}

LayersModelItem::LayersModelItem() :
    parentItem(0)
{
    qDebug() << "LayersModelItem" << (long)this;
}

LayersModelItem::~LayersModelItem()
{
    if (parentItem)
        parentItem->removeChild(this);
    qDebug() << "LayersModelItem parent" << (long)parentItem;
    qDebug() << "LayersModelItem" << (long)this;
}

void LayersModelItem::removeChild(LayersModelItem * child)
{
    if (child && child->parent() == this)
    {
        this->childItems.removeAll(child);
        child->setParent(0);
    }
}

LayersModelItem * LayersModelItem::parent() const
{
    return parentItem;
}

int LayersModelItem::row() const
{
    qDebug() << "LayersModelItem" << (long)this;
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<LayersModelItem*>(this));
    return 0;
}

int LayersModelItem::columnCount() const
{
    return itemData.count();
}

int LayersModelItem::childCount() const
{
    return childItems.count();
}

LayersModelItem * LayersModelItem::child(int row) const
{
    return childItems.value(row);
}

void LayersModelItem::setParent(LayersModelItem * parent)
{
    if (parent != this->parentItem && this->parentItem)
        parentItem->childItems.removeAll(this);
    this->parentItem = parent;
    if (parent && !parent->childItems.contains(this))
        parent->childItems.prepend(this);
    this->QObject::setParent(parent);
}

void LayersModelItem::setData(const QList<QVariant> & data)
{
    this->itemData = data;
}

bool LayersModelItem::setData(const QVariant & data, DataType type)
{
    if (type >= this->itemData.count())
        return false;
    this->itemData.takeAt(type);
    this->itemData.insert(type,data);
    return true;
}

QVariant LayersModelItem::data(int column) const
{
    return itemData.value(column, QVariant());
}

QList<QVariant> LayersModelItem::data() const
{
    return itemData;
}

bool LayersModelItem::insertChildren(int position, LayersModelItem * item)
{
    if (position < 0 || position > childItems.size())
        return false;

    childItems.insert(position, item);
    if (item != 0)
        item->setParent(this);
    return true;
}

bool LayersModelItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}
