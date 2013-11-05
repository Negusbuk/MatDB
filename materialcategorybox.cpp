#include <iostream>

#include "materialcategorybox.h"

MaterialCategoryBox::MaterialCategoryBox(MaterialListModel* listModel,
                                         MaterialSelectionModel* selectionModel,
                                         MaterialCategoryModel* categoryModel,
                                         QWidget *parent) :
    QComboBox(parent),
    listModel_(listModel),
    selectionModel_(selectionModel),
    categoryModel_(categoryModel)
{
    connect(selectionModel_, SIGNAL(selectionChanged(Material*)),
            this, SLOT(materialChanged(Material*)));

//    connect(categoryModel_, SIGNAL(categoriesChanged()),
//            this, SLOT(categoriesChanged()));

    connect(this, SIGNAL(materialMetadataChanged(Material*)),
            listModel_, SLOT(materialMetadataChanged(Material*)));

    connect(this, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(selectedCategoryChanged(QString)));

    setModel(categoryModel_);
}

void MaterialCategoryBox::materialChanged(Material* material)
{
    MaterialCategory* category = material->getCategory();
    if (!category) {
        setCurrentIndex(0);
    } else {
        int idx = findText(category->getName());
        setCurrentIndex(idx);
    }
}

void MaterialCategoryBox::selectedCategoryChanged(QString item)
{
    Material * material = selectionModel_->getSelection();
    if (!material) {
        setCurrentIndex(0);
        return;
    }

    if (currentIndex()==0) {
        material->setCategory(0);
    } else {
        material->setCategory(categoryModel_->getCategory(currentText()));
    }

    emit materialMetadataChanged(material);
}

void MaterialCategoryBox::categoriesChanged()
{
/*
    clear();
    addItem("");
    for (std::vector<MaterialCategory*>::const_iterator it = categoryModel_->getCategories().begin();
         it!=categoryModel_->getCategories().end();
         ++it) {
        addItem((*it)->getName());
        std::cout << (*it)->getName().toStdString() << std::endl;
    }

    Material * material = selectionModel_->getSelection();
    if (material && material->getCategory()) {
        int idx = findText(material->getCategory()->getName());
        std::cout << material->getCategory()->getName().toStdString() << " " << idx << std::endl;
        setCurrentIndex(idx);
    } else {
        setCurrentIndex(0);
    }
*/
}
