#include "PhotoEffectsGroup.h"
#include "AbstractPhotoEffect.h"
#include "BlurPhotoEffect.h"
#include "PixelizePhotoEffect.h"
#include "AbstractPhoto.h"

#include <QPainter>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

PhotoEffectsGroup::PhotoEffectsGroup(AbstractPhoto * photo, QObject * parent) :
    QAbstractItemModel(parent),
    m_photo(photo)
{
    this->push_back(new BlurPhotoEffect(2));
    this->push_back(new BlurPhotoEffect(2));
    this->push_back(new BlurPhotoEffect(3));
    this->push_back(new BlurPhotoEffect(1));
    this->push_back(new PixelizePhotoEffect(2));
}

void PhotoEffectsGroup::push_back(AbstractPhotoEffect * effect)
{
    m_effects_list.push_back(effect);
    effect->setParent(this);
    connect(effect,SIGNAL(effectChanged()),this,SLOT(emitEffectsChanged()));
    emitEffectsChanged();
    emit layoutChanged();
}

void PhotoEffectsGroup::push_front(AbstractPhotoEffect * effect)
{
    m_effects_list.push_front(effect);
    effect->setParent(this);
    connect(effect,SIGNAL(effectChanged()),this,SLOT(emitEffectsChanged()));
    emitEffectsChanged();
    emit layoutChanged();
}

QPixmap PhotoEffectsGroup::apply(const QPixmap & pixmap)
{
    QImage image = pixmap.toImage();
    foreach (AbstractPhotoEffect * effect, m_effects_list)
        if (effect)
            image = effect->apply(image);
    return QPixmap::fromImage(image);
}

AbstractPhoto * PhotoEffectsGroup::photo() const
{
    return m_photo;
}

AbstractPhotoEffect * PhotoEffectsGroup::getItem(const QModelIndex & index) const
{
    return static_cast<AbstractPhotoEffect*>(index.internalPointer());
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

void PhotoEffectsGroup::emitEffectsChanged()
{
    m_photo->refreshPixmap();
    emit effectsChanged();
}
