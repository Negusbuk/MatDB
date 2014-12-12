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
    setSelection(0);
}

void MaterialSelectionModel::deleteMaterial()
{
    NQLog("MaterialSelectionModel", NQLog::Spam) << "void deleteMaterial()";

    emit deleteMaterial(Selection_);
    setSelection(0);
}
