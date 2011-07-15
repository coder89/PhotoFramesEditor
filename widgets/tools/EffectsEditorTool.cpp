#include "EffectsEditorTool.h"
#include "AbstractPhoto.h"
#include "AbstractPhotoEffect.h"
#include "AbstractPhotoEffectsGroup.h"

#include <QGridLayout>
#include <QLabel>
#include <QListView>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::EffectsEditorToolPrivate
{
    QListView * m_list_widget;

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
    layout->addWidget(title,0,0);

    // Effects list
    d->m_list_widget = new QListView(this);
    layout->addWidget(d->m_list_widget,1,0);
    connect(d->m_list_widget,SIGNAL(clicked(QModelIndex)),this,SLOT(viewCurrentEffectEditor(QModelIndex)));

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
}

void EffectsEditorTool::viewCurrentEffectEditor(const QModelIndex & index)
{
    AbstractPhoto * photo = this->currentItem();
    qDebug() << "viewCurrentEffectEditor!";
    if (photo)
    {
        QtAbstractPropertyBrowser * browser = photo->effectsGroup()->getItem(index)->propertyBrowser();
        static_cast<QGridLayout*>(layout())->addWidget(browser,2,0);
        qDebug() << "added!";
    }
}
