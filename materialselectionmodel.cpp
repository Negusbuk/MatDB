#include <nqlogger.h>

#include <materialselectionmodel.h>

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
    if (Selection_ != selection) {
        Selection_ = selection;
        if (Selection_)
            NQLog("MaterialSelectionModel", NQLog::Spam) << "material selection changed: "
                                                         << selection->getName();
        emit selectionChanged(Selection_);
    }
}

void MaterialSelectionModel::materialChanged(Material* selection)
{
    if (!selection) return;

    NQLog("MaterialSelectionModel", NQLog::Spam) << "material changed: "
                                                 << selection->getName();

    emit selectionChanged(Selection_);
}

void MaterialSelectionModel::materialDetailsRequested(Material* selection)
{
    if (!selection) return;

    NQLog("MaterialSelectionModel", NQLog::Spam) << "material details requested: "
                                                 << selection->getName();

    emit showMaterialDetails(Selection_);
}

void MaterialSelectionModel::duplicateMaterial()
{
    NQLog("MaterialSelectionModel", NQLog::Spam) << "void duplicateMaterial()";

    emit duplicateMaterial(Selection_);
    void deleteMaterial(Material* selection);
    setSelection(0);
}

void MaterialSelectionModel::deleteMaterial()
{
    NQLog("MaterialSelectionModel", NQLog::Spam) << "void deleteMaterial()";

    emit deleteMaterial(Selection_);
}
