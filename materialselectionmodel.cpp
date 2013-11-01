#include <iostream>

#include "materialselectionmodel.h"

MaterialSelectionModel::MaterialSelectionModel(QObject *parent) :
    QObject(parent)
{
    Selection_ = 0;
}

Material* MaterialSelectionModel::getSelection()
{
    return Selection_;
}

void MaterialSelectionModel::setSelection(Material* selection)
{
    if (!selection) return;
    if (Selection_ != selection) {
        Selection_ = selection;
        std::cout << "material selection changed: " << selection->getName().toStdString() << std::endl;
        emit selectionChanged(Selection_);
    }
}

void MaterialSelectionModel::materialChanged(Material* selection)
{
    if (!selection) return;
    std::cout << "material changed: " << selection->getName().toStdString() << std::endl;
    emit selectionChanged(Selection_);
}

void MaterialSelectionModel::materialDetailsRequested(Material* selection)
{
    if (!selection) return;
    std::cout << "material details requested: " << selection->getName().toStdString() << std::endl;
    emit showMaterialDetails(Selection_);
}

void MaterialSelectionModel::duplicateMaterial()
{
    std::cout << "void MaterialSelectionModel::duplicateMaterial()" << std::endl;
    emit duplicateMaterial(Selection_);
    void deleteMaterial(Material* selection);
    setSelection(0);
}

void MaterialSelectionModel::deleteMaterial()
{
    std::cout << "void MaterialSelectionModel::deleteMaterial()" << std::endl;
    emit deleteMaterial(Selection_);
}
