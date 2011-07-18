#include "EffectsEditorTool.h"
#include "AbstractPhoto.h"
#include "PhotoEffectsLoader.h"
#include "PhotoEffectsGroup.h"

#include <QGridLayout>
#include <QLabel>
#include <QListView>
#include <QtAbstractPropertyBrowser>

#include <klocalizedstring.h>
#include <kpushbutton.h>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::EffectsEditorToolPrivate
{
    EffectsEditorToolPrivate() :
        m_list_widget(0),
        m_add_button(0),
        m_remove_button(0),
        m_down_button(0),
        m_up_button(0)
    {}
    EffectsListView * m_list_widget;
    KPushButton * m_add_button;
    KPushButton * m_remove_button;
    KPushButton * m_down_button;
    KPushButton * m_up_button;

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

    // Move add/remove buttons
    QHBoxLayout * addLayout = new QHBoxLayout();
    d->m_add_button = new KPushButton(KGuiItem("",":action_add.png",
                             i18n("Add new effect"),
                             i18n("This button adds new effect to the list. You'll be able to select effect type after you click this button.")));
    d->m_add_button->setIconSize(QSize(16,16));
    d->m_add_button->setFixedSize(24,24);
    d->m_remove_button = new KPushButton(KGuiItem("",":action_remove.png",
                             i18n("Remove selected item"),
                             i18n("This button removes selected item from the effects list.")));
    d->m_remove_button->setIconSize(QSize(16,16));
    d->m_remove_button->setFixedSize(24,24);
    d->m_remove_button->setEnabled(false);
    addLayout->addWidget(d->m_add_button);
    addLayout->addWidget(d->m_remove_button);
    addLayout->setSpacing(0);
    layout->addLayout(addLayout,0,1);
    connect(d->m_remove_button,SIGNAL(clicked()),this,SLOT(removeSelected()));

    // Move up/down buttons
    QHBoxLayout * moveLayout = new QHBoxLayout();
    d->m_down_button = new KPushButton(KGuiItem("",":arrow_down.png",
                             i18n("Moves effect down"),
                             i18n("This button moves the selected effect down in stack of effect's layers.")));
    d->m_down_button->setIconSize(QSize(16,16));
    d->m_down_button->setFixedSize(24,24);
    d->m_down_button->setEnabled(false);
    d->m_up_button = new KPushButton(KGuiItem("",":arrow_top.png",
                             i18n("Moves effect up"),
                             i18n("This button moves the selected effect up in stack of effect's layers.")));

    d->m_up_button->setIconSize(QSize(16,16));
    d->m_up_button->setFixedSize(24,24);
    d->m_up_button->setEnabled(false);
    moveLayout->addWidget(d->m_down_button);
    moveLayout->addWidget(d->m_up_button);
    moveLayout->setSpacing(0);
    layout->addLayout(moveLayout,0,2);
    connect(d->m_down_button,SIGNAL(clicked()),this,SLOT(moveSelectedDown()));
    connect(d->m_up_button,SIGNAL(clicked()),this,SLOT(moveSelectedUp()));

    // Effects list
    d->m_list_widget = new EffectsListView(this);
    layout->addWidget(d->m_list_widget,1,0,1,-1);
    connect(d->m_list_widget,SIGNAL(selectedIndex(QModelIndex)),this,SLOT(viewCurrentEffectEditor(QModelIndex)));

    this->setLayout(layout);
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
    layout->setRowStretch(2,1);
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

void EffectsEditorTool::viewCurrentEffectEditor(const QModelIndex & index)
{
    removeCurrentPropertyBrowser();
    d->m_remove_button->setEnabled(index.isValid());
    d->m_down_button->setEnabled(index.isValid() && (index.row() < index.model()->rowCount()-1));
    d->m_up_button->setEnabled(index.isValid() && (index.row() > 0));
    AbstractPhotoEffectInterface * effect = static_cast<AbstractPhotoEffectInterface*>(index.internalPointer());
    if (effect)
    {
        QtAbstractPropertyBrowser * browser = PhotoEffectsLoader::propertyBrowser(effect);
        static_cast<QGridLayout*>(layout())->addWidget(browser,2,0,1,-1);
        browser->show();
    }
}

void EffectsEditorTool::removeSelected()
{
    if (!d->m_list_widget)
        return;
    QModelIndex index = d->m_list_widget->selectedIndex();
    PhotoEffectsGroup * model = qobject_cast<PhotoEffectsGroup*>(d->m_list_widget->model());
    if (model && index.isValid())
        model->removeRow(index.row());
}

void EffectsEditorTool::moveSelectedDown()
{
    if (!d->m_list_widget)
        return;
    QModelIndex index = d->m_list_widget->selectedIndex();
    PhotoEffectsGroup * model = qobject_cast<PhotoEffectsGroup*>(d->m_list_widget->model());
    if (model && index.row() < model->rowCount()-1)
        model->moveRows(index.row(),1,index.row()+2);
}

void EffectsEditorTool::moveSelectedUp()
{
    if (!d->m_list_widget)
        return;
    QModelIndex index = d->m_list_widget->selectedIndex();
    PhotoEffectsGroup * model = qobject_cast<PhotoEffectsGroup*>(d->m_list_widget->model());
    if (model && index.row() > 0)
        model->moveRows(index.row(),1,index.row()-1);
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
    browser->deleteLater();
}
