#include <iostream>

#include <QHeaderView>

#include "materialcategorybox.h"
#include "materialtagsedit.h"
#include "materialmetadatawidget.h"

MaterialMetadataWidget::MaterialMetadataWidget(MaterialListModel* listModel,
                                               MaterialSelectionModel* selectionModel,
                                               MaterialCategoryModel* categoryModel,
                                               QWidget *parent) :
    QScrollArea(parent),
    listModel_(listModel),
    selectionModel_(selectionModel),
    categoryModel_(categoryModel)
{
    setWidgetResizable(true);

    content_ = new QWidget(this);
    layout_ = new QFormLayout(content_);

    layout_->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    this->setWidget(content_);

    descriptionEdit_ = new QLineEdit(content_);
    descriptionEdit_->setAttribute(Qt::WA_MacShowFocusRect, 0);
    layout_->addRow("Description:", descriptionEdit_);

    MaterialCategoryBox * categoryBox;
    categoryBox = new MaterialCategoryBox(listModel_, selectionModel_,
                                          categoryModel_, content_);
    layout_->addRow("Category:", categoryBox);

    MaterialTagsEdit * tagsEdit;
    tagsEdit = new MaterialTagsEdit(listModel_, selectionModel_,
                                    content_);
    tagsEdit->setFixedHeight(80);
    layout_->addRow("Tags:", tagsEdit);

    notesEdit_ = new QPlainTextEdit(content_);
    notesEdit_->setFixedHeight(60);
    layout_->addRow("Notes:", notesEdit_);

    connect(selectionModel_, SIGNAL(selectionChanged(Material*)),
            this, SLOT(materialChanged(Material*)));

    updateGeometry();
}

void MaterialMetadataWidget::materialChanged(Material* material)
{
    std::cout << "void MaterialMetadataWidget::materialChanged()" << std::endl;

    if (material) {
        descriptionEdit_->setText(material->getDescription());
        notesEdit_->setPlainText(material->getNotes());
    } else {
        descriptionEdit_->setText("");
        notesEdit_->setPlainText("");
    }
}
