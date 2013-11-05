#include <materialcategorybox.h>

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

    connect(this, SIGNAL(materialMetadataChanged(Material*)),
            listModel_, SLOT(materialMetadataChanged(Material*)));

    connect(this, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(selectedCategoryChanged(QString)));

    setModel(categoryModel_);
}

void MaterialCategoryBox::materialChanged(Material* material)
{
    setCurrentIndex(0);

    if (!material) return;

    MaterialCategory* category = material->getCategory();
    if (category) {
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
