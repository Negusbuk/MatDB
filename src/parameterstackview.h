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

#ifndef PARAMETERSTACKVIEW_H
#define PARAMETERSTACKVIEW_H

#include <map>

#include <QStackedWidget>

#include <materiallistmodel.h>
#include <materialselectionmodel.h>
#include <propertyselectionmodel.h>
#include <parameterselectionmodel.h>
#include <property.h>
#include <parameter.h>
#include <materialmetadatawidget.h>
#include <materialparameterview.h>
#include <propertymodel.h>
#include <materialcategorymodel.h>
#include <propertyspecialwidget.h>

class ParameterStackView : public QStackedWidget
{
    Q_OBJECT
public:
    explicit ParameterStackView(MaterialListModel *listmodel,
                                MaterialSelectionModel * selectionmodel,
                                PropertySelectionModel* propertyselectionmodel,
                                ParameterSelectionModel* parameterselectionmodel,
                                PropertyModel* propertymodel,
                                MaterialCategoryModel* categoryModel,
                                QWidget *parent = 0);
    
signals:
    
public slots:
    void materialSelectionChanged(Material* material);
    void propertySelectionChanged(Property* property);
    void parameterSelectionChanged(Parameter* parameter);

protected:
    MaterialListModel* MaterialListModel_;
    MaterialSelectionModel * MaterialSelectionModel_;
    PropertySelectionModel * PropertySelectionModel_;
    ParameterSelectionModel * ParameterSelectionModel_;
    PropertyModel * PropertyModel_;
    MaterialCategoryModel * MaterialCategoryModel_;

    QWidget * Empty_;
    MaterialMetadataWidget * MetadataWidget_;
    MaterialParameterView * MaterialParameterView_;
    std::map<QString,PropertySpecialWidget*> SpecialWidgetMap_;
};

#endif // PARAMETERSTACKVIEW_H
