#include <iostream>

#include <QBoxLayout>
#include <QLabel>
#include <QHeaderView>

#include "materialcategorydialog.h"

MaterialCategoryDialog::MaterialCategoryDialog(MaterialCategoryModel* categoryModel,
                                               QWidget *parent) :
    QDialog(parent),
    categoryModel_(categoryModel)
{
    QBoxLayout * layout = new QVBoxLayout();
    layout->setContentsMargins(3,3,3,3);
    setLayout(layout);

    categories_ = new QListView(this);
    layout->addWidget(categories_);

//    categories_->setSelectionBehavior(QAbstractItemView::SelectItems);
//    categories_->setSelectionMode(QAbstractItemView::SingleSelection);
//    categories_->horizontalHeader()->setStretchLastSection(true);
//    categories_->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
////    categories_->setColumnWidth(0, 302);
//    categories_->horizontalHeader()->hide();
//    categories_->verticalHeader()->hide();

    categories_->setModel(categoryModel_);
    connect(categoryModel_, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            categories_, SLOT(dataChanged(QModelIndex,QModelIndex)));

//    int row = 0;
//    for (std::vector<MaterialCategory*>::const_iterator it = categoryModel_->getCategories().begin();
//         it!=categoryModel_->getCategories().end();
//         ++it) {
//        MaterialCategory* category = *it;

//        QTableWidgetItem* item = new QTableWidgetItem(category->getIcon(), "", QTableWidgetItem::UserType+100);
//        categories_->setItem(row, 0, item);
//        item = new QTableWidgetItem(category->getName(), QTableWidgetItem::UserType+101);
//        categories_->setItem(row, 1, item);

//        row++;
//    }

    QWidget* tools = new QWidget();
    QBoxLayout * hl = new QHBoxLayout();
    hl->setContentsMargins(0,0,0,0);
    tools->setLayout(hl);
    layout->addWidget(tools);

    addCategoryButton_ = new QToolButton();
    addCategoryButton_->setText("+");
    hl->addWidget(addCategoryButton_);
    connect(addCategoryButton_, SIGNAL(clicked()),
            this, SLOT(addCategory()));

    removeCategoryButton_ = new QToolButton();
    removeCategoryButton_->setText("-");
    hl->addWidget(removeCategoryButton_);
    connect(removeCategoryButton_, SIGNAL(clicked()),
            this, SLOT(removeCategory()));

    hl->addStretch(1);

//    connect(categories_, SIGNAL(itemSelectionChanged()),
//            this, SLOT(selectionChanged()));
}

void MaterialCategoryDialog::selectionChanged()
{
//    QListWidgetItem *item = categories_->currentItem();
//    MaterialCategory* category = categoryModel_->getCategory(item->text());

//    nameEdit_->setText(category->getName());
}

void MaterialCategoryDialog::addCategory()
{
    if (categoryModel_->getCategory("New Category")!=NULL) return;
    categoryModel_->addCategory("New Category", QColor(242, 142, 0), false);
    categories_->update();
}

void MaterialCategoryDialog::removeCategory()
{
    QItemSelectionModel *sm =  categories_->selectionModel();
    QModelIndex mi = sm->currentIndex();
    QVariant data = categories_->model()->data(mi);
    categoryModel_->removeCategory(data.toString());
}
