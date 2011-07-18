#include "PhotoEffectsGroup.h"
#include "PhotoEffectsLoader.h"
#include "BlurPhotoEffect.h"
#include "PixelizePhotoEffect.h"
#include "AbstractPhoto.h"
#include "AbstractPhotoEffectFactory.h"
#include "UndoCommandEvent.h"
#include "photoframeseditor.h"

#include <QPainter>

#include <klocalizedstring.h>
#include <kapplication.h>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::MoveItemsUndoCommand : public QUndoCommand
{
        PhotoEffectsGroup * m_model;
        int m_starting_row;
        int m_rows_count;
        int m_destination_row;

    public:
        MoveItemsUndoCommand(int sourcePosition, int sourceCount, int destPosition, PhotoEffectsGroup * model, QUndoCommand * parent = 0) :
            QUndoCommand(parent),
            m_model(model),
            m_starting_row(sourcePosition),
            m_rows_count(sourceCount),
            m_destination_row(destPosition)
        {}
        virtual void redo();
        virtual void undo();
    private:
        void reverse();

};

void MoveItemsUndoCommand::redo()
{
    if (m_model)
    {
        m_model->moveRowsInModel(m_starting_row, m_rows_count, m_destination_row);
        reverse();
    }
}

void MoveItemsUndoCommand::undo()
{
    if (m_model)
    {
        m_model->moveRowsInModel(m_starting_row, m_rows_count, m_destination_row);
        reverse();
    }
}

void MoveItemsUndoCommand::reverse()
{
    int temp = m_destination_row;
    m_destination_row = m_starting_row;
    m_starting_row = temp;
    if (m_destination_row > m_starting_row)
        m_destination_row += m_rows_count;
    else
        m_starting_row -= m_rows_count;
}

class KIPIPhotoFramesEditor::RemoveItemsUndoCommand : public QUndoCommand
{
        int m_starting_pos;
        int m_count;
        PhotoEffectsGroup * m_model;
        QList<AbstractPhotoEffectInterface*> tempItemsList;
    public:
        RemoveItemsUndoCommand(int startingPos, int count, PhotoEffectsGroup * model, QUndoCommand * parent = 0) :
            QUndoCommand(parent),
            m_starting_pos(startingPos),
            m_count(count),
            m_model(model)
        {}
        ~RemoveItemsUndoCommand()
        {
            qDeleteAll(tempItemsList);
        }
        virtual void redo();
        virtual void undo();
};

void RemoveItemsUndoCommand::redo()
{
    tempItemsList = m_model->removeRowsInModel(m_starting_pos, m_count);
}

void RemoveItemsUndoCommand::undo()
{
    m_model->insertRowsInModel(tempItemsList, m_starting_pos);
    tempItemsList.clear();
}

PhotoEffectsGroup::PhotoEffectsGroup(AbstractPhoto * photo, QObject * parent) :
    QAbstractItemModel(parent),
    m_photo(photo)
{
    QStringList names = PhotoEffectsLoader::registeredEffectsNames();
    this->push_back(PhotoEffectsLoader::getEffectByName(names.at(0)));
    this->push_back(PhotoEffectsLoader::getEffectByName(names.at(1)));
    this->push_back(PhotoEffectsLoader::getEffectByName(names.at(0)));
    this->push_back(PhotoEffectsLoader::getEffectByName(names.at(1)));
    this->push_back(PhotoEffectsLoader::getEffectByName(names.at(0)));
}

void PhotoEffectsGroup::push_back(AbstractPhotoEffectInterface * effect)
{
    m_effects_list.push_back(effect);
    effect->setParent(this);
    emitEffectsChanged(effect);
    emit layoutChanged();
}

void PhotoEffectsGroup::push_front(AbstractPhotoEffectInterface * effect)
{
    m_effects_list.push_back(effect);
    effect->setParent(this);
    emitEffectsChanged(effect);
    emit layoutChanged();
}

QPixmap PhotoEffectsGroup::apply(const QPixmap & pixmap)
{
    QImage image = pixmap.toImage();
    foreach (AbstractPhotoEffectInterface * effect, m_effects_list)
        if (effect)
            image = effect->apply(image);
    return QPixmap::fromImage(image);
}

AbstractPhoto * PhotoEffectsGroup::photo() const
{
    return m_photo;
}

AbstractPhotoEffectInterface * PhotoEffectsGroup::getItem(const QModelIndex & index) const
{
    return static_cast<AbstractPhotoEffectInterface*>(index.internalPointer());
}

