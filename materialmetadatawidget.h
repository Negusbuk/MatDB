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

#ifndef MATERIALMETADATAWIDGET_H
#define MATERIALMETADATAWIDGET_H

#include <QScrollArea>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QFormLayout>

#include <materiallistmodel.h>
#include <materialselectionmodel.h>
#include <materialcategorymodel.h>

class MaterialMetadataWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit MaterialMetadataWidget(MaterialListModel* listModel,
                                    MaterialSelectionModel* selectionModel,
                                    MaterialCategoryModel* categoryModel,
                                    QWidget *parent = 0);
    
signals:
    
public slots:
    
    void materialChanged(Material* material);

protected:

    MaterialListModel* listModel_;
    MaterialSelectionModel* selectionModel_;
    MaterialCategoryModel* categoryModel_;

    QWidget* content_;
    QFormLayout* layout_;

    QLineEdit* descriptionEdit_;
    QPlainTextEdit* notesEdit_;
};

#endif // MATERIALMETADATAWIDGET_H
