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
    std::map<QString,QWidget*> SpecialWidgetMap_;
};

#endif // PARAMETERSTACKVIEW_H
