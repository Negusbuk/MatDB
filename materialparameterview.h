#ifndef MATERIALPARAMETERVIEW_H
#define MATERIALPARAMETERVIEW_H

#include <QTableWidget>
#include <QComboBox>
#include <QMenu>
#include <QAction>

#include <materiallistmodel.h>
#include <materialselectionmodel.h>
#include <propertyselectionmodel.h>
#include <parameterselectionmodel.h>
#include <unitcombobox.h>
#include <unit.h>

class MaterialParameterViewItem : public QTableWidgetItem
{
public:
    explicit MaterialParameterViewItem(ParameterValue* parameter,
                                       int column);
    ParameterValue* getParameterValue() { return ParameterValue_; }
    int getColumn() const { return Column_; }
    void update();

protected:
    ParameterValue* ParameterValue_;
    int Column_;
};

class MaterialParameterView : public QWidget
{
    Q_OBJECT
public:
    explicit MaterialParameterView(MaterialListModel *listmodel,
                                   MaterialSelectionModel *selectionmodel,
                                   PropertySelectionModel *propertyselectionmodel,
                                   ParameterSelectionModel *parameterselectionmodel,
                                   QWidget *parent = 0);
    
signals:
    
public slots:
    void materialChanged(Material*);
    void parameterChanged(Parameter*);
    void parameterValueChanged(QTableWidgetItem*);
    void temperatureUnitChanged(const QString& name);
    void valueUnitChanged(const QString& name);

protected slots:
    void displayContextMenu(const QPoint& point);
    void import();
    void deleteParameterRow();
    void deleteParameterRow(int row);
    void deleteTemperature();
    void deleteTemperature(int row);

protected:
    MaterialListModel* ListModel_;
    MaterialSelectionModel * SelectionModel_;
    PropertySelectionModel* PropertySelectionModel_;
    ParameterSelectionModel* ParameterSelectionModel_;

    QTableWidget* unitTable_;
    QTableWidget* valueTable_;

    UnitComboBox* TempUnitBox_;
    UnitComboBox* ValueUnitBox_;

    QMenu* ContextMenu_;
    QAction* DeleteAction_;
    QAction* DeleteTempAction_;
};

#endif // MATERIALPARAMETERVIEW_H
