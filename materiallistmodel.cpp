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

#include <algorithm>

#include <nqlogger.h>

#include <materiallistmodel.h>

MaterialListModel::MaterialListModel(MaterialCategoryModel* model,
                                     QObject *parent) :
    QObject(parent),
    CategoryModel_(model),
    isFiltered_(false)
{
    MaterialIndexer_ = new MaterialIndexer(this);

    connect(CategoryModel_, SIGNAL(categoriesChanged()),
            this, SLOT(categoriesChanged()));
    connect(CategoryModel_, SIGNAL(categoryChanged(MaterialCategory*)),
            this, SLOT(categoryChanged(MaterialCategory*)));
}

size_t MaterialListModel::getMaterialCount() const
{
    if (isFiltered_) return FilteredMaterialList_.size();
    return MaterialList_.size();
}

void MaterialListModel::addMaterial(Material* material)
{
    QString basename = material->getName();
    int index = 2;
    QString newname = basename;
    while (findMaterial(newname)!=0) {
        newname = basename + " " + QString::number(index);
        index++;
    }
    material->setName(newname);
    MaterialList_.push_back(material);

    sort();

    emit materialListChanged(MaterialList_.size());

    if (isFiltered_) {
        filterChanged(currentFilter_);
    } else {
        emit materialCountChanged(getMaterialCount());
    }
}

void MaterialListModel::addMaterials(const std::vector<Material*>& materials)
{
    NQLog("MaterialListModel") << "adding " << materials.size() << " materials";

    for (std::vector<Material*>::const_iterator it = materials.begin();
         it!=materials.end();
         ++it) {
        Material * material = *it;
        QString basename = material->getName();
        int index = 2;
        QString newname = basename;
        while (findMaterial(newname)!=0) {
            newname = basename + " " + QString::number(index);
            index++;
        }
        material->setName(newname);
        MaterialList_.push_back(material);
    }

    sort();

    emit materialListChanged(MaterialList_.size());

    if (isFiltered_) {
        filterChanged(currentFilter_);
    } else {
        emit materialCountChanged(getMaterialCount());
    }
}

const std::vector<Material*>& MaterialListModel::getMaterials() const
{
    if (isFiltered_) return FilteredMaterialList_;
    return MaterialList_;
}

Material* MaterialListModel::getMaterial(size_t idx)
{
    if (idx>=MaterialList_.size()) return 0;
    return MaterialList_[idx];
}

void MaterialListModel::read(const QString& /* filename */)
{

}

void MaterialListModel::sort()
{
    std::sort(MaterialList_.begin(), MaterialList_.end(),
              [](Material*lhs, Material*rhs) {
        int result = QString::localeAwareCompare(lhs->getName(), rhs->getName());
        return (result<0);
    });
}

void MaterialListModel::sortFiltered()
{
    if (isFiltered_) {
        std::sort(FilteredMaterialList_.begin(), FilteredMaterialList_.end(),
                  [](Material*lhs, Material*rhs) {
            int result = QString::localeAwareCompare(lhs->getName(), rhs->getName());
            return (result<0);
        });
    }
}

Material* MaterialListModel::findMaterial(const QString& name)
{
    for (std::vector<Material*>::iterator it = MaterialList_.begin();
         it!=MaterialList_.end();
         ++it) {
        if ((*it)->getName()==name) return *it;
    }
    return 0;
}

void MaterialListModel::removeMaterial(Material* material)
{
    std::vector<Material*>::iterator it = std::find(MaterialList_.begin(),
                                                    MaterialList_.end(),
                                                    material);
    if (it==MaterialList_.end()) return;
    MaterialList_.erase(it);
}

void MaterialListModel::materialMetadataChanged(Material* material)
{
    emit metadataChanged(material);
}

void MaterialListModel::duplicateMaterial(Material* material)
{
    NQLog("MaterialListModel") << "void MaterialSelectionModel::duplicateMaterial() "
                               << material->getName();

    Material* mat = material->clone();
    addMaterial(mat);
}

void MaterialListModel::deleteMaterial(Material* material)
{
    NQLog("MaterialListModel") << "void MaterialSelectionModel::deleteMaterial() "
                               << material->getName();

    for (std::vector<Material*>::iterator it = MaterialList_.begin();
         it!=MaterialList_.end();
         ++it) {
        Material * mat = *it;
        if (mat==material) {
            MaterialList_.erase(it);

            emit materialListChanged(MaterialList_.size());
            materialCountChanged(MaterialList_.size());

            break;
        }
    }
}

void MaterialListModel::categoriesChanged()
{
    for (std::vector<Material*>::iterator it = MaterialList_.begin();
         it!=MaterialList_.end();
         ++it) {
        Material * mat = *it;
        if (mat->getCategory()==0) continue;

        if (!CategoryModel_->isCategoryValid(mat->getCategory())) {
            mat->setCategory(0);
            emit metadataChanged(mat);
        }
    }
}

void MaterialListModel::categoryChanged(MaterialCategory* category)
{
    for (std::vector<Material*>::iterator it = MaterialList_.begin();
         it!=MaterialList_.end();
         ++it) {
        Material * mat = *it;
        if (mat->getCategory()==0) continue;

        if (mat->getCategory()==category) emit metadataChanged(mat);
    }
}

void MaterialListModel::filterChanged(const QString& filter)
{
    NQLog("MaterialListModel", NQLog::Spam) << "void filterChanged(const QString& filter) "
                                            << filter << " " << currentFilter_;

    if (currentFilter_!=filter) {
        MaterialIndexer_->filter(filter, FilteredMaterialList_);
        sortFiltered();
        isFiltered_ = (filter.length()!=0);
        emit materialCountChanged(getMaterialCount());
    }

    currentFilter_ = filter;
    isFiltered_ = (filter.length()!=0);
}
