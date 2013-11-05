#include <QBoxLayout>
#include <QLabel>
#include <QHeaderView>

#include "materialcategoryeditdialog.h"
#include "materialcategorydialog.h"

MaterialCategoryDialog::MaterialCategoryDialog(MaterialCategoryModel* categoryModel,
                                               QWidget *parent) :
    QDialog(parent),
    categoryModel_(categoryModel)
{
    Qt::WindowFlags flags = 0;
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowTitleHint;
    flags |= Qt::WindowCloseButtonHint;
    flags |= Qt::WindowStaysOnTopHint;
    flags |= Qt::Tool;
    setWindowFlags(flags);

    setMinimumWidth(200);
    setMaximumWidth(300);
    setMinimumHeight(200);
    setMaximumHeight(400);

    QBoxLayout * layout = new QVBoxLayout();
    layout->setContentsMargins(3,3,3,3);
    setLayout(layout);

    categories_ = new QListView(this);
    layout->addWidget(categories_);

    categories_->setModel(categoryModel_);
    connect(categoryModel_, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            categories_, SLOT(dataChanged(QModelIndex,QModelIndex)));

    connect(this, SIGNAL(categoryChanged(MaterialCategory*)),
            categoryModel_, SLOT(changedCategory(MaterialCategory*)));

    connect(categories_, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(categoryDoubleClicked(QModelIndex)));

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

    positions_ = QPoint(100, 100);
    size_ = sizeHint();
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

void MaterialCategoryDialog::categoryDoubleClicked(const QModelIndex& index)
{
    QVariant data = categories_->model()->data(index);
    MaterialCategory* category = categoryModel_->getCategory(data.toString());
    if (category->isReadOnly()) return;

    MaterialCategoryEditDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);

    dialog.setName(category->getName());

    QColor c = category->getColor();
    dialog.setColor(c);

    int result = dialog.exec();

    if (result==1) {
        bool isChanged = false;

        QString newName = dialog.getName();
        if (newName!=category->getName()) {
            MaterialCategory* dummy = categoryModel_->getCategory(newName);
            if (!dummy) {
                if (newName!=category->getName()) {
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

void MaterialCategoryDialog::closeEvent(QCloseEvent* /* e*/)
{
    storeGeometry();
}

void MaterialCategoryDialog::storeGeometry()
{
    positions_ = this->pos();
    size_ = this->size();
}

void MaterialCategoryDialog::applyGeometry()
{
    this->move(positions_);
    this->resize(size_);
}
