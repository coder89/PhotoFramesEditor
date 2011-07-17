#include "PhotoEffectsGroup.h"
#include "PhotoEffectsLoader.h"
#include "BlurPhotoEffect.h"
#include "PixelizePhotoEffect.h"
#include "AbstractPhoto.h"
#include "AbstractPhotoEffectFactory.h"

#include <QPainter>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

PhotoEffectsGroup::PhotoEffectsGroup(AbstractPhoto * photo, QObject * parent) :
    QAbstractItemModel(parent),
    m_photo(photo)
{
    QStringList names = PhotoEffectsLoader::registeredEffectsNames();
    qDebug() << names;
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
    return createIndex(row,column,m_effects_list.at(m_effects_list.count()-row-1));
}

QModelIndex PhotoEffectsGroup::parent(const QModelIndex & index) const
{
    return QModelIndex();
}

int PhotoEffectsGroup::rowCount(const QModelIndex & parent) const
{
    return this->m_effects_list.count();
}

void PhotoEffectsGroup::emitEffectsChanged(AbstractPhotoEffectInterface * effect)
{
    m_photo->refreshPixmap();
    int row = m_effects_list.count()-m_effects_list.indexOf(effect)-1;
    QModelIndex indexChanged = index(row,0);
    emit dataChanged(indexChanged,indexChanged);
    emit effectsChanged();
}
