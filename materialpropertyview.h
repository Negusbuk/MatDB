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

#ifndef MATERIALPROPERTYVIEW_H
#define MATERIALPROPERTYVIEW_H

#include <QTreeWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMenu>
#include <QEvent>

#include "material.h"
#include "materiallistmodel.h"
#include "materialselectionmodel.h"
#include "property.h"
#include "parameter.h"
#include "propertymodel.h"
#include "parametermodel.h"
#include <propertyselectionmodel.h>
#include <parameterselectionmodel.h>

class MaterialPropertyViewItem;

class MaterialPropertyViewParameterItem : public QTreeWidgetItem
{
public:
    explicit MaterialPropertyViewParameterItem(Material * material,
                                               Property * property,
                                               Parameter * parameter,
                                               QTreeWidget *parent);
    explicit MaterialPropertyViewParameterItem(Material * material,
                                               Property * property,
                                               Parameter * parameter,
                                               MaterialPropertyViewItem *parent);

    Material* getMaterial() { return Material_; }
    Property* getProperty() { return Property_; }
    Parameter* getParameter() { return Parameter_; }
    int getNumberOfParameters();

    void update();

protected:

    Material* Material_;
    Property* Property_;
    Parameter* Parameter_;
};

class MaterialPropertyViewItem : public QTreeWidgetItem
{
public:
    explicit MaterialPropertyViewItem(Material * material,
                                      Property * property,
                                      QTreeWidget *parent);

    Material* getMaterial() { return Material_; }
    Property* getProperty() { return Property_; }

    void update();

protected:
    Material* Material_;
    Property* Property_;
};

class MaterialPropertyView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit MaterialPropertyView(MaterialListModel *listmodel,
                                  MaterialSelectionModel * selectionmodel,
                                  PropertyModel *propertymodel,
                                  ParameterModel *parametermodel,
                                  PropertySelectionModel* propertyselectionmodel,
                                  ParameterSelectionModel* parameterselectionmodel,
                                  QWidget *parent = 0);
    
signals:
    
public slots:
    void materialChanged(Material* material);
    void selectedPropertyChanged();
    void parameterModified(Parameter* parameter);
    void propertyModified(Property* property);

protected slots:
    void displayContextMenu(const QPoint& point);
    void deleteProperty();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void changeEvent(QEvent *event);

    MaterialListModel* ListModel_;
    MaterialSelectionModel * SelectionModel_;
    PropertyModel* PropertyModel_;
    ParameterModel* ParameterModel_;
    PropertySelectionModel * PropertySelectionModel_;
    ParameterSelectionModel * ParameterSelectionModel_;

    QMenu* ContextMenu_;
    QAction* deleteAction_;
};

#endif // MATERIALPROPERTYVIEW_H
