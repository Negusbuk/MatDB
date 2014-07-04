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

#include <nqlogger.h>

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

    connect(this, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(selectedCategoryChanged(const QString&)));

    setModel(categoryModel_);
}

void MaterialCategoryBox::materialChanged(Material* material)
{
    if (!material) {
        setCurrentIndex(0);
        return;
    }

    MaterialCategory* category = material->getCategory();
    if (category) {
        int idx = findText(category->getDisplayName());
        setCurrentIndex(idx);
    } else {
        setCurrentIndex(0);
    }
}

void MaterialCategoryBox::selectedCategoryChanged(const QString& /* item */)
{
    Material * material = selectionModel_->getSelection();
    if (!material) {
        setCurrentIndex(0);
        return;
    }

    if (currentIndex()==0) {
        material->setCategory(0);
    } else {
        material->setCategory(categoryModel_->getCategoryByDisplayName(currentText()));
    }

    emit materialMetadataChanged(material);
}

void MaterialCategoryBox::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {

    } else {
        QWidget::changeEvent(event);
    }
}
