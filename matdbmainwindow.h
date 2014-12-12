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

#ifndef MATDBMAINWINDOW_H
#define MATDBMAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QSplitter>
#include <QToolBar>
#include <QAction>
#include <QDockWidget>
#include <QTimer>

#include <parametermodel.h>
#include <propertymodel.h>

#include <materiallistmodel.h>
#include <materialcategorymodel.h>
#include <materialselectionmodel.h>
#include <materialtableview.h>
#include <materialpropertyview.h>
#include <materialparameterview.h>
#include <propertytoolbox.h>
#include <propertyselectionmodel.h>
#include <parameterselectionmodel.h>
#include <parameterstackview.h>

class MatDBMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MatDBMainWindow(QWidget *parent = 0);
    virtual ~MatDBMainWindow();

public slots:

    void importMaterials();
    void exportMaterialsXML();
    void exportMaterialsHTML();
    void togglePropertyToolBoxDockWidget();
    void toggleCategoryDockWidget();
    void aboutDialog();
    void preferenceDialog();

    void addDefaultIsotropicMaterial();
    void addDefaultOrthotropicMaterial();
    void addDefaultLiquidMaterial();
    void addDefaultGaseousMaterial();

    void saveData();

protected:

    void closeEvent(QCloseEvent *event);
    void makeDefaultMaterials();
    void changeEvent(QEvent *event);

    QToolBar* ToolBar_;
    QSplitter* HSplitter_;
    QSplitter* VSplitter1_;
    QSplitter* VSplitter2_;
    MaterialTableView* MaterialTableView_;
    MaterialPropertyView* MaterialPropertyView_;
    ParameterStackView* ParameterStackView_;
    MaterialParameterView* MaterialParameterView_;
    PropertyToolBox* PropertyToolBox_;

    QAction* importXMLAction_;
    QAction* addIsotropicMatAction_;
    QAction* addOrthotropicMatAction_;
    QAction* addLiquidMatAction_;
    QAction* addGaseousMatAction_;

    QAction* togglePropertyToolBoxDockWidgetAction_;
    QDockWidget* propertyToolBoxDockWidget_;
    QAction* toggleCategoryDockWidgetAction_;
    QDockWidget* categoryDockWidget_;
    QDockWidget* propertiesDockWidget_;
    QDockWidget* parameterDockWidget_;

    QAction* exportXMLAction_;
    QAction* exportHTMLAction_;

    MaterialListModel* MaterialListModel_;
    MaterialCategoryModel* MaterialCategoryModel_;
    MaterialSelectionModel* MaterialSelectionModel_;

    ParameterModel* ParameterModel_;
    PropertyModel* PropertyModel_;

    PropertySelectionModel* PropertySelectionModel_;
    ParameterSelectionModel* ParameterSelectionModel_;

    QTimer* autoSaveTimer_;
};

#endif // MATDBMAINWINDOW_H
