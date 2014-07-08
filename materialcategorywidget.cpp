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

#include <QBoxLayout>
#include <QLabel>
#include <QHeaderView>

#include "materialcategoryeditdialog.h"
#include "materialcategorywidget.h"

MaterialCategoryWidget::MaterialCategoryWidget(MaterialCategoryModel* categoryModel,
                                               QWidget *parent) :
    QWidget(parent),
    categoryModel_(categoryModel)
{
    setMinimumWidth(200);
    setMinimumHeight(200);

    QBoxLayout * layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    setLayout(layout);

    categories_ = new QListView(this);
    categories_->setAttribute(Qt::WA_MacShowFocusRect, 0);
    layout->addWidget(categories_);

    categories_->setModel(categoryModel_);
    connect(categoryModel_, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            categories_, SLOT(dataChanged(QModelIndex,QModelIndex)));

    connect(this, SIGNAL(categoryChanged(MaterialCategory*)),
            categoryModel_, SLOT(changedCategory(MaterialCategory*)));

    connect(categories_, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(categoryDoubleClicked(QModelIndex)));

    QWidget* tools = new QWidget(this);
    QBoxLayout * hl = new QHBoxLayout();
    hl->setContentsMargins(2,2,2,2);
    hl->setSpacing(2);
    tools->setLayout(hl);
    layout->addWidget(tools);

    addCategoryButton_ = new QPushButton(QIcon(":/icons/MatDBAddCategory.png"), "", this);
    addCategoryButton_->setFlat(true);
    addCategoryButton_->setIconSize(QSize(22,22));
    addCategoryButton_->setFixedSize(26, 26);
    hl->addWidget(addCategoryButton_);
    connect(addCategoryButton_, SIGNAL(clicked()),
            this, SLOT(addCategory()));

    removeCategoryButton_ = new QPushButton(QIcon(":/icons/MatDBRemoveCategory.png"), "", this);
    removeCategoryButton_->setFlat(true);
    removeCategoryButton_->setIconSize(QSize(22,22));
    removeCategoryButton_->setFixedSize(26, 26);
    hl->addWidget(removeCategoryButton_);
    connect(removeCategoryButton_, SIGNAL(clicked()),
            this, SLOT(removeCategory()));

    hl->addStretch(1);
}


void MaterialCategoryWidget::addCategory()
{
    if (categoryModel_->getCategory("New Category")!=NULL) return;
    categoryModel_->addCategory("New Category", tr("New Category"), QColor(242, 142, 0), false);
    categories_->update();
}

void MaterialCategoryWidget::removeCategory()
{
    QItemSelectionModel *sm =  categories_->selectionModel();
    QModelIndex mi = sm->currentIndex();
    QVariant data = categories_->model()->data(mi);
    categoryModel_->removeCategory(data.toString());
}

void MaterialCategoryWidget::categoryDoubleClicked(const QModelIndex& index)
{
    QVariant data = categories_->model()->data(index);
    MaterialCategory* category = categoryModel_->getCategory(data.toString());
    if (!category || category->isReadOnly()) return;

    MaterialCategoryEditDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);

    dialog.setName(category->getDisplayName());

    QColor c = category->getColor();
    dialog.setColor(c);

    int result = dialog.exec();

    if (result==1) {
        bool isChanged = false;

        QString newName = dialog.getName();
        if (newName!=category->getName()) {
            MaterialCategory* dummy = categoryModel_->getCategory(newName);
            if (!dummy) {
                if (newName!=category->getDisplayName()) {
                    categoryModel_->renameCategory(category, newName);
                    isChanged = true;
                }
            }
        }

        QColor newColor = dialog.getColor().toHsv();

        if (newColor.hue()!=c.hue() ||
            newColor.saturation()!=c.saturation() ||
            newColor.value()!=c.value()) {
            category->setColor(newColor);
            isChanged = true;
        }

        if (isChanged) {
            categories_->update();
            emit categoryChanged(category);
        }
    }
}

void MaterialCategoryWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {

        categories_->update();

    } else {
        QWidget::changeEvent(event);
    }
}
