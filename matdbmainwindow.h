#ifndef MATDBMAINWINDOW_H
#define MATDBMAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QSplitter>
#include <QToolBar>

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

#include <materialcategorydialog.h>

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
    void editCategories();
    void aboutDialog();

protected:

    void closeEvent(QCloseEvent *event);
    void makeDefaultMaterial();

    QToolBar* ToolBar_;
    QSplitter* HSplitter_;
    QSplitter* VSplitter1_;
    QSplitter* VSplitter2_;
    MaterialTableView* MaterialTableView_;
    MaterialPropertyView* MaterialPropertyView_;
    ParameterStackView* ParameterStackView_;
    MaterialParameterView* MaterialParameterView_;
    PropertyToolBox* PropertyToolBox_;

    MaterialListModel* MaterialListModel_;
    MaterialCategoryModel* MaterialCategoryModel_;
    MaterialSelectionModel* MaterialSelectionModel_;

    ParameterModel* ParameterModel_;
    PropertyModel* PropertyModel_;

    PropertySelectionModel* PropertySelectionModel_;
    ParameterSelectionModel* ParameterSelectionModel_;

    MaterialCategoryDialog* categoryDialog_;
};

#endif // MATDBMAINWINDOW_H
