#ifndef MATERIALPROPERTYVIEW_H
#define MATERIALPROPERTYVIEW_H

#include <QTreeWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMenu>

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
    void selectionChanged();
    void parameterModified(Parameter* parameter);

protected slots:
    void displayContextMenu(const QPoint& point);
    void deleteProperty();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    MaterialListModel* ListModel_;
    MaterialSelectionModel * SelectionModel_;
    PropertyModel* PropertyModel_;
    ParameterModel* ParameterModel_;
    PropertySelectionModel * PropertySelectionModel_;
    ParameterSelectionModel * ParameterSelectionModel_;

    QMenu* ContextMenu_;
};

#endif // MATERIALPROPERTYVIEW_H
