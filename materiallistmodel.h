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

#ifndef MATERIALLISTMODEL_H
#define MATERIALLISTMODEL_H

#include <vector>

#include <QObject>

#include <material.h>
#include <materialcategorymodel.h>
#include <materialindexer.h>

class MaterialListModel : public QObject
{
    Q_OBJECT
public:
    explicit MaterialListModel(MaterialCategoryModel* model,
                               QObject *parent = 0);
    
    size_t getMaterialCount() const;
    void addMaterial(Material* material);
    void addMaterials(const std::vector<Material*>& materials);
    const std::vector<Material*>& getAllMaterials() const { return MaterialList_; }
    const std::vector<Material*>& getMaterials() const;
    Material* getMaterial(size_t idx);
    Material* findMaterial(const QString& name);
    void removeMaterial(Material* material);

    void read(const QString& filename);
    void sort();
    void sortFiltered();

signals:

    void materialCountChanged(int count);
    void materialListChanged(int count);
    void metadataChanged(Material*);

public slots:

    void materialMetadataChanged(Material*material);
    void duplicateMaterial(Material* selection);
    void deleteMaterial(Material* selection);
    void categoriesChanged();
    void categoryChanged(MaterialCategory*);
    void filterChanged(const QString& filter);

protected:

    MaterialCategoryModel* CategoryModel_;
    MaterialIndexer* MaterialIndexer_;
    std::vector<Material*> MaterialList_;
    bool isFiltered_;
    QString currentFilter_;
    std::vector<Material*> FilteredMaterialList_;
};

#endif // MATERIALLISTMODEL_H
