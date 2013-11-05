#ifndef MATERIALLISTMODEL_H
#define MATERIALLISTMODEL_H

#include <vector>

#include <QObject>

#include <material.h>
#include <materialcategorymodel.h>

class MaterialListModel : public QObject
{
    Q_OBJECT
public:
    explicit MaterialListModel(MaterialCategoryModel* model,
                               QObject *parent = 0);
    
    size_t getMaterialCount() const;
    void addMaterial(Material* material);
    void addMaterials(const std::vector<Material*>& materials);
    const std::vector<Material*>& getMaterials() const { return MaterialList_; }
    Material* getMaterial(size_t idx);
    Material* findMaterial(const QString& name);
    void removeMaterial(Material* material);

    void read(const QString& filename);
    void sort();

signals:

    void materialCountChanged(int count);
    void metadataChanged(Material*);

public slots:

    void materialMetadataChanged(Material*material);
    void duplicateMaterial(Material* selection);
    void deleteMaterial(Material* selection);
    void categoriesChanged();
    void categoryChanged(MaterialCategory*);

protected:

    MaterialCategoryModel* CategoryModel_;
    std::vector<Material*> MaterialList_;
};

#endif // MATERIALLISTMODEL_H
