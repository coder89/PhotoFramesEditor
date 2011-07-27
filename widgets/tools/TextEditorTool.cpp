#include "TextEditorTool.h"
#include "TextItem.h"
#include "ToolsDockWidget.h"

#include <QVBoxLayout>

using namespace KIPIPhotoFramesEditor;

TextEditorTool::TextEditorTool(ToolsDockWidget * parent) :
    AbstractTool(parent),
    m_text_item(0),
    m_browser(0)
{
    QVBoxLayout * layout = new QVBoxLayout();
    this->setLayout(layout);
}

void TextEditorTool::currentItemAboutToBeChanged()
{
    if (m_browser)
    {
        this->layout()->removeWidget(m_browser);
        m_browser->deleteLater();
        m_browser = 0;
    }
}

void TextEditorTool::currentItemChanged()
{
    m_text_item = dynamic_cast<TextItem*>(currentItem());
    if (m_text_item)
    {
        m_browser = m_text_item->propertyBrowser();
        if (m_browser)
        {
            this->layout()->addWidget(m_browser);
        }
    }
}

void TextEditorTool::positionAboutToBeChanged()
{
    if (m_text_item)
        m_text_item->clearFocus();
}

void TextEditorTool::positionChanged()
{
    setCurrentItem( new TextItem() );
    currentItem()->setPos( this->mousePosition() );
    emit itemCreated( currentItem() );
}
