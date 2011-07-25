#include "AbstractTool.h"
#include "AbstractPhoto.h"

using namespace KIPIPhotoFramesEditor;

AbstractTool::AbstractTool(QWidget * parent) :
    QWidget(parent)
{
}

AbstractPhoto * AbstractTool::currentItem()
{
    return m_photo;
}

void AbstractTool::setCurrentItem(AbstractPhoto * photo)
{
    currentItemAboutToBeChanged();
    m_photo = photo;
    setEnabled((bool)m_photo);
    currentItemChanged();
}
