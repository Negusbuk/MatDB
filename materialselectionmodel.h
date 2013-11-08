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

#ifndef MATERIALSELECTIONMODEL_H
#define MATERIALSELECTIONMODEL_H

#include <QObject>

#include <material.h>

class MaterialSelectionModel : public QObject
{
    Q_OBJECT
public:
    explicit MaterialSelectionModel(QObject *parent = 0);
    Material* getSelection();

signals:
    void selectionChanged(Material* selection);
    void showMaterialDetails(Material* selection);
    void duplicateMaterial(Material* selection);
    void deleteMaterial(Material* selection);

public slots:
    void setSelection(Material* selection);
    void materialChanged(Material* selection);
    void materialDetailsRequested(Material* selection);
    void duplicateMaterial();
    void deleteMaterial();

protected:
    Material* Selection_;
};

#endif // MATERIALSELECTIONMODEL_H
