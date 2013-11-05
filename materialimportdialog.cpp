#include <QHeaderView>
#include <QLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QLabel>

#include "materialtableitem.h"
#include "materialimportdialog.h"

MaterialImportDialog::MaterialImportDialog(MaterialListModel* model,
                                                 QWidget *parent) :
    QDialog(parent),
    model_(model),
    allItemsChecked_(true)
{
    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->setContentsMargins(1,1,1,1);
    setLayout(layout);

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

    QWidget *buttons = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttons);
    buttonLayout->setContentsMargins(1,1,1,1);
    buttons->setLayout(buttonLayout);

    QPushButton *button = new QPushButton("Import", buttons);
    button->setFlat(true);
    button->setDefault(false);
    connect(button, SIGNAL(clicked()),
            this, SLOT(buildSelectedMaterials()));
    buttonLayout->addWidget(button);

    button = new QPushButton("Cancel", buttons);
    button->setFlat(true);
    button->setDefault(false);
    connect(button, SIGNAL(clicked()),
            this, SLOT(reject()));
    buttonLayout->addWidget(button);

    layout->addWidget(buttons);
}

void MaterialImportDialog::buildSelectedMaterials()
{
    selectedMaterials_.clear();

    for (int row=0;row<materialView_->rowCount();++row) {
        MaterialTableItem* item = static_cast<MaterialTableItem*>(materialView_->item(row, 0));
        Material * material = item->getMaterial();
        model_->removeMaterial(material);
        if (item->checkState()==Qt::Checked) {
            selectedMaterials_.push_back(material);
        } else {
            delete material;
        }
    }

    accept();
}

void MaterialImportDialog::headerViewDoubleClicked(int logicalIndex)
{
    if (logicalIndex!=0) return;

    allItemsChecked_ = !allItemsChecked_;

    for (int row=0;row<materialView_->rowCount();++row) {
        MaterialTableItem* item = static_cast<MaterialTableItem*>(materialView_->item(row, 0));
        item->setCheckState( allItemsChecked_==true ? Qt::Checked : Qt::Unchecked);
    }
}
