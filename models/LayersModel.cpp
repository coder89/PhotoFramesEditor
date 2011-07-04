#include "LayersModel.h"

// KDE
#include <kicon.h>

// Qt
#include <QDebug>
#include <QCommonStyle>
#include <QLabel>
#include <QVariant>
#include <QList>

// Local
#include "LayersModelItem.h"
#include "abstract_photo.h"

using namespace KIPIPhotoFramesEditor;

LayersModel::LayersModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    root = new LayersModelItem(
                                (QList<QVariant>() << "sfa" << QIcon(":eye.png") << QIcon(":padlock.png") << "Name")
                              );
    this->setSupportedDragActions(Qt::MoveAction);
}

LayersModel::~LayersModel()
{
    delete root;
}

QModelIndex LayersModel::index(int row, int column, const QModelIndex & parent) const
{
    if (!hasIndex( row, column, parent ))
        return QModelIndex();
    LayersModelItem * parentItem;
    if(parent.isValid())
        parentItem = static_cast<LayersModelItem*>(parent.internalPointer());
    else
        parentItem = root;
    LayersModelItem * childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row,column,childItem);
    else
        return QModelIndex();
}

QModelIndex LayersModel::parent(const QModelIndex & index) const
{
    if(!index.isValid())
        return QModelIndex();
    LayersModelItem * childItem = static_cast<LayersModelItem*>(index.internalPointer());
    LayersModelItem * parentItem = childItem->parent();
    if (parentItem == root)
        return QModelIndex();
    return createIndex(parentItem->row(), 0, parentItem);
}

int LayersModel::rowCount(const QModelIndex & parent) const
{
    if (parent.column() > 0)
        return 0;
    if (!parent.isValid())
        return root->childCount();
    else
        return static_cast<LayersModelItem*>(parent.internalPointer())->childCount();
}

int LayersModel::columnCount(const QModelIndex & parent) const
{
    if (parent.isValid())
        return static_cast<LayersModelItem*>(parent.internalPointer())->columnCount();
    else
        return root->columnCount();
}

QVariant LayersModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
        return QVariant();
    LayersModelItem * item = static_cast<LayersModelItem*>(index.internalPointer());
    switch(role)
    {    
        case LayersModel::VisibilityIcon:
            return item->data(1);
        case LayersModel::LockIcon:
            return item->data(2);
        case Qt::DisplayRole:
            if (index.column() != 1)
                return item->data(index.column());
            break;
        case Qt::EditRole:
            if (index.column() == LayersModelItem::NameString)
                return item->data(LayersModelItem::NameString);
    }
    return QVariant();
}

bool LayersModel::setData(const QModelIndex & index, const QVariant & value, int /*role*/)
{
    if (!index.isValid() || index.column() != LayersModelItem::NameString)
        return false;
    LayersModelItem * item = static_cast<LayersModelItem*>(index.internalPointer());
    return item->setData(value, LayersModelItem::NameString);
}

Qt::ItemFlags LayersModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags result = Qt::ItemIsDropEnabled | this->QAbstractItemModel::flags(index);
    if (index.isValid())
    {
        if (index.column() == LayersModelItem::NameString)
            result |= Qt::ItemIsEditable;
        result |= Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
    }
    return result;
}

QVariant LayersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        switch (role)
        {
            case Qt::DisplayRole:
                return root->data(section);
            case Qt::DecorationRole:
                if (section == 1 || section == 2)
                    return root->data(section);
        }
    }
    return QVariant();
}

bool LayersModel::appendItem(AbstractPhoto * item, const QModelIndex & parent)
{
    LayersModelItem * parentItem = getItem(parent);
    beginInsertRows(parent, parentItem->childCount(), parentItem->childCount());
    parentItem->insertChildren(parentItem->childCount(), item);
    endInsertRows();
    return true;
}

bool LayersModel::prependItem(AbstractPhoto * item, const QModelIndex & parent)
{
    beginInsertRows(parent, 0, 0);
    LayersModelItem * parentItem = getItem(parent);
    parentItem->insertChildren(0, item);
    endInsertRows();
    return true;
}

bool LayersModel::appendItems(const QList<AbstractPhoto*> & items, const QModelIndex & parent)
{
    foreach(AbstractPhoto * item, items)
        if (!appendItem(item, parent))
            return false;
    return true;
}

bool LayersModel::insertRows(int position, int count, const QModelIndex  &parent)
{
    beginInsertRows(parent,position,position+count-1);
    endInsertRows();
    return true;
}

void LayersModel::addNewItem(AbstractPhoto * item, const QModelIndex & parent)
{
    insertRow(1, QModelIndex());
}

LayersModelItem * LayersModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        LayersModelItem * item = static_cast<LayersModelItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return root;
}

QModelIndexList LayersModel::itemsToIndexes(const QList<AbstractPhoto*> & items) const
{
    QModelIndexList indexes;
    QModelIndex temp;
    foreach(AbstractPhoto * item, items)
    {
        temp = findIndex(static_cast<LayersModelItem*>(item));
        if (temp.isValid())
            indexes.append(temp);
    }
    return indexes;
}

QModelIndex LayersModel::findIndex(LayersModelItem * item, const QModelIndex & parent) const
{
    QModelIndex temp;
    LayersModelItem * parentItem = getItem(parent);
    int rows = parentItem->childCount();
    for (int i = 0; i < rows; ++i)
    {
        temp = index(i,0,parent);
        if (!temp.isValid())
            continue;
        if (static_cast<LayersModelItem*>(temp.internalPointer()) == item)
            return temp;
        temp = findIndex(item, temp);
        if (temp.isValid())
            return temp;
    }
    return QModelIndex();
}

bool LayersModel::removeRows(int row, int count, const QModelIndex & parent)
{
    LayersModelItem * parentItem = getItem(parent);
    beginRemoveRows(parent, row, row+count-1);
    bool result = parentItem->removeChildren(row, count);
    endRemoveRows();
    return result;
}
