#include "EffectsEditorTool.h"
#include "AbstractPhoto.h"
#include "PhotoEffectsLoader.h"
#include "PhotoEffectsGroup.h"

#include <QGridLayout>
#include <QLabel>
#include <QListView>
#include <QtAbstractPropertyBrowser>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::EffectsEditorToolPrivate
{
    EffectsListView * m_list_widget;

    friend class EffectsEditorTool;
};

EffectsEditorTool::EffectsEditorTool(QWidget * parent) :
    AbstractTool(parent),
    d(new EffectsEditorToolPrivate)
{
    QGridLayout * layout = new QGridLayout(this);

    // Title
    QLabel * title = new QLabel(i18n("Effects editor"), this);
    QFont titleFont = title->font();
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title,0,0,1,-1);

    // Effects list
    d->m_list_widget = new EffectsListView(this);
    layout->addWidget(d->m_list_widget,1,0,1,-1);
    connect(d->m_list_widget,SIGNAL(selectionChanged(PhotoEffectsLoader*)),this,SLOT(viewCurrentEffectEditor(PhotoEffectsLoader*)));

    this->setLayout(layout);
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
}

void EffectsEditorTool::currentItemChanged()
{
    AbstractPhoto * photo = this->currentItem();
    if (photo)
        d->m_list_widget->setModel(photo->effectsGroup());
    else
        d->m_list_widget->setModel(0);
    removeCurrentPropertyBrowser();
}

void EffectsEditorTool::viewCurrentEffectEditor(PhotoEffectsLoader * effect)
{
    removeCurrentPropertyBrowser();
    if (effect)
    {
        QtAbstractPropertyBrowser * browser = effect->propertyBrowser();
        static_cast<QGridLayout*>(layout())->addWidget(browser,2,0,1,-1);
        browser->show();
    }
}

void EffectsEditorTool::removeCurrentPropertyBrowser()
{
    QLayoutItem * itemBrowser = static_cast<QGridLayout*>(layout())->itemAtPosition(2,0);
    if (!itemBrowser)
        return;
    QWidget * browser = itemBrowser->widget();
    if (!browser)
        return;
    static_cast<QGridLayout*>(layout())->removeWidget(browser);
    browser->hide();
}
