#include "AbstractTool.h"
#include "AbstractPhoto.h"
#include "ToolsDockWidget.h"

using namespace KIPIPhotoFramesEditor;

AbstractTool::AbstractTool(ToolsDockWidget * parent) :
    QWidget(parent)
{
}

AbstractPhoto * AbstractTool::currentItem()
{
    return m_photo;
}

void AbstractTool::setCurrentItem(AbstractPhoto * photo)
{
    if (m_photo == photo)
        return;
    currentItemAboutToBeChanged();
    m_photo = photo;
    setEnabled((bool)m_photo);
    currentItemChanged();
}

QPointF AbstractTool::mousePosition()
{
    return m_point;
}

void AbstractTool::setMousePosition(const QPointF & position)
{
    if (m_point == position)
        return;
    positionAboutToBeChanged();
    m_point = position;
    positionChanged();
}
