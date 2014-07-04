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

#ifndef MATERIALCATEGORYBOX_H
#define MATERIALCATEGORYBOX_H

#include <QEvent>
#include <QComboBox>

#include <materiallistmodel.h>
#include <materialselectionmodel.h>
#include <materialcategorymodel.h>

class MaterialCategoryBox : public QComboBox
{
    Q_OBJECT
public:
    explicit MaterialCategoryBox(MaterialListModel* listModel,
                                 MaterialSelectionModel* selectionModel,
                                 MaterialCategoryModel* categoryModel,
                                 QWidget *parent = 0);

signals:

    void materialMetadataChanged(Material*);

public slots:

    void materialChanged(Material* material);
    void selectedCategoryChanged(const QString&);

protected:

    void changeEvent(QEvent *event);

    MaterialListModel* listModel_;
    MaterialSelectionModel* selectionModel_;
    MaterialCategoryModel* categoryModel_;
};

#endif // MATERIALCATEGORYBOX_H
