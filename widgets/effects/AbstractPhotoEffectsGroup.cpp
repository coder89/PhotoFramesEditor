#include "AbstractPhotoEffectsGroup.h"
#include "AbstractPhotoEffect.h"
#include "BlurPhotoEffect.h"
#include "PixelizePhotoEffect.h"
#include "AbstractPhoto.h"

#include <QPainter>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

AbstractPhotoEffectsGroup::AbstractPhotoEffectsGroup(AbstractPhoto * photo, QObject * parent) :
    QAbstractItemModel(parent),
    m_photo(photo)
{
    this->push_back(new BlurPhotoEffect(10));
}

void AbstractPhotoEffectsGroup::push_back(AbstractPhotoEffect * effect)
{
    m_effects_list.push_back(effect);
    effect->setParent(this);
    emit effectsChanged();
    emit layoutChanged();
}

void AbstractPhotoEffectsGroup::push_front(AbstractPhotoEffect * effect)
{
    m_effects_list.push_front(effect);
    effect->setParent(this);
    emit effectsChanged();
    emit layoutChanged();
}

QPixmap AbstractPhotoEffectsGroup::apply(const QPixmap & pixmap)
{
    QImage image = pixmap.toImage();
    foreach (AbstractPhotoEffect * effect, m_effects_list)
        if (effect)
            image = effect->apply(image);
    return QPixmap::fromImage(image);
}

AbstractPhoto * AbstractPhotoEffectsGroup::photo() const
{
    return m_photo;
}

AbstractPhotoEffect * AbstractPhotoEffectsGroup::getItem(const QModelIndex & index) const
{
    return static_cast<AbstractPhotoEffect*>(index.internalPointer());
}

int AbstractPhotoEffectsGroup::columnCount(const QModelIndex & parent) const
{
    return 1;
}

QVariant AbstractPhotoEffectsGroup::data(const QModelIndex & index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (!index.isValid())
        return i18n("Effect name");
    else
        return getItem(index)->toString();
}

QModelIndex AbstractPhotoEffectsGroup::index(int row, int column, const QModelIndex & parent) const
{
    if (column != 0)
        return QModelIndex();
    if (row >= m_effects_list.count())
        return QModelIndex();
    if (parent.isValid())
        return QModelIndex();
    return createIndex(row,column,m_effects_list.at(row));
}

QModelIndex AbstractPhotoEffectsGroup::parent(const QModelIndex & index) const
{
    return QModelIndex();
}

int AbstractPhotoEffectsGroup::rowCount(const QModelIndex & parent) const
{
    return this->m_effects_list.count();
}