bool PhotoEffectsGroup::moveRows(int sourcePosition, int sourceCount, int destPosition)
{
    if (    sourceCount                                          &&
            sourcePosition < rowCount()                          &&
            sourcePosition+sourceCount <= rowCount()             &&
            destPosition <= rowCount()                           &&
            sourcePosition != destPosition                       &&
            sourcePosition != destPosition-1                     &&
            sourcePosition >= 0                                  &&
            destPosition >= 0)
    {
        QUndoCommand * command = new MoveItemsUndoCommand(sourcePosition, sourceCount, destPosition, this);
        UndoCommandEvent * event = new UndoCommandEvent();
        event->setUndoCommand(command);
        KApplication::postEvent(PhotoFramesEditor::instancePhotoFramesEditor(),event);
        return true;
    }
    return false;
}

int PhotoEffectsGroup::columnCount(const QModelIndex & parent) const
{
    return 1;
}

QVariant PhotoEffectsGroup::data(const QModelIndex & index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (!index.isValid())
        return i18n("Effect name");
    else
        return getItem(index)->toString();
}

QModelIndex PhotoEffectsGroup::index(int row, int column, const QModelIndex & parent) const
{
    if (column != 0)
        return QModelIndex();
    if (row >= m_effects_list.count())
        return QModelIndex();
    if (parent.isValid())
        return QModelIndex();
    return createIndex(row,column,m_effects_list.at(rowCount()-row-1));
}

QModelIndex PhotoEffectsGroup::parent(const QModelIndex & index) const
{
    return QModelIndex();
}

int PhotoEffectsGroup::rowCount(const QModelIndex & parent) const
{
    if (!parent.isValid())
        return this->m_effects_list.count();
    else
        return 0;
}

bool PhotoEffectsGroup::removeRows(int row, int count, const QModelIndex & parent)
{
    if (!count || row < 0 || row > rowCount(parent)-1)
        return false;
    QUndoCommand * command = new RemoveItemsUndoCommand(row, count, this);
    UndoCommandEvent * event = new UndoCommandEvent();
    event->setUndoCommand(command);
    KApplication::postEvent(PhotoFramesEditor::instancePhotoFramesEditor(),event);
    return true;
}

void PhotoEffectsGroup::emitEffectsChanged(AbstractPhotoEffectInterface * effect)
{
    m_photo->refreshPixmap();
    if (effect)
    {
        int row = m_effects_list.count()-m_effects_list.indexOf(effect)-1;
        QModelIndex indexChanged = index(row,0);
        emit dataChanged(indexChanged,indexChanged);
    }
    else if (rowCount())
        emit dataChanged(index(0,0),index(rowCount()-1,0));
    emit effectsChanged();
}

void PhotoEffectsGroup::moveRowsInModel(int sourcePosition, int sourceCount, int destPosition)
{
    beginMoveRows(QModelIndex(), sourcePosition, sourcePosition+sourceCount-1, QModelIndex(), destPosition);

    // Inverse directions becouse effects stack is presented in reverse order
    destPosition = rowCount()-destPosition;
    sourcePosition = rowCount()-sourcePosition-1;

    QList<AbstractPhotoEffectInterface*> movingItems;
    if (destPosition > sourcePosition)
        destPosition -= sourceCount;
    while(sourceCount--)
        movingItems.push_back(m_effects_list.takeAt(sourcePosition));
    for ( ; movingItems.count() ; movingItems.pop_back())
        m_effects_list.insert(destPosition, movingItems.last());

    endMoveRows();
    emitEffectsChanged();
    emit layoutChanged();
}

QList<AbstractPhotoEffectInterface*> PhotoEffectsGroup::removeRowsInModel(int startingPosition, int count)
{
    beginRemoveRows(QModelIndex(), startingPosition, startingPosition+count-1);

    // Inverse directions becouse effects stack is presented in reverse order
    startingPosition = rowCount()-startingPosition-1;

    QList<AbstractPhotoEffectInterface*> removedItems;
    while (count--)
        removedItems.push_back( m_effects_list.takeAt(startingPosition) );

    endRemoveRows();
    emitEffectsChanged();
    emit layoutChanged();

    return removedItems;
}

void PhotoEffectsGroup::insertRowsInModel(const QList<AbstractPhotoEffectInterface*> & itemList, int startingPosition)
{
    beginInsertRows(QModelIndex(), startingPosition, startingPosition+itemList.count()-1);

    // Inverse directions becouse effects stack is presented in reverse order
    startingPosition = rowCount()-startingPosition;

    foreach (AbstractPhotoEffectInterface * effect, itemList)
    {
        m_effects_list.insert(startingPosition, effect);
        ++startingPosition;
    }

    endInsertRows();
    emitEffectsChanged();
    emit layoutChanged();
}
