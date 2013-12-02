/****************************************************************************
 **
 **  Copyright (C) 2013 Andreas Mussgiller
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 **
 ****************************************************************************/

#include <QHeaderView>
#include <QMimeData>
#include <QUuid>

#include <nqlogger.h>

#include <materialcategory.h>
#include <materialtableview.h>

MaterialTableView::MaterialTableView(MaterialListModel *listmodel,
                                     MaterialSelectionModel * selectionmodel,
                                     PropertyModel *propertymodel,
                                     ParameterModel *parametermodel,
                                     QWidget *parent) :
    QTableWidget(1, 1, parent),
    ListModel_(listmodel),
    SelectionModel_(selectionmodel),
    PropertyModel_(propertymodel),
    ParameterModel_(parametermodel)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(displayContextMenu(const QPoint&)));

    connect(ListModel_, SIGNAL(materialCountChanged(int)),
            this, SLOT(fillTable(int)));

    connect(ListModel_, SIGNAL(metadataChanged(Material*)),
            this, SLOT(metadataChanged(Material*)));

    connect(SelectionModel_, SIGNAL(selectionChanged(Material*)),
            this, SLOT(setSelection(Material*)));

    connect(this, SIGNAL(itemSelectionChanged()),
            this, SLOT(selectionChanged()));

    connect(this, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(itemEdited(QTableWidgetItem*)));

    fillTable(ListModel_->getMaterialCount());

    setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Material")));

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DropOnly);

    setAlternatingRowColors(true);

    horizontalHeader()->setStretchLastSection(true);

    setMinimumWidth(350);
    setMinimumHeight(200);

    ContextMenu_ = new QMenu();
    ContextMenu_->addAction(tr("Delete"), selectionmodel, SLOT(deleteMaterial()));
    ContextMenu_->addSeparator();
    ContextMenu_->addAction(tr("Duplicate"), selectionmodel, SLOT(duplicateMaterial()));
}

void MaterialTableView::fillTable(int count)
{
    setEnabled(count >= 0);

    clearContents();
    setRowCount(count+1);

    Material * mat;
    for (int i=0;i<count;++i) {
        mat = ListModel_->getMaterial(i);

        MaterialTableItem * item = new MaterialTableItem(mat, QTableWidgetItem::UserType+100);
        item->setText(mat->getName());

        if (mat->getCategory()) {
            item->setIcon(mat->getCategory()->getIcon());
        } else {
            item->setIcon(MaterialCategory::getEmptyIcon());
        }

        setItem(i, 0, item);
        setVerticalHeaderItem(i, new QTableWidgetItem(QString().setNum(i+1)));

        indexMap_[mat] = item;
    }

    setItem(count, 0, new MaterialTableItem(NULL, QTableWidgetItem::UserType+101));
    setVerticalHeaderItem(count, new QTableWidgetItem("*"));
}

void MaterialTableView::setSelection(Material *selection)
{
    MaterialMap::const_iterator it = indexMap_.find(selection);

    // assure it was actually found
    if (it != indexMap_.end())
        setCurrentItem( it->second );
    else
        setCurrentItem( 0 );
}

void MaterialTableView::metadataChanged(Material* material)
{
    if (!material) return;

    NQLog("MaterialTableView", NQLog::Spam) << "void metadataChanged(Material* material) "
                                            << material->getName();

    MaterialMap::iterator it = indexMap_.find(material);
    if (it!=indexMap_.end()) {
        MaterialTableItem* item = it->second;
        if (material->getCategory()==0) {
            item->setIcon(MaterialCategory::getEmptyIcon());
        } else {
            item->setIcon(material->getCategory()->getIcon());
        }
    }
}

void MaterialTableView::selectionChanged()
{
    QList<QTableWidgetItem*> items = selectedItems();
    if (items.count()==1) {
        MaterialTableItem* item = dynamic_cast<MaterialTableItem*>(items.first());
        Material* material = item->getMaterial();
        if (!material) return;
        SelectionModel_->setSelection(material);
    } else {
        SelectionModel_->setSelection(NULL);
    }
}

void MaterialTableView::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MaterialTableView::dragMoveEvent(QDragMoveEvent *event)
{
    QTableWidgetItem * item = itemAt(event->pos());
    if (!item || item->type()!=QTableWidgetItem::UserType+100) {
        event->ignore();
        return;
    }

    MaterialTableItem * matitem = dynamic_cast<MaterialTableItem*>(item);
    Material * material = dynamic_cast<Material*>(matitem->getMaterial());

    Property * property = material->getProperty(event->mimeData()->text());
    if (property) {
        event->ignore();
        return;
    }

    event->acceptProposedAction();
}

void MaterialTableView::dropEvent(QDropEvent *event)
{
    NQLog("MaterialTableView", NQLog::Spam) << "void MaterialTableView::dropEvent(QDropEvent *event)";

    QTableWidgetItem * item = itemAt(event->pos());
    if (!item) {
        event->ignore();
        return;
    }

    MaterialTableItem * matitem = dynamic_cast<MaterialTableItem*>(item);
    Material * material = dynamic_cast<Material*>(matitem->getMaterial());

    Property * property = material->getProperty(event->mimeData()->text());
    if (property) {
        event->ignore();
        return;
    }

    property = PropertyModel_->getProperty(event->mimeData()->text());
    if (!property) {
        event->ignore();
        return;
    }

    material->addProperty(property->clone());

    if (SelectionModel_->getSelection()!=material) {
        SelectionModel_->setSelection(material);
    } else {
        SelectionModel_->materialChanged(material);
    }

    event->acceptProposedAction();
}

void MaterialTableView::itemEdited(QTableWidgetItem * item)
{
    if (!item || item->text().isEmpty()) return;

    MaterialTableItem * matitem = dynamic_cast<MaterialTableItem*>(item);
    Material * material = dynamic_cast<Material*>(matitem->getMaterial());

    if (material) {
        if (material==SelectionModel_->getSelection() && material->getName()!=item->text()) {
            material->setName(item->text());
            fillTable(ListModel_->getMaterialCount());
        }
    } else {

        if (item->type()==QTableWidgetItem::UserType+101) {
            material = new Material();
            material->setUUID(QUuid::createUuid().toString());
            material->setName(item->text());
            ListModel_->addMaterial(material);
        }
    }
}

void MaterialTableView::displayContextMenu(const QPoint& point)
{
    ContextMenu_->exec(mapToGlobal(point));
}
