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

#include <iostream>

#include <QHeaderView>
#include <QLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QLabel>

#include "materialtableitem.h"
#include "materialxmlexportdialog.h"

MaterialXMLExportDialog::MaterialXMLExportDialog(MaterialListModel* model,
                                                 QWidget *parent) :
    QDialog(parent),
    model_(model),
    allItemsChecked_(true),
    exportMode_(ANSYS)
{
    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->setContentsMargins(1,1,1,1);
    setLayout(layout);

    QWidget *buttons = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttons);
    buttonLayout->setContentsMargins(1,1,1,1);
    buttons->setLayout(buttonLayout);
    QButtonGroup* group = new QButtonGroup(buttons);
    connect(group, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(modeChanged(QAbstractButton*)));

    buttonLayout->addWidget(new QLabel("Export Mode:", buttons));

    modeANSYSbutton_ = new QRadioButton("ANSYS", buttons);
    modeANSYSbutton_->setChecked(true);
    group->addButton(modeANSYSbutton_);
    buttonLayout->addWidget(modeANSYSbutton_);

    modeMATMLbutton_ = new QRadioButton("MatML", buttons);
    group->addButton(modeMATMLbutton_);
    buttonLayout->addWidget(modeMATMLbutton_);

    layout->addWidget(buttons);

    materialView_ = new QTableWidget(model_->getMaterialCount(), 2, this);
    materialView_->setHorizontalHeaderItem(0, new QTableWidgetItem("Material"));
    materialView_->setColumnWidth(0, 400);
    materialView_->setHorizontalHeaderItem(1, new QTableWidgetItem("Description"));
    materialView_->setSelectionBehavior(QAbstractItemView::SelectItems);
    materialView_->setSelectionMode(QAbstractItemView::NoSelection);

    materialView_->setAcceptDrops(false);
    materialView_->setDropIndicatorShown(false);
    materialView_->setDragDropMode(QAbstractItemView::NoDragDrop);

    materialView_->setAlternatingRowColors(true);
    materialView_->horizontalHeader()->setStretchLastSection(true);
    connect(materialView_->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),
            this, SLOT(headerViewDoubleClicked(int)));

    materialView_->setMinimumWidth(700);
    materialView_->setMinimumHeight(400);

    int row = 0;
    const std::vector<Material*>& list = model_->getMaterials();
    for (std::vector<Material*>::const_iterator it = list.begin();
         it!=list.end();
         ++it) {
        Material *mat = *it;

        QTableWidgetItem * item;
        item = new MaterialTableItem(mat, QTableWidgetItem::UserType+100);
        item->setText(mat->getName());
        item->setCheckState(Qt::Checked);
        materialView_->setItem(row, 0, item);

        if (!mat->getDescription().isNull() && mat->getDescription()!="") {
            item = new QTableWidgetItem(mat->getDescription());
            materialView_->setItem(row, 1, item);
        }
        row++;
    }

    layout->addWidget(materialView_);

    buttons = new QWidget(this);
    buttonLayout = new QHBoxLayout(buttons);
    buttonLayout->setContentsMargins(1,1,1,1);
    buttons->setLayout(buttonLayout);

    QPushButton *button;

    button = new QPushButton("Export", buttons);
    button->setFlat(true);
    button->setDefault(false);
    connect(button, SIGNAL(clicked()),
            this, SLOT(exportMaterials()));
    buttonLayout->addWidget(button);

    button = new QPushButton("Cancel", buttons);
    button->setFlat(true);
    button->setDefault(false);
    connect(button, SIGNAL(clicked()),
            this, SLOT(reject()));
    buttonLayout->addWidget(button);

    layout->addWidget(buttons);
}

void MaterialXMLExportDialog::exportMaterials()
{
    selectedMaterials_.clear();

    for (int row=0;row<materialView_->rowCount();++row) {
        MaterialTableItem* item = static_cast<MaterialTableItem*>(materialView_->item(row, 0));
        if (item->checkState()!=Qt::Checked) continue;
        selectedMaterials_.push_back(item->getMaterial());
    }

    accept();
}

void MaterialXMLExportDialog::headerViewDoubleClicked(int logicalIndex)
{
    if (logicalIndex!=0) return;

    allItemsChecked_ = !allItemsChecked_;

    for (int row=0;row<materialView_->rowCount();++row) {
        MaterialTableItem* item = static_cast<MaterialTableItem*>(materialView_->item(row, 0));
        item->setCheckState( allItemsChecked_==true ? Qt::Checked : Qt::Unchecked);
    }
}

void MaterialXMLExportDialog::modeChanged(QAbstractButton* button)
{
    if (button==modeMATMLbutton_) exportMode_ = MatML;
    if (button==modeANSYSbutton_) exportMode_ = ANSYS;
    exportMode_ = Unknown;
}
