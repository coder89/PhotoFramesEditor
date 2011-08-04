#include "BordersGroup.h"
#include "BorderDrawerInterface.h"
#include "AbstractPhoto.h"

#include "BorderDrawersLoader.h"

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::BordersGroupPrivate
{
    BordersGroupPrivate(BordersGroup * group) :
        group(group),
        photo(0)
    {}

    BordersGroup * group;
    AbstractPhoto * photo;
    QList<BorderDrawerInterface*> borders;
    QPainterPath shape;

    friend class BordersGroup;
};

BordersGroup::BordersGroup(AbstractPhoto * photo) :
    AbstractMovableModel(photo),
    d(new BordersGroupPrivate(this))
{
    d->photo = photo;
}

QPainterPath BordersGroup::shape()
{
    if (!graphicsItem())
        return QPainterPath();
    if (d->shape.isEmpty())
        calculateShape();
    return d->shape;
}

void BordersGroup::calculateShape()
{
    QPainterPath photoShape = graphicsItem()->itemOpaqueArea();
    d->shape = QPainterPath();
    for (int i = d->borders.count()-1; i >= 0; --i)
    {
        BorderDrawerInterface * border = d->borders.at(i);
        if (border)
            d->shape = d->shape.united( border->path(photoShape) );
    }
}

AbstractPhoto * BordersGroup::graphicsItem() const
{
    return d->photo;
}

void BordersGroup::paint(QPainter * painter, const QStyleOptionGraphicsItem * option)
{
    if (!graphicsItem())
        return;
    for (int i = d->borders.count()-1; i >= 0; --i)
    {
        BorderDrawerInterface * border = d->borders.at(i);
        if (border)
            border->paint(painter, option);
    }
}

void BordersGroup::refresh()
{
    QRectF updateRect;
    if (graphicsItem())
        updateRect = updateRect.united(graphicsItem()->boundingRect());
    this->calculateShape();
    if (graphicsItem())
    {
        updateRect = updateRect.united(graphicsItem()->boundingRect());
        graphicsItem()->update(updateRect);
    }
    dataChanged(QModelIndex(), QModelIndex());
}

bool BordersGroup::prependDrawer(BorderDrawerInterface * drawer)
{
    return insertDrawer(drawer, 0);
}

bool BordersGroup::insertDrawer(BorderDrawerInterface * drawer, int position)
{
    if (!drawer || position < 0 || position > rowCount() || !insertRow(position))
        return false;
    d->borders.takeAt(position);
    d->borders.insert(position, drawer);
    return true;
}

bool BordersGroup::appendDrawer(BorderDrawerInterface * drawer)
{
    return insertDrawer(drawer, this->rowCount());
}

BorderDrawerInterface * BordersGroup::removeDrawer(int position)
{
    if (position < 0 || position >= rowCount())
        return 0;
    BorderDrawerInterface * result = d->borders.at(position);
    removeRow(position);
    return result;
}

bool BordersGroup::moveDrawer(int sourcePosition, int destinationPosition)
{
    return moveRows(sourcePosition, 1, destinationPosition);
}

QDomElement BordersGroup::toSvg(QDomDocument & document)
{
    QDomElement result = document.createElement("borders");
    int i = 0;
    foreach (BorderDrawerInterface * border, d->borders)
    {
        QDomElement temp = border->toSvg(document);
        if (temp.isNull())
            continue;
        temp.setAttribute("z", i++);
        result.appendChild(temp);
    }
    return result;
}

BordersGroup * BordersGroup::fromSvg(QDomElement & element)
{
}

QObject * BordersGroup::item(const QModelIndex & index) const
{
    if (index.isValid() && index.row() < d->borders.count())
        return d->borders.at(index.row());
    return 0;
}

void BordersGroup::setItem(QObject * item, const QModelIndex & index)
{
    BorderDrawerInterface * drawer = dynamic_cast<BorderDrawerInterface*>(item);
    if (!drawer || !index.isValid())
        return;
    int row = index.row();
    if (row < 0 || row >= rowCount())
        return;
    if (drawer == d->borders.at(row))
        return;
    d->borders.removeAt(row);
    d->borders.insert(row, drawer);
    drawer->setGroup(this);
    this->refresh();
}

int BordersGroup::columnCount(const QModelIndex & parent) const
{
    return 1;
}

QVariant BordersGroup::data(const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole && hasIndex(index.row(), index.column(), index.parent()))
    {
        BorderDrawerInterface * border = d->borders.at(index.row());
        if (border)
            return border->toString();
    }
    return QVariant();
}

QModelIndex BordersGroup::index(int row, int column, const QModelIndex & parent) const
{
    if (row >= rowCount(parent) || column >= columnCount(parent))
        return QModelIndex();
    return createIndex(row, column, d->borders.at(row));
}

bool BordersGroup::insertRows(int row, int count, const QModelIndex & parent)
{

    if (row > rowCount(parent) || count < 0)
        return false;
    beginInsertRows(parent, row, row+count-1);
    while (count--)
        d->borders.insert(row, 0);
    endInsertRows();
    emit layoutChanged();
    return true;
}

QModelIndex BordersGroup::parent(const QModelIndex & child) const
{
    return QModelIndex();
}

bool BordersGroup::removeRows(int row, int count, const QModelIndex & parent)
{
    if (row >= rowCount(parent) || count < 0 || row+count > rowCount(parent))
        return false;
    beginRemoveRows(QModelIndex(), row, row+count-1);
    while (count--)
        d->borders.removeAt(row);
    endRemoveRows();
    this->refresh();
    emit layoutChanged();
    return true;
}

int BordersGroup::rowCount(const QModelIndex & parent) const
{
    if (parent.isValid())
        return 0;
    return d->borders.count();
}

bool BordersGroup::moveRows(int sourcePosition, int sourceCount, int destPosition)
{
    if (sourcePosition <= destPosition && sourcePosition+sourceCount >= destPosition)
        return false;
    beginMoveRows(QModelIndex(), sourcePosition, sourcePosition+sourceCount-1, QModelIndex(), destPosition);
    QList<BorderDrawerInterface*> movingItems;
    if (destPosition > sourcePosition)
        destPosition -= sourceCount;
    while(sourceCount--)
        movingItems.push_back(d->borders.takeAt(sourcePosition));
    for ( ; movingItems.count() ; movingItems.pop_back())
        d->borders.insert(destPosition, movingItems.last());
    endMoveRows();
    this->refresh();
    emit layoutChanged();
}